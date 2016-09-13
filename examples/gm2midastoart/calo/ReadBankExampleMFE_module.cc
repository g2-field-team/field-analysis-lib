//Example of how to open a MIDAS bank art record

//Art components
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "art/Utilities/InputTag.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"


// quiet gcc flags TODO: removed eventually                                    
                                                                                
//#pragma GCC diagnostic ignored "-Wunused-but-set-variable"                     
//#pragma GCC diagnostic ignored "-Wunused-parameter"                            
//#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"





//Art records
#include "gm2midastoart/sources/MidasEventArtRecord.hh"

#include "gm2util/common/RootManager.hh"
//C/C++ includes                                                                                
#include <memory>
#include "TH2.h"
#include "TH1.h"
#include "TTree.h"
#include "TFile.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <netdb.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <poll.h>

//C/C++ includes
#include <boost/format.hpp>

//! namespace
namespace gm2calo {

  class ReadBankExampleMFE : public art::EDAnalyzer {

    public:

      explicit ReadBankExampleMFE(fhicl::ParameterSet const & p);
      virtual ~ReadBankExampleMFE();

    void analyze(const art::Event& event ) override;
    void beginJob() override;
    void endJob() override;
    private:

    //!root plotting members                                                                         
    std::unique_ptr<RootManager> rootManager_;
    std::string outputRootFileName_;
    std::unique_ptr<TFile> outputRootFile_;


    //FCL params
    std::string inputModuleLabel_;
    std::string inputInstanceName_;

    //book histogram directory                                                                       
    std::string dirName_;


  };


  double calc_dt( UInt_t t1_s, UInt_t t1_us, UInt_t t2_s, UInt_t t2_us){

    long int dt_s = t1_s;

    dt_s -= t2_s;

    long int dt_us = t1_us;

    dt_us -= t2_us;

    if ( dt_us < 0 )

      {

        dt_s -= 1;

	dt_us += 1000000;

      }

    double t = 1.e6*(double)dt_s + (double)dt_us;

    return t;
  }




  //! standard constructor
  ReadBankExampleMFE::ReadBankExampleMFE(fhicl::ParameterSet const & pset)
   : art::EDAnalyzer(pset)
   , inputModuleLabel_( pset.get<std::string>("inputModuleLabel","MidasBankInput") )
  {}

  //! standard destructor
  ReadBankExampleMFE::~ReadBankExampleMFE() {}

  //! analyzes data
  void ReadBankExampleMFE::analyze(const art::Event& event)
  {





    std::vector<uint32_t>  datablock;
    double t[8];



    //! get histogram directory                                                              

    auto dir = rootManager_->GetDir(dirName_);


    unsigned int caloNum = 2;


    for(unsigned int id=1; id<=caloNum; id++)
    

    {
     //Get the CaloHeader MIDAS bank art record
      art::Handle< std::vector< gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloHeaderBank> > > bankHandle;
      event.getByLabel(inputModuleLabel_, gm2midastoart::CaloHeaderBank::GetBankName(id), bankHandle);

    


      auto bankCollection = *bankHandle;

     //Check there is only one in collection (only expect one per event from trigger)
      if( bankCollection.size() != 1 ) { 
	throw cet::exception("ReadBankExampleMFE") << "Expected exactly one trigger bank in event, found " << bankCollection.size();
        return;
      }

     //Print bank values
     //  mf::LogVerbatim("ReadBankExample") << "  Trigger bank :";
     // auto bank = bankCollection.front();
     //for(auto word : bank.words) {
     //std::stringstream ss; ss << boost::format("    0x%08lx") % word;
     //mf::LogVerbatim("ReadBankExample") << ss.str();
     // }

     //     mf::LogVerbatim("ReadBankExample"); //New line

      


  //Print bank values                                                                             
      //  mf::LogVerbatim("ReadBankExample") << "  caloHeader bank :";
      auto bank = bankCollection.front();
      for(auto word : bank.words) {
	std::stringstream ss; ss << boost::format("    0x%08lx") % word;
	mf::LogVerbatim("ReadBankExampleMFE") << ss.str();
	datablock.push_back(word);
      }

      mf::LogVerbatim("ReadBankExampleMFE"); //New line                                                  
    


      for(auto word : bank.words) {
	std::stringstream ss; ss << boost::format("    0x%08lx") % word;
	datablock.clear();
      }


  
      int cdfheader1 = datablock[0];

      mf::LogVerbatim("ReadBankExampleMFE") <<"id: "<<id;

      mf::LogVerbatim("ReadBankExampleMFE") <<"cdfheader1:   0x" << std::hex << cdfheader1;
       
      int cdfheader2 = datablock[1];

      mf::LogVerbatim("ReadBankExampleMFE") <<"cdfheader2:   0x" << std::hex << cdfheader2;

      int time_tcp_listen_s   = datablock[2];
      
      mf::LogVerbatim("ReadBankExampleMFE") <<"time_tcp_listen_s:   0x" << std::hex << time_tcp_listen_s;


      int time_tcp_listen_us   = datablock[4];
      
      mf::LogVerbatim("ReadBankExampleMFE") <<"time_tcp_listen_us:   0x" << std::hex << time_tcp_listen_us;

       int time_tcp_got_header_s   = datablock[6];

       mf::LogVerbatim("ReadBankExampleMFE") <<"time_tcp_got_header_s:   0x" << std::hex << time_tcp_got_header_s;


       int time_tcp_got_header_us   = datablock[8];
       
       mf::LogVerbatim("ReadBankExampleMFE") <<"time_tcp_got_header_us:   0x" << std::hex << time_tcp_got_header_us;


       int time_tcp_got_data_s  = datablock[10];
       
       mf::LogVerbatim("ReadBankExampleMFE") <<"time_tcp_got_data_s:   0x" << std::hex << time_tcp_got_data_s;

       int time_tcp_got_data_us  = datablock[12];
       
       mf::LogVerbatim("ReadBankExampleMFE") <<"time_tcp_got_data_us:   0x" << std::hex << time_tcp_got_data_us;

       int time_gpu_start_s  = datablock[14];

       mf::LogVerbatim("ReadBankExampleMFE") <<"time_gpu_start_s:   0x" << std::hex << time_gpu_start_s;


       int time_gpu_start_us  = datablock[16];

       mf::LogVerbatim("ReadBankExampleMFE") <<"time_gpu_start_us:   0x" << std::hex << time_gpu_start_us;

       int time_gpu_copy_done_s  = datablock[18];

       mf::LogVerbatim("ReadBankExampleMFE") <<"time_gpu_copy_done_s:   0x" << std::hex << time_gpu_copy_done_s;

       int time_gpu_copy_done_us  = datablock[20];

       mf::LogVerbatim("ReadBankExampleMFE") <<"time_gpu_copy_done_us:   0x" << std::hex << time_gpu_copy_done_us;


       int time_gpu_proc_done_s  = datablock[22];

       mf::LogVerbatim("ReadBankExampleMFE") <<"time_gpu_proc_done_s:   0x" << std::hex << time_gpu_proc_done_s;


       int time_gpu_proc_done_us  = datablock[24];

       mf::LogVerbatim("ReadBankExampleMFE") <<"time_gpu_proc_done_us:   0x" << std::hex << time_gpu_proc_done_us;

       int time_mfe_start_s  = datablock[26];

       mf::LogVerbatim("ReadBankExampleMFE") <<"time_mfe_start_s:   0x" << std::hex << time_mfe_start_s;


       int time_mfe_start_us  = datablock[28];

       mf::LogVerbatim("ReadBankExampleMFE") <<"time_mfe_start_us:   0x" << std::hex << time_mfe_start_us;


       int time_mfe_sent_data_s  = datablock[30];

       mf::LogVerbatim("ReadBankExampleMFE") <<"time_mfe_sent_data_s:   0x" << std::hex << time_mfe_sent_data_s;


       int time_mfe_sent_data_us  = datablock[32];

       mf::LogVerbatim("ReadBankExampleMFE") <<"time_mfe_sent_data_us:   0x" << std::hex << time_mfe_sent_data_us;

       int tcpeventnumber = datablock[34];

       mf::LogVerbatim("ReadBankExampleMFE") <<"tcpeventnumber:   0x" << std::hex << tcpeventnumber;

       int gpueventnumber = datablock[36];

       mf::LogVerbatim("ReadBankExampleMFE") <<"gpueventnumber:   0x" << std::hex << gpueventnumber;

       memset( t, 0, sizeof(t));

       // EOF timing - redundant                                                                                                                                          

       t[1] = t[2] = 0;

       // TCP timing                                                                                                                                                      

       t[3] = calc_dt( time_tcp_got_data_s, time_tcp_got_data_us,

		       time_tcp_got_header_s, time_tcp_got_header_us );

       // GPU timing                                                                                                                                                      

       t[4] = calc_dt( time_gpu_copy_done_s, time_gpu_copy_done_us,

		       time_tcp_got_header_s, time_tcp_got_header_us );

       t[5] = calc_dt( time_gpu_proc_done_s, time_gpu_proc_done_us,

		       time_tcp_got_header_s, time_tcp_got_header_us );

       // MFE timing                                                                                                                                                      

       t[6] = calc_dt( time_mfe_start_s, time_mfe_start_us,

		       time_tcp_got_header_s, time_tcp_got_header_us );

       t[7] = calc_dt( time_mfe_sent_data_s, time_mfe_sent_data_us,

		       time_tcp_got_header_s, time_tcp_got_header_us );



       for (int j=1; j <=7; j++){

	 //printf("j = %d, t[j]=%f\n", j, t[j]);


	 mf::LogVerbatim("ReadBankExampleMFE") <<"t[j]" << t[j] << "j" << j << "id" << id;

	 std::stringstream hName; hName << "h2_dt_tcpgotheader" << id;


	 rootManager_->Get<TH2F*>(dir,hName.str())->Fill(t[j],j);



       }

    }

  }





}//namespace

// These are some necessary boilerplate for the ROOT persistency system
using gm2calo::ReadBankExampleMFE;

DEFINE_ART_MODULE(ReadBankExampleMFE)

void gm2calo::ReadBankExampleMFE::beginJob()
{

  // create a root file and manager                                                                                                                                    
  outputRootFileName_ = "ReadBankExampleMFE.root";

  outputRootFile_.reset( new TFile(outputRootFileName_.c_str(),"recreate"));


  rootManager_.reset( new RootManager("ReadBankExampleMFE",outputRootFile_.get()));

  //create directories                                                                                                                           
  auto dir = rootManager_->GetDir(dirName_,true);


  for(int id=1; id<=2; id++)
    {

      std::stringstream nameStream; nameStream << "h2_dt_tcpgotheader" << id;

      //book 2D histogram                                                                                                                                    
      rootManager_->Add(dir, new TH2F(nameStream.str().c_str(),"time relative to tcp got header; time; Entries", 300000, -1000000, 5000000, 10, 0, 10));


    

    }
}
void gm2calo::ReadBankExampleMFE::endJob()
{
  rootManager_->WriteToFile();
  outputRootFile_->Close();
  }





