//
// art source for converting MIDAS runs/events into art objects
// Creates event/run principals, and fills art records with MIDAS header and bank data
//
// Handles:
//   Event MIDAS header
//   MIDAS banks in events
//   ODB dump (recorded at BOR)
//
// User provides a list of desired banks in the fcl file
// If a specified bank is present in an event, it will be put in the event in art record form
// If it is not in the event (e.g. no bank in MIDAS file), nothing is put (user modules should 
// check if it exists in event before resolving the handle)
//
// Two modes:
//   Online - recevies MIDAS event via socket from MIDAS mserver
//          - waits for each new event from the running DAQ
//          - use as backend for DQM applications
//          - exit with ctrl+c
//   Offline - reads MIDAS events from a MIDAS file
//           - use for offline processing of data after a run is over
//
// FIXME Currently put a collection of MidasBankArtRecords in event for each bank
//       However, this always has only a single entry (as put a different product in the event for every
//       bank, as use the bank name as the instance label).
//       If there is no bank then nothing is put, so don't need a vector to tell if the bank was there
//       This is historical, but should be removed to save users needing to check there is exactly 1 entry
//       in vector for each event. TODO this, but requires updates to downstream code reprocessing of e.g. 
//       SLAC data so needs some coordinating
//
// Note that using the art::Source "readFile", "readNext" and "closeCurrentFile" functions in online mode because
// they must be implemented, but the names don't make much sense in this case. Basically, using "readFile" as "open
// connection", "readNext" for the online loop and "closeCurrentFile" to close connection (after "readNext" returns
// false). We assume only one "file" here, e.g. we only open connection once.
//
// Based heavily on examples from:
//   art source by Rob Kutschke, mu2e (Offline/Sandbox/src/Source00_source.cc)
//   ROME MIDAS parsing by Ryu Sawada, PSI (rome/examples/midas/standalone2, rome/src/ROMEMidasDAQ, rome/src/ROMEMidasFile)
//   ROOTANA MIDAS parsing by Konstantin Olchanski, TRIUMF (TMidasOnline.cxx)
//   Conversations with Andrew Norman, NOvA (see https://cdcvs.fnal.gov/redmine/projects/novaart/repository/show/trunk/DAQ2RawDigit)
//
// Tom Stuttard (thomas.stuttard.13@ucl.ac.uk)
//

//C++
#include <iostream>
#include <boost/utility.hpp>
#include <boost/filesystem.hpp>
#include <sys/types.h>

//art
#include "art/Framework/IO/Sources/Source.h"
#include "art/Framework/Core/InputSourceMacros.h"
#include "art/Framework/IO/Sources/SourceHelper.h"
#include "art/Persistency/Provenance/Timestamp.h"
#include "art/Persistency/Provenance/RunID.h"
#include "art/Persistency/Provenance/SubRunID.h"
#include "art/Persistency/Provenance/EventID.h"
#include "art/Framework/Principal/RunPrincipal.h"
#include "art/Framework/Principal/SubRunPrincipal.h"
#include "art/Framework/Principal/EventPrincipal.h"
#include "art/Framework/IO/Sources/put_product_in_principal.h"

//Message facility
#include "messagefacility/MessageLogger/MessageLogger.h"

//MIDAS
#include "midas.h"

//Art records
#include "gm2dataproducts/daq/MidasEventHeaderArtRecord.hh"
#include "gm2dataproducts/daq/MidasEventArtRecord.hh"
#include "gm2dataproducts/daq/MidasODBArtRecord.hh"

//C/C++/boost
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <zlib.h> //gz file read
#include <unistd.h> //plain file read
#include <fcntl.h> //plain file read
#include <errno.h>

//Typedefs for MIDAS consistency
typedef signed short int SHORT;

namespace gm2midastoart {

    class MidasBankInputDetail : private boost::noncopyable {

        public:

            // The signatures and return types of the c'tor and the three member functions
            // are prescribed by the art::Source class template.
            MidasBankInputDetail(const fhicl::ParameterSet&,
                    art::ProductRegistryHelper&,
                    const art::SourceHelper&);

            ~MidasBankInputDetail();

            void readFile(std::string const& filename, //Required for art sources
                    art::FileBlock*& fb);

            bool readNext(art::RunPrincipal*    const& inR, //Required for art sources
                    art::SubRunPrincipal* const& inSR,
                    art::RunPrincipal*&          outR,
                    art::SubRunPrincipal*&       outSR,
                    art::EventPrincipal*&        outE);

            void closeCurrentFile(); //Required for art sources

        private:

            //Subfunctions for online/offline event read //TODO Do these need the "in" variable?
            bool readEventFromSocket(art::RunPrincipal* const& inR,
                                     art::SubRunPrincipal* const& inSR,
                                     art::RunPrincipal*&          outR,
                                     art::SubRunPrincipal*&       outSR,
                                     art::EventPrincipal*&        outE);

            bool readEventFromFile(art::RunPrincipal* const& inR,
                                   art::SubRunPrincipal* const& inSR,
                                   art::RunPrincipal*&          outR,
                                   art::SubRunPrincipal*&       outSR,
                                   art::EventPrincipal*&        outE);

            bool processEvent(art::RunPrincipal* const& inR,
                              art::SubRunPrincipal* const& inSR,
                              art::RunPrincipal*&          outR,
                              art::SubRunPrincipal*&       outSR,
                              art::EventPrincipal*&        outE);

            //MIDAS file handling
            bool openMidasFile(std::string filePath);
            void closeMidasFile();
            ssize_t readMidasFileBlock(void *buf, size_t size);

            //Online connection handling
            bool connectToMidasExperiment();
            void disconnectFromMidasExperiment();

            //ODB handling
            INT dumpODB(); //Dumps the ODB to the event (online mode)
            void readODB(art::RunPrincipal*& outR, char* odbBuffer, INT odbSize); //Read ODB from event and create art record

            //Read in a MIDAS bank from the event and fill corresponding art record
            template<typename D> bool readBank(art::EventPrincipal*& outE, const std::string& bankName, MidasEventHeaderArtRecord* eventRecord);

            // A helper function used to manage the principals.
            // This is boilerplate that does not change if you change the data products.
            void managePrincipals ( int runNumber,
                    int subRunNumber,
                    int eventNumber,
                    art::RunPrincipal*&    outR,
                    art::SubRunPrincipal*& outSR,
                    art::EventPrincipal*&  outE);

            //Callback function for receipt of MIDAS system messages
            static void handleMidasMessage(HNDLE hBuf, HNDLE id, EVENT_HEADER *header, void *message);

            //Offline mode handling: File info
            boost::filesystem::path currentFileName_; //Full path
            enum MidasFileType{ UNKNOWN=0, MID, GZ }; //Define known file types
            MidasFileType fileType_; //File type of current file
            int plainFileHandle_; //File handle when using .mid file
            gzFile gzFileHandle_; //File handle when using .mid.gz file

            //Online mode handline: Connection info
            std::string mserverHost_; //Format is "host:port"
            std::string midasExperimentName_;
            std::string analyzerName_;
            unsigned int onlineLoopPeriodMs_; //Time in [ms] for MIDAS yield. Keeps as 0 unless want to deliberately introduce delay
            short int midasTriggerMask_;
            bool connectToMidasMessageSystem_;
            HNDLE onlineBuffer_;
            HNDLE odbHandle_;
            bool firstRunStarted_;

            //FCL params common to both online and offline modes 
            std::string moduleLabel_;
            bool online_; //Choose online of offline mode 
            std::vector<std::string> bankNames_; //List of "names,type" for banks to parse (four character bank identifier)
            bool parseODB_; //Flag indicating ODB dump should be parsed in art form
            bool swapEndian_; //Flag to enable endian swap (big <-> little)
            bool forceEndianSwap_; //Force endian swap even if MIDAS header says not required (e.g. if a backend requires it)
            bool realData_; //True indicates that this is real data from the DAQ (as opposed to sim data, so should always be true here)
            unsigned int maxBankWordsToPrint_; //Max words to print for each bank

            //Principal data
            int currentRunNumber_;
            int currentSubRunNumber_;
            int currentEventNumber_;

            // Helper object needed by the member function managePrincipals.
            art::SourceHelper pm_;

            // Used to identify boundaries between runs and subruns.
            art::SubRunID lastSubRunID_;

            //Buffer for MIDAS event read
            union {
                char         *rawData_; //Use when accessing raw bytes
                EVENT_HEADER *pevent_;  //Use when want to access header
            };

            //Buffer for MIDAS bank read
            BANK_HEADER *pbank_;

            //Buffer for MIDAS ODB read
            char *odbData_;

    };//MidasBankInputDetail


    //----------------------------------------------------------------
    MidasBankInputDetail::MidasBankInputDetail(const fhicl::ParameterSet& pset,
            art::ProductRegistryHelper& rh,
            const art::SourceHelper& pm)
          : currentFileName_()
          , fileType_( MidasFileType::UNKNOWN )
          , plainFileHandle_(-1)
          , gzFileHandle_(Z_NULL)
          , mserverHost_( pset.get<std::string>("detail.mserverHost","localhost:7070") )
          , midasExperimentName_( pset.get<std::string>("detail.midasExperimentName","gm2") )
          , analyzerName_( pset.get<std::string>("detail.analyzerName","DQM") )
          , onlineLoopPeriodMs_( pset.get<unsigned int>("detail.onlineLoopPeriodMs",0) ) //Use 0 unless have reason not to
          , midasTriggerMask_( pset.get<short int>("detail.midasTriggerMask",TRIGGER_ALL) )
          , connectToMidasMessageSystem_( pset.get<bool>("detail.connectToMidasMessageSystem",true) )
          , firstRunStarted_(false)
          , moduleLabel_( pset.get<std::string>("detail.moduleLabel","MidasBankInput") )
          , online_( pset.get<bool>("detail.online",false) )
          , bankNames_( pset.get<std::vector<std::string>>("detail.bankNames") )
          , parseODB_( pset.get<bool>("detail.parseODB",true) )
          , swapEndian_( pset.get<bool>("detail.swapEndian",false) )
          , forceEndianSwap_( pset.get<bool>("detail.forceEndianSwap",false) )
          , realData_( pset.get<bool>("detail.realData",true) ) //By deafult assume a MIDAS bank is real data
          , maxBankWordsToPrint_( pset.get<unsigned int>("detail.maxBankWordsToPrint",10) )
          , currentRunNumber_(-1)
          , currentSubRunNumber_(-1)
          , currentEventNumber_(-1)
          , pm_(pm)
          , lastSubRunID_()
    {

        //Report real data status
        mf::LogVerbatim("MidasBankInput") << "Marking event as " << ( realData_ ? "" : "NOT " ) << "containing real data";

        //Check bank definitions vector is not empty
        if(bankNames_.size() == 0) {
            throw art::Exception(art::errors::Configuration) << "No MIDAS bank names supplied";
        }

        //Setup event info write
        rh.reconstitutes<MidasEventHeaderArtRecord,art::InEvent>(moduleLabel_,""); //No instance name
        rh.reconstitutes<MidasODBArtRecord,art::InRun>(moduleLabel_,"MidasODB"); //No instance name

        //Setup each bank
        for( auto bankName : bankNames_ ) {

            //Check all bank names have exactly 4 characters (enforced by MIDAS)
            if(bankName.size() != MIDAS_BANK_NAME_LENGTH) {
                throw art::Exception(art::errors::Configuration) << "MIDAS bank name not 4 characters : " << bankName;
            }

            //Setup art record production for each bank...

            //"reconstitutes" is the analog of the call to produces<T> in a producer module.
            //Same pattern is used for art::InRun and art::InSubRun objects.
            //Note: Using bank name as instance label (to differentiate between numbered banks of same type)

            /*
               >>> IF NEW BANK ADDED, ADD LINE HERE <<
               */
            if( TrackerBank::CompareBankName(bankName) ) { 
                rh.reconstitutes< std::vector< MidasEventArtRecord<TrackerBank> > ,art::InEvent >(moduleLabel_,bankName);
            }
            else if( TrackerGLIBBank::CompareBankName(bankName) ) { 
                rh.reconstitutes< std::vector< MidasEventArtRecord<TrackerGLIBBank> > ,art::InEvent >(moduleLabel_,bankName);
            }
            else if( TrackerSlowBank::CompareBankName(bankName) ) { 
                rh.reconstitutes< std::vector< MidasEventArtRecord<TrackerSlowBank> > ,art::InEvent >(moduleLabel_,bankName);
            }
            else if( MasterTriggerBank::CompareBankName(bankName) ) { 
                rh.reconstitutes< std::vector< MidasEventArtRecord<MasterTriggerBank> > ,art::InEvent >(moduleLabel_,bankName);
            }
            else if( GPSBank::CompareBankName(bankName) ) { 
                rh.reconstitutes< std::vector< MidasEventArtRecord<GPSBank> > ,art::InEvent >(moduleLabel_,bankName);
            }
            else if( CaloHeaderBank::CompareBankName(bankName) ) { 
                rh.reconstitutes< std::vector< MidasEventArtRecord<CaloHeaderBank> > ,art::InEvent >(moduleLabel_,bankName);
            }
            else if( CaloRawBank::CompareBankName(bankName) ) {
                rh.reconstitutes< std::vector< MidasEventArtRecord<CaloRawBank> > ,art::InEvent >(moduleLabel_,bankName);
            }
            else if( CaloTBank::CompareBankName(bankName) ) {
                rh.reconstitutes< std::vector< MidasEventArtRecord<CaloTBank> > ,art::InEvent >(moduleLabel_,bankName);
            }
            else if( CaloCQBank::CompareBankName(bankName) ) {
                rh.reconstitutes< std::vector< MidasEventArtRecord<CaloCQBank> > ,art::InEvent >(moduleLabel_,bankName);
            }
            else if( CaloCHBank::CompareBankName(bankName) ) {
                rh.reconstitutes< std::vector< MidasEventArtRecord<CaloCHBank> > ,art::InEvent >(moduleLabel_,bankName);
            }
            else if( CaloCPBank::CompareBankName(bankName) ) {
                rh.reconstitutes< std::vector< MidasEventArtRecord<CaloCPBank> > ,art::InEvent >(moduleLabel_,bankName);
            }
	    else if( GalilBank::CompareBankName(bankName) ) {
	        rh.reconstitutes< std::vector< MidasEventArtRecord<GalilBank> > ,art::InEvent >(moduleLabel_,bankName);
	    }

            else {
                throw art::Exception(art::errors::Configuration) << "Unrecognised MIDAS bank : " << bankName;
            }

        }//bank name loop

        //Allocate memory for buffers
        rawData_ = new char[DEFAULT_MAX_EVENT_SIZE];
        if(online_) odbData_ = new char[DEFAULT_MAX_EVENT_SIZE];

    }


    //----------------------------------------------------------------
    MidasBankInputDetail::~MidasBankInputDetail() {

        //Free memory
        if(rawData_) delete [] rawData_ ; 
        if(online_) {
          if(odbData_) delete [] odbData_ ; 
        }

    }


    //----------------------------------------------------------------
    void MidasBankInputDetail::readFile(const std::string& filename, art::FileBlock*& fb) {

        //Init principal data
        currentSubRunNumber_ = 0; //Not used on g-2, also 0 in simulation
        currentEventNumber_ = -1; //Will be overwritten from data

        //Connect to experiment if running online
        if( online_ ) {
          if( ! connectToMidasExperiment() ) {
            throw art::Exception(art::errors::FileOpenError) << "Cannot connect to MIDAS experiment\n";
          }
        }

        //Open file if running offline
        else {
          if( ! openMidasFile(filename) ) {
            throw art::Exception(art::errors::FileOpenError) << "Cannot open input MIDAS file : " << currentFileName_ << "\n";
          }
        }

        // A FileBlock is the object art uses to maintain state information about the file.
        // art takes ownership of this pointer and will call delete on it at the appropriate time.
        // In online mode we don't really use this, but still needs to be here
        fb = new art::FileBlock( art::FileFormatVersion(1, "MidasBankInput"), currentFileName_.string() );
    }


    //----------------------------------------------------------------
    void MidasBankInputDetail::closeCurrentFile() {

        //Note that this is called in event of a throw in readNext

        //If online, disconnect from experiment
        if( online_ ) {
          mf::LogVerbatim("MidasBankInput") << "Disconnecting from MIDAS experiment " << midasExperimentName_;
          disconnectFromMidasExperiment();
        }

        //If offline, close the current MIDAS file
        else {
          mf::LogVerbatim("MidasBankInput") << "Closing file " << currentFileName_;
          closeMidasFile();
        }

        currentFileName_ = "";

    }


    //----------------------------------------------------------------
    bool MidasBankInputDetail::readNext(art::RunPrincipal*    const& inR,
            art::SubRunPrincipal* const& inSR,
            art::RunPrincipal*&          outR,
            art::SubRunPrincipal*&       outSR,
            art::EventPrincipal*&        outE)
    {

       //Online vs offline cases
       if( online_ ) return readEventFromSocket(inR, inSR, outR, outSR, outE);
       else return readEventFromFile(inR, inSR, outR, outSR, outE);

    } // readNext()


    //----------------------------------------------------------------
    void MidasBankInputDetail::disconnectFromMidasExperiment() {

      //Disconnect from the MIDAS experiment
      cm_disconnect_experiment();

    }


    //----------------------------------------------------------------
    bool MidasBankInputDetail::connectToMidasExperiment() {

      //Disconnect any previous connection before starting
      disconnectFromMidasExperiment();


      //
      // Connect to MIDAS experiment via the mserver
      //

      if (cm_connect_experiment(mserverHost_.c_str(), midasExperimentName_.c_str(),  analyzerName_.c_str(), 0) != SUCCESS ) {
        throw art::Exception(art::errors::Configuration) << "Cannot connect to experiment \"" << midasExperimentName_ 
                                                         << "\" running on \"" << mserverHost_ << "\"\n";
      }

      mf::LogVerbatim("MidasBankInput") << "Connected to experiment \"" << midasExperimentName_ << "\" running on \"" 
                                        << mserverHost_ << "\" as analyzer \"" << analyzerName_ << "\"";

      //Note that at this point MIDAS has taken control of the ctrl+c interrupt signal (sig 2), so art no longer catches it.
      //In the online loop we receive the resuling MIDAS signal and start the exit procedure.
      //If in the future we want to change this interrupt handling, can take back ownership of ctrl+c handling by passing
      //a function ("void func(int)") to ss_ctrlc_handler(<function ptr>).

      //Note that currently using ctrl+c to exit means that the desired RootOutput file is never renamed at the end of
      //the job. Currently not using one, but if we do in the future we'll need to address this.

 
      //
      // Create a receive buffer for event
      //

      //Open the event buffer
      if( bm_open_buffer(EVENT_BUFFER_NAME, DEFAULT_BUFFER_SIZE, &onlineBuffer_) != BM_SUCCESS ) {
        throw art::Exception(art::errors::Configuration) << "Error opening receive buffer\n";
      }

      //mf::LogVerbatim("MidasBankInput") << "Receive buffer for MIDAS events opened";


      //
      // Request events be sent to us
      //

      //Request that mserver sends us events as they appear
      //For sampling_type type, choosing GET_NONBLOCKING as this doesn't slow down the producer (event builder at all)
      //Could use GET_ALL, but this would slow the DAQ if the DQM is slow, which we don't want
      //Only requesting events with event_id == 1 (EB events), as handle BOR, EOR, etc via transitions instead
      //(also, never seem to actually receive EVENTID_BOR/EOR events via this system anyway)
      //Not registering a callback function, will receive event in polling loop instead
      INT request_id;
      if( bm_request_event(onlineBuffer_, 1, midasTriggerMask_, GET_NONBLOCKING, &request_id, NULL) != BM_SUCCESS ) {
        throw art::Exception(art::errors::Configuration) << "Error setting up event requests\n";
      }


       //
      // Register to receive MIDAS transitions (BOR/EOR)
      //

      //Request that we are notified of run start and stop transitions
      //Setting a null pointer for callback function, as instead going to poll 
      //for transitions in our online loop using cm_query_transition
      //500 is default priority, e.g. we don't need to be before or after other clients (frontends)
      if( cm_register_transition(TR_START, NULL, 500) != CM_SUCCESS ||
          cm_register_transition(TR_STOP, NULL, 500) != CM_SUCCESS) {
        throw art::Exception(art::errors::Configuration) << "Error requesting transitions from MIDAS\n";
      }


      //
      // Connect to message system
      //

      //Register a callback function to process incoming messages from MIDAS
      if( connectToMidasMessageSystem_ ) {
        cm_msg_register(MidasBankInputDetail::handleMidasMessage);
        mf::LogVerbatim("MidasBankInput") << "Connected to MIDAS messaging system";
      }


      //
      // Connect to ODB
      //

      //Connect to the MIDAS ODB
      if( cm_get_experiment_database(&odbHandle_, 0) != CM_SUCCESS) {
        throw art::Exception(art::errors::Configuration) << "Cannot connect to MIDAS ODB\n";
      }

      //Record most recent run number
      //Use this if this program starts mid-way through a MIDAS run, in
      //which case we don't receive a BOR transition so can't get run number this way)
      INT size = sizeof(currentRunNumber_);
      if( db_get_value(odbHandle_, 0, const_cast<char*>("/Runinfo/Run number"), &currentRunNumber_, &size, TID_INT, kFALSE) != CM_SUCCESS) {
        throw art::Exception(art::errors::Configuration) << "Cannot read run number from MIDAS ODB\n";
      }
      //mf::LogVerbatim("MidasBankInput") << "Most recent MIDAS run number is " << currentRunNumber_;


      //
      // Done
      //

      //TODO Set cache size?
      //TODO Set watchdog period?

      return true;

    }//connectToMidasExperiment


    //----------------------------------------------------------------
    bool MidasBankInputDetail::openMidasFile(std::string filePath) {

      //
      // Check inputs
      //

      currentFileName_ = filePath;
      mf::LogVerbatim("MidasBankInput") << "Opening file : " << currentFileName_;

      //Close any previous file
      closeMidasFile();

      //Check current file exists
      if( ! boost::filesystem::exists(currentFileName_) ) {
        throw art::Exception(art::errors::FileOpenError) << "File \""<< currentFileName_ << "\" does not exist\n";
      }


      //
      // Handle different MIDAS file types
      //

      //Use extension to determine file type
      std::string tmpStringName = currentFileName_.string();
      std::string fileExtension = tmpStringName.substr(tmpStringName.find_last_of(".") + 1);
      //mf::LogVerbatim("MidasBankInput") << "File extension is " << fileExtension;

      //Pain MIDAS file (no compression)
      if( fileExtension == "mid" ) {
        fileType_ = MidasFileType::MID;
      }
  
      //GZ compression
      else if( fileExtension == "gz" ) {
        fileType_ = MidasFileType::GZ;
      }

      //Other compression types that are not currently supported
      //Implement if we decide to use them on g-2, but aren't currently used
      else if( fileExtension == "xz" ) {
        throw art::Exception(art::errors::UnimplementedFeature) << "Not currently supporting XZ compression\n";
        return false;
      }

      else if( fileExtension == "bz2" ) {
        throw art::Exception(art::errors::UnimplementedFeature) << "Not currently supporting BZ2 compression\n";
        return false;
      }

      //Unknown
      else {
        throw art::Exception(art::errors::FileOpenError) << "Unknown file extension : " << fileExtension << "\n";
        return false;
      }


      //
      // Open the MIDAS file
      //

      //Plain file
      if( fileType_ == MidasFileType::MID ) {

        //Open the uncompressed binary file
        plainFileHandle_ = open(currentFileName_.c_str(), O_RDONLY);
        if( plainFileHandle_ == -1 ) {
          throw art::Exception(art::errors::FileOpenError) << "Error opening file " << filePath << " : " << strerror(errno) << "\n";
        }

      }

      //GZ file
      else if( fileType_ == MidasFileType::GZ ) {

        //Open the gz file
        gzFileHandle_ = gzopen(currentFileName_.c_str(), "rb");
        if( gzFileHandle_ == Z_NULL ) { 
          throw art::Exception(art::errors::FileOpenError) << "Error opening GZ file " << filePath << " : " << strerror(errno) << "\n";
        }

      }


      //
      // Done
      //

      mf::LogVerbatim("MidasBankInput") << "Opened MIDAS file : " << currentFileName_;

      return true;

    }//openMidasFile


    //----------------------------------------------------------------
    void MidasBankInputDetail::closeMidasFile() {

      //
      // Close a MIDAS file
      //

      //Check what file type has been opened (will be UNKNOWN if none), and close it...

      //Plain file
      if( fileType_ == MidasFileType::MID ) {
        close(plainFileHandle_);
      }
      else if( fileType_ == MidasFileType::MID ) {
        gzclose(gzFileHandle_);
      }

      //Clear file info
      fileType_ = MidasFileType::UNKNOWN;
      plainFileHandle_ = -1;
      gzFileHandle_ = Z_NULL;

    }//closeMidasFile


    //----------------------------------------------------------------
    ssize_t MidasBankInputDetail::readMidasFileBlock(void *buf, size_t size) {

      //
      // Read from a MIDAS file
      //

      //Read data block of requested size from file into provided buffer...

      ssize_t n = 0;

      //Read plain file
      if( fileType_ == MidasFileType::MID ) {

        n = read(plainFileHandle_, buf, size);

        if( n < 0 ) {
          throw art::Exception(art::errors::FileReadError) << "Error reading from file : " << strerror(errno) << "\n";
        }

      }

      //Read gz file
      else if( fileType_ == MidasFileType::GZ ) {

        n = gzread(gzFileHandle_, buf, size);

        if( n < 0 ) {
          throw art::Exception(art::errors::FileReadError) << "Error reading from file : " << strerror(errno);
        }

      }

      //Check found something
      if( n == 0 ) {
        throw art::Exception(art::errors::FileReadError) << "End of file reached before EOR found\n";
      }

      return n;

    }//readFile


    //----------------------------------------------------------------
    bool MidasBankInputDetail::readEventFromSocket(art::RunPrincipal*    const& inR,
            art::SubRunPrincipal* const& inSR,
            art::RunPrincipal*&          outR,
            art::SubRunPrincipal*&       outSR,
            art::EventPrincipal*&        outE) 
    {

      //
      // Online loop looking for transitions or events
      //

      INT status;

      //Poll until receive run transition or event from mserver
      do {

        //Yield to allow RPC comms between mserver and this analyzer for this loop iteration
        status = cm_yield(onlineLoopPeriodMs_); //Arg is [ms] to yield for, basically sleeps system

        //Check for interrupt from MIDAS (using return status of yield)
        //MIDAS will raise this interrupt if ctrl+c is received
        if( ( status == RPC_SHUTDOWN ) || ( status == SS_ABORT ) ) {
          mf::LogVerbatim("MidasBankInput") << "Stopping online loop, interrupt received";
          return false;
        }


        //
        // Check for new BOR/EOR
        //

        //Check if a transition has occurred since last loop
        int transID, transRunNum, transTime; //Variables to be filled with transition info
        status = cm_query_transition(&transID, &transRunNum, &transTime);
        if(status == CM_SUCCESS) {

          //Found a transition!

          //Handle BOR transition
          if( transID == TR_START ) {

	    //Record that we have found at least one BOR
	    firstRunStarted_ = true;

	    if (currentRunNumber_ != static_cast<int>(transRunNum)) {

	      //Get run number
	      currentRunNumber_ = transRunNum; 
	      mf::LogVerbatim("MidasBankInput") << "\nFound Begin Of Run " << currentRunNumber_;

	      //Create new run (and sub-run) principal
	      managePrincipals( currentRunNumber_, currentSubRunNumber_, 1, outR, outSR, outE );

	      //Dump the ODB to a pseudo BOR event, then parse it and fill art record
	      if( parseODB_ ) {
		auto odbSize = dumpODB();
		readODB(outR,odbData_,odbSize);
	      }

	    }
	  }

          //Handle EOR transition
	  else if(transID == TR_STOP) {

	    mf::LogVerbatim("MidasBankInput") << "Found End Of Run " << currentRunNumber_; 

	  }

        }


        //
        // Check for new event
        //

        //TODO Use bm_skip_event to give a "most recent event only" option (see midas.c for details)

        //Note that only receive data events here, not BOR/EOR etc type events as in file read (see transitions above)

        //Note that this loop with continue trying to receive events here even after run stops. This means that if we are
        //lagging behind DAQ, we carry one processing events after the run has stopped until we catch up.

        //Try to receive the requested event (non-blocking, BM_NO_WAIT), and if fill rawData_ byte array with the raw data.
        //Initially set "size" to total buffer size and pass to bm_receive_event. 
        //After call to bm_receive_event, "size" gives the event size we actually read. 
        INT size = sizeof(char) * DEFAULT_MAX_EVENT_SIZE; //Size of rawData_ buffer
        status = bm_receive_event(onlineBuffer_, rawData_, &size, BM_NO_WAIT);

        //Check if received an event
        if( status == BM_SUCCESS ) {

          //
          // Handle event before BOR case
          //

          //If this program is started whilst a MIDAS run is in progress, won't receive the BOR transition
          //In that case, will receive an event without a BOR, so need to create the run principal now using 
          //the run number we read from the ODB when we first connected
          if( ! firstRunStarted_ ) {

             //Found an event before BOR, must have started mid-way through a run
             mf::LogVerbatim("MidasBankInput") << "\nStarted mid-way through run " << currentRunNumber_;

             //Create new run (and sub-run) principal
             managePrincipals( currentRunNumber_, currentSubRunNumber_, 1, outR, outSR, outE );

             //Dump the ODB to a pseudo BOR event, then parse it and fill art record
             if( parseODB_ ) {
               auto odbSize = dumpODB();
               readODB(outR,odbData_,odbSize);
             }

             //Locked in to sequence now, should receive subsequent BOR signals
            firstRunStarted_ = true;

          }


          //
          // Check event looks OK
          //

          //TODO Have extra 4 bytes at end of block when using bm_receive_event that we ignore, why?
          size -= 4;

          mf::LogVerbatim("MidasBankInput") << "Received an event : Size = " << size;

          //Check got at least an event header
          if( size < static_cast<long>(sizeof(EVENT_HEADER)) ) {
            throw art::Exception(art::errors::DataCorruption) << "Not enough data in event (found " << size 
              << ", expected at least " << sizeof(EVENT_HEADER) << " for header)\n";
          } 

          //Swap bytes if required
          if(swapEndian_) bk_swap( pevent_, (TRUE ? forceEndianSwap_ : FALSE) ); //FIXME Currently untested


          //Check that "data size" word is sensible
          if( pevent_->data_size <= 0 ) {
            throw art::Exception(art::errors::DataCorruption) << "Nonsense data size word, data is corrupt\n";
          }

          //Check "data size" word in event matches size of block
          auto eventSize = size - sizeof(EVENT_HEADER); //Total read size includes header, so subtract this
          if( eventSize != static_cast<long>(pevent_->data_size) ) {
            throw art::Exception(art::errors::DataCorruption) << "Data size word in event (" << pevent_->data_size 
                << ",0x" << std::hex << pevent_->data_size << std::dec << ") != data size from read (" << size << ",0x" 
                << std::hex << size << std::dec << ")\n";
          }


          //
          // Process the event
          //

          //Process the event (get header and banks)
          bool foundEvent = processEvent(inR, inSR, outR, outSR, outE);

          //Move on if found an event here
          if(foundEvent) return true;

        }


        //
        // Handle errors receiving the event
        //

        //If no event, check that we received a sensible "no event" reply, or throw errors
        else if( status == BM_ASYNC_RETURN ) { } //This means no event available yet, continue loop

        else if( status == BM_TRUNCATED ) {
          throw art::Exception(art::errors::Configuration) << "Received event too large for buffer\n";
        }

        else if( status == BM_INVALID_HANDLE ) {
          throw art::Exception(art::errors::Configuration) << "Invalid buffer handle, cannot receive event\n";
        }
 
        else  {
          throw art::Exception(art::errors::Unknown) << "Invali buffer handle, cannot receive event\n";
        }


      } while(true);

      //Should never get here
      return false;

    }//readEventFromSocket


    //---------------------------------------------------------------
    bool MidasBankInputDetail::readEventFromFile(art::RunPrincipal*    const& inR,
            art::SubRunPrincipal* const& inSR,
            art::RunPrincipal*&          outR,
            art::SubRunPrincipal*&       outSR,
            art::EventPrincipal*&        outE )
     {

        //MIDAS events don't map directly to art events, so some mapping required here. 
        //The MIDAS file contains "MIDAS events" of different types (BOR, EOR, MESSAGE, DATA EVENT). 
        //An art event is a single data event. The function below handles this mapping.

        //
        // Loop through events in MIDAS file to find next data event
        //

        //Read MIDAS file events until find a data event or end of file
        bool foundEvent = false;
        while( ! foundEvent ) {

            //
            // Read and check current MIDAS event
            //

            //Read event header from MIDAS file. Could be data event or special event (e.g. BOR, EOR, log message, etc)
            ssize_t size = readMidasFileBlock(pevent_, sizeof(EVENT_HEADER));

            //Check size of read
            if(size < static_cast<long>(sizeof(EVENT_HEADER)) ) {
              throw art::Exception(art::errors::DataCorruption) << "Unexpected end of file : " << currentFileName_ << "\n";
            } 

            //Swap bytes if required
            if(swapEndian_) bk_swap( pevent_, (TRUE ? forceEndianSwap_ : FALSE) ); //FIXME Currently untested

            //Check data size in event
            if (pevent_->data_size <= 0) {
              throw art::Exception(art::errors::DataCorruption) << "Nonsense data size word, data is corrupt"
                << currentFileName_ << "\n";
            }

            //Read data area of event (step pointer by one from event header)
            //Check that size of the data block matches the "data_size" word in the event header
            size = readMidasFileBlock(pevent_ + 1, pevent_->data_size);
            if( size != static_cast<long>(pevent_->data_size) ) {
              throw art::Exception(art::errors::DataCorruption) << "Data size word in event (" << pevent_->data_size 
                << ",0x" << std::hex << pevent_->data_size << std::dec << ") != data size from read (" << size << ",0x" 
                << std::hex << size << std::dec << ") : " << currentFileName_ << "\n";
            }


            //
            // Handle BOR/EOR/MESSAGE events
            //

            //Handle "Begin Of Run" events
            if (pevent_->event_id == EVENTID_BOR) {

	      //Get run number
	      if (currentRunNumber_ != static_cast<int>(pevent_->serial_number)) {

                currentRunNumber_ = pevent_->serial_number; 

                mf::LogVerbatim("MidasBankInput") << "Found Begin Of Run " << currentRunNumber_;

                //Create new run (and sub-run) principal
                managePrincipals( currentRunNumber_, currentSubRunNumber_, 1, outR, outSR, outE );

                //Read the ODB from the BOR "event" (data area, step 1 to ge tpast header) and fill art record
                if( parseODB_ ) readODB(outR,(char*)(pevent_+1),pevent_->data_size);                

	      }
	    }

            //Handle "End Of Run" events
            else if (pevent_->event_id == EVENTID_EOR) {

                //Check EOR is for current run (otherwise something has gone wrong)
                if( currentRunNumber_ != static_cast<int>(pevent_->serial_number) ) {
                    throw art::Exception(art::errors::DataCorruption) << "EOR does not match current run : " << currentFileName_;
                }
                else {
                    mf::LogVerbatim("MidasBankInput") << "Found End Of Run " << pevent_->serial_number;
                }

                //No more data in this file, tell art that this is the end of file
                return false;

            }

            //Handle "Log Message" events
            else if (pevent_->event_id == EVENTID_MESSAGE) {

              mf::LogVerbatim("MidasBankInput") << "Found log message"; //TODO Print the message

            }


            //
            // Handle data events
            //

            else {

              //Have a data event, process it (get header and banks)
              foundEvent = processEvent(inR, inSR, outR, outSR, outE);

            }//else

        }//end while ! foundEvent

        //When here, means found data in this event
        return true;

    }//readEventFromFile


    //----------------------------------------------------------------
    //Find banks and created art records from a MIDAS event
    bool MidasBankInputDetail::processEvent(art::RunPrincipal*    const& inR,
            art::SubRunPrincipal* const& inSR,
            art::RunPrincipal*&          outR,
            art::SubRunPrincipal*&       outSR,
            art::EventPrincipal*&        outE )
     {

        //
        // Read event header
        //

        //Check that this event is header for a data event (which will be in the next data event)
        if (pevent_->event_id == 1) { //1 = "triggered event"

            //Get event number from this data event header
            currentEventNumber_ = pevent_->serial_number + 1; //Offsetting as art doesn't like event num = 0 
            mf::LogVerbatim("MidasBankInput") << "Found Event " << currentEventNumber_;

            //Create event art record
            std::unique_ptr<MidasEventHeaderArtRecord> eventRecord(new MidasEventHeaderArtRecord());

            //Add event header info to event art record
            eventRecord->eventType = pevent_->event_id;
            eventRecord->triggerMask = pevent_->trigger_mask;
            eventRecord->eventNumber = currentEventNumber_; //includes the +1 
            eventRecord->timeStamp = pevent_->time_stamp;
            eventRecord->eventDataSize = pevent_->data_size;

            //mf::LogVerbatim("MidasBankInput") << eventRecord*; //TODO Overload << in MidasEventHeaderArtRecord


            //
            // Read event data area
            //

            //Now have finished reading event header, step pointer by one to get to the event data area
            pbank_ = (BANK_HEADER*) (pevent_+1);

            //Add (global) bank header info to event art record
            eventRecord->allBankDataSize = pbank_->data_size;
            eventRecord->flags = pbank_->flags;

            //Create a new event principal
            managePrincipals( currentRunNumber_, currentSubRunNumber_, currentEventNumber_, outR, outSR, outE );


            //
            // Decode MIDAS banks
            //

            //Loop over banks
            for( auto bankName : bankNames_ ) {

                //Create and fill art record collection from MIDAS bank and add to event (handle different bank types)
                /*
                   >>> IF NEW BANK ADDED, ADD LINE HERE <<
                */
                if( TrackerBank::CompareBankName(bankName) ) { readBank<TrackerBank>(outE,bankName,eventRecord.get()); }
                else if( TrackerGLIBBank::CompareBankName(bankName) ) { readBank<TrackerGLIBBank>(outE,bankName,eventRecord.get()); }
                else if( TrackerSlowBank::CompareBankName(bankName) ) { readBank<TrackerSlowBank>(outE,bankName,eventRecord.get()); }
                else if( MasterTriggerBank::CompareBankName(bankName) ) { readBank<MasterTriggerBank>(outE,bankName,eventRecord.get()); }
                else if( GPSBank::CompareBankName(bankName) ) { readBank<GPSBank>(outE,bankName,eventRecord.get()); }
                else if( CaloHeaderBank::CompareBankName(bankName) ) { readBank<CaloHeaderBank>(outE,bankName,eventRecord.get()); }
                else if( CaloRawBank::CompareBankName(bankName) ) { readBank<CaloRawBank>(outE,bankName,eventRecord.get()); }
                else if( CaloTBank::CompareBankName(bankName) ) { readBank<CaloTBank>(outE,bankName,eventRecord.get()); }
                else if( CaloCQBank::CompareBankName(bankName) ) { readBank<CaloCQBank>(outE,bankName,eventRecord.get()); }
                else if( CaloCHBank::CompareBankName(bankName) ) { readBank<CaloCHBank>(outE,bankName,eventRecord.get()); }
                else if( CaloCPBank::CompareBankName(bankName) ) { readBank<CaloCPBank>(outE,bankName,eventRecord.get()); }
		else if( GalilBank::CompareBankName(bankName) ) { readBank<GalilBank>(outE,bankName,eventRecord.get()); }
                else { mf::LogVerbatim("MidasBankInput") << "Bank not found in event : " << bankName; }

            }//bank name loop

            //Write MIDAS event art record to art event
            art::put_product_in_principal(std::move(eventRecord), *outE, moduleLabel_, ""); //No instance name  

            //Tell higher-level functions that an event was found
            return true;      

        } //End if event_id==1

        //If here, no event found
        return false;

     }//processEvent


    //----------------------------------------------------------------
    //Function for reading an individual bank from an event
    template<typename D> //Detector bank 
        bool MidasBankInputDetail::readBank(art::EventPrincipal*& outE, const std::string& bankName, MidasEventHeaderArtRecord* eventRecord) {

            bool foundEvent = false;

            //
            // Create variables required to read bank
            //

            //Define type that MIDAS bank was written as
            DWORD bkType = 0;
            if(typeid(typename D::MidasWordType)==typeid(WORD)) bkType = TID_WORD;
            else if(typeid(typename D::MidasWordType)==typeid(DWORD)) bkType = TID_DWORD;
            else if(typeid(typename D::MidasWordType)==typeid(SHORT)) bkType = TID_SHORT;
            else throw art::Exception(art::errors::UnimplementedFeature) << "Unsupported MIDAS bank type, should be WORD or DWORD or SHORT\n";

            //Create pointer to read data with and size variable to fill
            //Note that the read type may be larger than than the MIDAS bank type (e.g. read 32-bit bank 2 words at a time into a 64-bit vector)
            DWORD bankLength = 0;
            union {
              typename D::WordType* bankPointer; //This is bank type, not MIDAS bank type (although normally the same)
              void* bankVoidPointer;
            };
            bankPointer = 0;

            //Determine value to scale bank size by if reading into large type that bank was written in
            unsigned int typeScaling = 1;
            if( sizeof(typename D::WordType) > sizeof(typename D::MidasWordType) ) {
                typeScaling = sizeof(typename D::WordType) / sizeof(typename D::MidasWordType);
            }
            else if( sizeof(typename D::WordType) < sizeof(typename D::MidasWordType) ) {
                throw art::Exception(art::errors::Configuration) << "Reading from MIDAS bank type into vector of a smaller type is not supported\n";
            }


            //
            // Read the bank
            //

            //Look for the bank
            if ( bk_find(pbank_, bankName.c_str(), &bankLength, &bkType, &bankVoidPointer) ) {

                //Create a collection to fill //TODO Should we just add single instance since shouldn't have multiple of same bank in an event?
                std::unique_ptr<std::vector<MidasEventArtRecord<D>>> bankCollection(new std::vector<MidasEventArtRecord<D>>());

                //Scale bank length in case using different types
                bankLength /= typeScaling; 

                //Fill art record with words from MIDAS bank in this event
                MidasEventArtRecord<D> bankEvent(bankPointer,bankLength);

                //Print a few
                mf::LogVerbatim("MidasBankInput") << "Found bank name \"" << bankName << "\"";
                auto numToPrint = std::min( (unsigned int)bankEvent.words.size() , maxBankWordsToPrint_ );
                for(unsigned int i_word=0 ; i_word < numToPrint ; i_word++) {
                    std::stringstream ss; ss << boost::format("  %6d : 0x%08lx") % i_word % bankEvent.words.at(i_word);
                    mf::LogVerbatim("MidasBankInput") << ss.str();
                }

                //Add bank art record to collection
                bankCollection->push_back(bankEvent);

                //Move collection to event (using bank name as instance name)
                art::put_product_in_principal(std::move(bankCollection), *outE, moduleLabel_, bankName);

                //Register that data has been found, so can exit loop and function
                foundEvent = true;

                //Add to list of banks that have been read in the event record
                eventRecord->banksList.push_back(bankName);

            }
            else {
                mf::LogVerbatim("MidasBankInput") << "No bank \"" << bankName << "\" found in event";
            }

            return foundEvent;

        }


    //----------------------------------------------------------------
    // Each time that we encounter a new run, a new subRun or a new event, we need to make a new principal
    // of the appropriate type.  This code does not need to change as the number and type of data products changes.
    void MidasBankInputDetail::managePrincipals ( int runNumber,
            int subRunNumber,
            int eventNumber,
            art::RunPrincipal*&    outR,
            art::SubRunPrincipal*& outSR,
            art::EventPrincipal*&  outE) {

        art::Timestamp ts;

        //If this is a new run, create new run principal
        art::SubRunID newID(runNumber, subRunNumber);
        if(newID.runID() != lastSubRunID_.runID()) {
            //art takes ownership of the object pointed to by outR and will delete it at the appropriate time.
            outR = pm_.makeRunPrincipal(runNumber, ts);
            mf::LogVerbatim("MidasBankInput") << "Making run principal: " << newID.runID();
        }

        //If this is a new subrun, create subrun run principal
        if(newID != lastSubRunID_) {
            // art takes ownership of the object pointed to by outSR and will delete it at the appropriate time.
            outSR = pm_.makeSubRunPrincipal(runNumber, subRunNumber, ts);
            mf::LogVerbatim("MidasBankInput") << "Making subRun principal: " << newID;
        }
        lastSubRunID_ = newID;

        //Create new event principal
        //art takes ownership of the object pointed to by outE and will delete it at the appropriate time.
        outE = pm_.makeEventPrincipal(runNumber, subRunNumber, eventNumber, ts, realData_);

    } // managePrincipals()


    //----------------------------------------------------------------
    void MidasBankInputDetail::handleMidasMessage(HNDLE hBuf, HNDLE id, EVENT_HEADER *header, void *message) {

      //Log the incoming message
      mf::LogVerbatim("MidasBankInput") << "MIDAS system message -> " << static_cast<char*>(message);

      //TODO in future could buffer messages to make them available for GUIs, etc

    }//handleMidasMessage


    //----------------------------------------------------------------
    //Dump the ODB to a local buffer
    INT MidasBankInputDetail::dumpODB() {

      //Dump the ODB into a buffer
      //Size is total buffer size when passed as input, and is returned as remaining buffer space after ODB write
      INT bufferSize = sizeof(char) * DEFAULT_MAX_EVENT_SIZE; //Size of odbData__ buffer
      INT size = bufferSize;
      INT status = db_copy_xml(odbHandle_, 0, odbData_, &size);
      INT remainingBufferSize = size;

      //Check dump was successful
      if( status == DB_SUCCESS ) {
        mf::LogVerbatim("MidasBankInput") << "ODB dumped";
      }
      else if( status == DB_TRUNCATED ) {
        //This error means that the buffer (rawData_ here) was too small to receive the ODB
        //Could dynamically resize buffer in this case as mlogger does, but our buffer should be
        //large enough here (DEFAULT_MAX_EVENT_SIZE) and in any case if the buffer used here was 
        //too small we'd be truncating the ODB dump when we read it from the file in offline mode,
        //so we'd have to take action in any case.
        throw art::Exception(art::errors::Configuration) << "Buffer too small for ODB dump";
      }
      else if( status == DB_NO_MEMORY ) {
        throw art::Exception(art::errors::BadAlloc) << "Not enough memory for ODB dump buffer";
      }
      else {
        throw art::Exception(art::errors::Unknown) << "ODB dump failed with unknown error";
      }

      //Return the size of the ODB
      INT odbSize = bufferSize - remainingBufferSize;
      return odbSize;

    }//dumpODB


    //----------------------------------------------------------------
    //Read the ODB from a buffer and fill an art record
    void MidasBankInputDetail::readODB(art::RunPrincipal*& outR, char* odbBuffer, INT odbSize) {

      //Note: Must be called AFTER the run principal has been created

      //Create MIDAS ODB art record
      std::unique_ptr<MidasODBArtRecord> odbRecord(new MidasODBArtRecord());

      //Create a string from the ODB char buffer
      std::string tmp_string(odbBuffer, odbBuffer + odbSize);
 
      //Fill ODB art record
      odbRecord->runNum = currentRunNumber_;
      odbRecord->odb_string = tmp_string;

      //mf::LogVerbatim("MidasBankInput") << odbRecord->odb_string;

      //Write MIDAS ODB art record to art event
      art::put_product_in_principal(std::move(odbRecord), *outR, moduleLabel_, "MidasODB"); 

      mf::LogVerbatim("MidasBankInput") << "ODB parsed";

    }//readODB


} // namespace gm2midastoart


//Create source from detail class
typedef art::Source<gm2midastoart::MidasBankInputDetail> MidasBankInput;
DEFINE_ART_INPUT_SOURCE(MidasBankInput)

