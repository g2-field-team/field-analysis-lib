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
#define AMC13_NUMBER_OF_SHELVES    1

/**                                                                                                                               
 * Number of AMC13 boards per shelf                                                                                               
 */
#define AMC13_NUMBER_OF_AMCS_PER_SHELF  11

/**                                                                                                                               
 * Number of channels in AMC13 board                                                                                              
 */

#define AMC13_NUMBER_OF_CHANNELS   5
/**                                                                                                                               
 * Size of data from AMC13                                                                                                        
 */
#define AMC13_DATA_SIZE 32440320 


//! namespace
namespace gm2calo {

  class ReadBankExampleraw : public art::EDAnalyzer {

  public:

    explicit ReadBankExampleraw(fhicl::ParameterSet const & p);
    virtual ~ReadBankExampleraw();


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
    std::string dirName_;
    

  };



  //! standard constructor
  ReadBankExampleraw::ReadBankExampleraw(fhicl::ParameterSet const & pset)
     : art::EDAnalyzer(pset)
     , inputModuleLabel_( pset.get<std::string>("inputModuleLabel","MidasBankInput") )
  {
   

  }





  //! standard destructor
  ReadBankExampleraw::~ReadBankExampleraw() {}

  //! analyzes data
  void ReadBankExampleraw::analyze(const art::Event& event)
  {

    
    std::vector<int16_t>  datablock;
   
    //! get histogram directory

    auto dir = rootManager_->GetDir(dirName_);

    unsigned int caloNum = 2;

    for(unsigned int ishelf=1; ishelf<caloNum; ishelf++)


      {

	//Get the CaloHeader MIDAS bank art record                                                                                                                         
	art::Handle< std::vector< gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloRawBank> > > bankHandle;
	event.getByLabel(inputModuleLabel_, gm2midastoart::CaloRawBank::GetBankName(ishelf), bankHandle);
	auto bankCollection = *bankHandle;

  
    //Check there is only one in collection (only expect one per event from trigger)
    if( bankCollection.size() != 1 ) { 
      throw cet::exception("ReadBankExampleraw") << "Expected exactly one trigger bank in event, found " << bankCollection.size();
      return;
    }

     
    //Print bank values

      mf::LogVerbatim("ReadBankExampleraw") << "  RC01 bank :";
      auto bank = bankCollection.front();
      for(auto word : bank.words) {
	datablock.push_back(word);
      }
    


    int16_t adc;
  
    unsigned int n_samples = datablock.size()/55;

   
      int l = 0;

      for (unsigned int iamc=0; iamc< AMC13_NUMBER_OF_AMCS_PER_SHELF; iamc++){
	l++;
	int t = 0;

	for ( unsigned int ichannel=0; ichannel<AMC13_NUMBER_OF_CHANNELS; ichannel++)

	  {
                              

	    int m = 0;

	    t++;
                                 
			       
	    for (unsigned int k=(ichannel*560000)+16*t; k<((ichannel+1)*n_samples)+16*t; k++)
	      {
                                                        
		if(iamc==0)
		  {
		    while(k<((ichannel+1)*560000)+16*t){
		      adc=datablock[(iamc*5*n_samples+k)];

		      std::stringstream hName; hName << "h_wf" << ishelf << "_" << iamc << "_" << ichannel;

		      rootManager_->Get<TH1F*>(dir,hName.str())->SetBinContent(m+1,adc);

		      m++;

		      k++;

		    }
		  }
          

		if(iamc>0 && iamc<=10)
		  {


		    while(k<((ichannel+1)*560000)+16*t-4*iamc){

		      adc = datablock[(iamc*5*n_samples+k+6*iamc)];
						 
		      std::stringstream hName; hName << "h_wf" << ishelf << "_" << iamc << "_" << ichannel;

		      rootManager_->Get<TH1F*>(dir,hName.str())->SetBinContent(m+1,adc);


		      m++;

		     
		      k++;
		    
		    }
		  }

	      }// k looop
  
	  }//ichannel
      
      }//iamc
    
   
      }//ishelf:CaloNum


  }//analyze
   

// These are some necessary boilerplate for the ROOT persistency system
using gm2calo::ReadBankExampleraw;
DEFINE_ART_MODULE(ReadBankExampleraw)

void gm2calo::ReadBankExampleraw::beginJob()
{

  // create a root file and manager
  outputRootFileName_ = "ReadBankExampleraw.root";

  outputRootFile_.reset( new TFile(outputRootFileName_.c_str(),"recreate"));

  rootManager_.reset( new RootManager("ReadBankExampleraw",outputRootFile_.get()));

  std::string name = " ";


  //create directories
  auto dir = rootManager_->GetDir(dirName_,true);

  unsigned int caloNum = 2;


  for (unsigned int ishelf = 1; ishelf<caloNum; ishelf++) {
    
    for (unsigned int iamc=0; iamc< AMC13_NUMBER_OF_AMCS_PER_SHELF; iamc++){

      for (unsigned int ichannel=0; ichannel<AMC13_NUMBER_OF_CHANNELS; ichannel++)

	{


	  std::stringstream nameStream; nameStream << "h_wf" << ishelf << "_" << iamc << "_" << ichannel;

	  //book 1D histogram
	  rootManager_->Add(dir, new TH1F(nameStream.str().c_str(),"h_wf", 1.1 * AMC13_DATA_SIZE / AMC13_NUMBER_OF_AMCS_PER_SHELF / AMC13_NUMBER_OF_CHANNELS, 0, 1.1 * AMC13_DATA_SIZE / AMC13_NUMBER_OF_AMCS_PER_SHELF / AMC13_NUMBER_OF_CHANNELS)); // AMC13_DATA_SIZE is size  of data in 16-bit words from all digitizers        

	}
    }//amc
  }//ishelf:calorimeter
}
			 
  void gm2calo::ReadBankExampleraw::endJob()
  {
    rootManager_->WriteToFile();
    outputRootFile_->Close();
  }

}// End of namespace gm2calo
