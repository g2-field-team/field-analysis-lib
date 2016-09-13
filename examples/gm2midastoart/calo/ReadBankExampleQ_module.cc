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



/**                                                                                                                               
 * Number of AMC13 shelves in the experiment                                                                                      
 */
#define AMC13_NUMBER_OF_SHELVES  2

/**                                                                                                                               
 * Size of data from AMC13                                                                                                        
 */

#define AMC13_DATA_SIZE 32441332 

/**                                                                                                                               
 * Number of segments per calorimeter                                                                                             
 */

#define NUMBER_OF_SEGMENTS   55


/**                                                                                                                                                                                                                                        
 * Waveform size 

 */

#define N_SAMPLES 560000 

//! namespace
namespace gm2calo {

class ReadBankExampleQ : public art::EDAnalyzer {

    public:

      explicit ReadBankExampleQ(fhicl::ParameterSet const & p);
      virtual ~ReadBankExampleQ();


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

  //! standard constructor
  ReadBankExampleQ::ReadBankExampleQ(fhicl::ParameterSet const & pset)
     : art::EDAnalyzer(pset)
     , inputModuleLabel_( pset.get<std::string>("inputModuleLabel","MidasBankInput") )     
  {
   

  }

    
  //! standard destructor
  ReadBankExampleQ::~ReadBankExampleQ() {}

  //! analyzes data
  void ReadBankExampleQ::analyze(const art::Event& event)
  {
   
     std::vector<uint32_t>  datablock;
   
     //! get histogram directory

     auto dir = rootManager_->GetDir(dirName_);

     for(unsigned int ishelf=1; ishelf<AMC13_NUMBER_OF_SHELVES; ishelf++)
       {

     //Get the HC01 MIDAS bank art record
     art::Handle< std::vector< gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloQBank> > > bankHandle;
     event.getByLabel(inputModuleLabel_,gm2midastoart::CaloQBank::GetBankName(ishelf), bankHandle);
     auto bankCollection = *bankHandle;

     //Check there is only one in collection (only expect one per event from trigger)
     if( bankCollection.size() != 1 ) { 
       throw cet::exception("ReadBankExampleQ") << "Expected exactly one trigger bank in event, found " << bankCollection.size();
       return;
     }
     

     //Print bank values
     //mf::LogVerbatim("ReadBankExampleQ") << "  HC01 bank :";
     auto bank = bankCollection.front();
     for(auto word : bank.words)
    {
       
      datablock.push_back(word);
      //      mf::LogVerbatim("ReadBankExampleQ") << "0x" << std::hex  << word;

     }


 unsigned int iw = 0; //data counter                                                                                    
 while(iw < datablock.size())
   {
                                                                                   

     unsigned int iseg = iw/N_SAMPLES;
     unsigned int ibin = iw - iseg*N_SAMPLES;

     if(ibin<N_SAMPLES && iseg< NUMBER_OF_SEGMENTS && ibin>=0 && iseg>=0)
    
    {
      
    
       std::stringstream hName; hName << "h_event" << "_" << ishelf << "_" << iseg ;

       rootManager_->Get<TH1F*>(dir,hName.str())->SetBinContent(ibin+1,datablock[iw]);


     }
  

     else{
       printf("***ERROR: Iw = %i, iseg = %i, ibin = %i",iw,iseg,ibin);
       break;
     }

     iw++;



   }//while loop

 

 if(datablock.size() == 32441332){

 for(int i=0; i<NUMBER_OF_SEGMENTS; i++)
 { //array size of the histogram                                            
  
   std::stringstream hName1; hName1 << "h_Qmethod" << "_" << ishelf << "_" << i;

   std::stringstream hName2; hName2 << "h_event" << "_" << ishelf << "_" << i;


   rootManager_->Get<TH1F*>(dir,hName1.str().c_str())->Add(rootManager_->Get<TH1F*>(dir,hName2.str().c_str()));

  
 } //loop over number of segments


 }//if datablock size

  
 }//ishelf 

}//analyze


// These are some necessary boilerplate for the ROOT persistency system
using gm2calo::ReadBankExampleQ;
DEFINE_ART_MODULE(ReadBankExampleQ)

void gm2calo::ReadBankExampleQ::beginJob()
{

  // create a root file and manager
  outputRootFileName_ = "ReadBankExampleQ.root";

  outputRootFile_.reset( new TFile(outputRootFileName_.c_str(),"recreate"));

  rootManager_.reset( new RootManager("ReadBankExampleQ",outputRootFile_.get()));

  std::string name = " ";

  //create directories
			 
   auto dir = rootManager_->GetDir(dirName_,true);

  // book h_event histogram

   for(int ishelf=1;ishelf<AMC13_NUMBER_OF_SHELVES;ishelf++)
      for(int iSeg=0;iSeg<NUMBER_OF_SEGMENTS;iSeg++)
	{

	std::stringstream nameStream; nameStream << "h_event" << "_" << ishelf << "_" << iSeg ;
	rootManager_->Add(dir, new TH1F(nameStream.str().c_str(),"h_event", N_SAMPLES,0,N_SAMPLES));
		
	std::stringstream nameStream1; nameStream1 << "h_Qmethod" << "_" << ishelf << "_" << iSeg ;
	rootManager_->Add(dir, new TH1F(nameStream1.str().c_str(),"h_Qmethod",N_SAMPLES,0,N_SAMPLES));
                                                   
	 }

					       
}
			 


void gm2calo::ReadBankExampleQ::endJob()
{
			 
  rootManager_->WriteToFile();
  outputRootFile_->Close();
			 
}


}// End of namespace gm2calo
