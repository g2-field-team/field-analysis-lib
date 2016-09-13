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
#include "gm2midastoart/calo/calorimeter.h"
#include "gm2midastoart/calo/calorimeter_defna.h"
#include "gm2midastoart/calo/defna.h"

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


#include <boost/format.hpp>




/*-- Parameters ----------------------------------------------------*/


// vector of Defna parameters parametrizing wavefroms                                                              
                                                                                                         
std::vector<CALO_WAVEFORM>calo_waveforms[CALO_N_STATIONS+1][CALO_N_SEGMENTS+1];

std::vector<DEFNA>calo_defna[CALO_N_STATIONS+1][CALO_N_SEGMENTS+1];

// dummy defna structure                                                                                              
static DEFNA defna_dummy;


//! namespace

namespace gm2calo {

  class ReadBankExampleT : public art::EDAnalyzer {

  public:

    explicit ReadBankExampleT(fhicl::ParameterSet const & p);

    virtual ~ReadBankExampleT();

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

  ReadBankExampleT::ReadBankExampleT(fhicl::ParameterSet const & pset)

    : art::EDAnalyzer(pset)

    , inputModuleLabel_( pset.get<std::string>("inputModuleLabel","MidasBankInput") )

  {}

  //! standard destructor

  ReadBankExampleT::~ReadBankExampleT() {}


  //! analyzes data

  void ReadBankExampleT::analyze(const art::Event& event)

  {

    //initialize dummy variable                                                                                     
    defna_dummy.time = 0;
    defna_dummy.pedestal = 0;
    defna_dummy.amplitude = 0;
    defna_dummy.area = 0;
    defna_dummy.width = 0;

    std::vector<int16_t>  datablock;

    unsigned int caloNum = 2;

    //! get histogram directory                                                                                     
    auto dir = rootManager_->GetDir(dirName_);




    for(unsigned int icalo=1; icalo<caloNum; icalo++)


      {


    //Get the CaloT MIDAS bank art record

    art::Handle< std::vector< gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloTBank> > > bankHandle;

    event.getByLabel(inputModuleLabel_, gm2midastoart::CaloTBank::GetBankName(icalo), bankHandle);

    auto bankCollection = *bankHandle;

    //Check there is only one in collection (only expect one per event from trigger)

    if( bankCollection.size() != 1 ) { 

      throw cet::exception("ReadBankExampleT") << "Expected exactly one CT bank in event, found " << bankCollection.size();

      return;

    }


    int16_t foo = 0;
    int16_t *pdata = &foo;


    //Print bank values

    // mf::LogVerbatim("ReadBankExampleT") << "  FC bank :";

    auto bank = bankCollection.front();

    for(auto word : bank.words) {

      datablock.push_back(word);
      pdata = & datablock[0];

      //      std::stringstream ss; ss << boost::format(" ") % word;

      //mf::LogVerbatim("ReadBankExampleT") << ss.str();

    }

    mf::LogVerbatim("ReadBankExampleT"); //New line


    for(auto word : bank.words) {
      std::stringstream ss; ss << boost::format("    0x%08lx") % word;
      datablock.clear();
    }




    int16_t data_size   = *pdata++;
    int16_t next1 = *pdata++;
    mf::LogVerbatim("ReadBankExampleT") << "next1" << next1;


    int16_t n_islands   = *pdata++;
    mf::LogVerbatim("ReadBankExampleT") << "data_size:" << data_size;
    mf::LogVerbatim("ReadBankExampleT") << "n_islands:" << n_islands; 

    int16_t next2 = *pdata++;
    mf::LogVerbatim("ReadBankExampleT") << "next2" << next2;


    int16_t next3 =  *pdata++;
    mf::LogVerbatim("ReadBankExampleT") << "next3:" << next3;

    int16_t next4 = *pdata++;
    mf::LogVerbatim("ReadBankExampleT") << "next4:" << next4;


    //record waveforms                                                                                             
    for (int16_t i_island=0; i_island<n_islands; i_island++)
      {

	int16_t island_time = *pdata++;
	int16_t next5 = *pdata++;

	mf::LogVerbatim("ReadBankExampleT") << "island_time:" << island_time;
	mf::LogVerbatim("ReadBankExampleT") << "next5" << next5;

	int16_t island_len  = *pdata++;
	int16_t next6 = *pdata++;

	//mf::LogVerbatim("ReadBankExampleT") << "island_length:" << island_len;                                   
	mf::LogVerbatim("ReadBankExampleT") << "next6" << next6;

	int16_t sum = 0;
	for (int16_t i_segment=0; i_segment<CALO_N_SEGMENTS; i_segment++)
	  {
	    CALO_WAVEFORM wf;
	    wf.time = island_time;
	    for (int16_t k=0; k<(island_len); k++)
	      {

		CALO_ADC_TYPE adc = *pdata++;
		wf.adc.push_back(adc);
		sum = sum+(30-adc);

	      }//island length                                                                                     

	    calo_waveforms[icalo][i_segment].push_back(wf);

	  
	  }//segments                      
	std::stringstream hName; hName << "h_Esum" << icalo;
	rootManager_->Get<TH1F*>(dir, hName.str())->Fill(sum);
	sum = 0;

      }//i_island                   

    for ( int16_t isegment=0; isegment<CALO_N_SEGMENTS; isegment++)
      {
	// clear old data                                                                                          
	(calo_defna[icalo][isegment]).clear();                                                                    


	std::vector<CALO_WAVEFORM> &waveforms = calo_waveforms[icalo][isegment];

	for ( unsigned int i=0; i<waveforms.size(); i++)
	  {
	    CALO_WAVEFORM &wf = waveforms[i];
	    int16_t n_samples = wf.adc.size();

	    // set dummy parameters for the pulse                                                                  
	    (calo_defna[icalo][isegment]).push_back(defna_dummy);
	    DEFNA &defna = (calo_defna[icalo][isegment])[i];

	    // pedestal: average over first several samples                                                        
	    int16_t ped = 0;

	    int16_t ped_n = 4; // number of samples for averaging                                                 
                                                                                                                    

	    for ( int16_t k=0; k<ped_n; k++)
	      {
		ped = ped+wf.adc[k];

	      }

	    if ( ped_n > 0 ) ped /= ped_n;

	    defna.pedestal = ped;

	    // histogram all pedestals, even w/o defna pulses                                                     
       	    std::stringstream hName1; hName1 << "h_pedestal" << icalo << "_" << isegment;

	    rootManager_->Get<TH1F*>(dir, hName1.str())->Fill( ped ); // Pedestals (All pedestals, even for blocks w/o defna pulses)                                                                                                  

	  int16_t k_le = -1; // pulse start sample (leading edge)                                               
                                                                                                                    
	  int16_t k_te = -1; // pulse stop  sample (trailing edge)                                              
                                                                                                                    
	  
	  // find leading and trailing edges; amplitude                                                         
                                                                                                                    
	  int16_t amplitude = 0;

	  for (int16_t k=0; k<n_samples; k++)
	    {
	      int16_t amp = ped - wf.adc[k]; // our pulses are negative                                         
                                                                                                                    

	      if ( amp > amplitude ) amplitude = amp;
	      if ( amp >=30 &&  k_le == -1 )

		{

		  k_le = k;

		}

	      else if ( k_le >= 0 )
		{

		  if ( amp < 30 )
		    k_te = k;
		}

	    }//k; n_sample loop                                                                                  
	  
	  if(amplitude>=30)
	    defna.amplitude = amplitude;


	  // histogram all amplitudes even w/o pulses                                                           
                                                                                                                    
	  if(amplitude>=30)
	    {

	      std::stringstream hName2; hName2 << "h_amplitude" << icalo << "_" << isegment;

	      rootManager_->Get<TH1F*>(dir, hName2.str())->Fill( amplitude ); // Amplitude = ped - ADC( all amplitudes, even for blocks w/o defna pulses)                                                                               

	    }

	  
	  // calculate defna parameters only if both                                                              
                                                                                                                    
	  // leading and trailing edges are found                                                                 
                                                                                                                    
	  if ( k_le < 0 ) continue;

	  if ( k_te < 0 ) continue;


	  defna.width = k_te - k_le;

	  int16_t k_start = k_le;

	  if ( k_start < 0 ) k_start = 0;

	  int16_t k_stop = k_te + 1;

	  if ( k_stop > n_samples ) k_stop = n_samples;


	  int16_t t = 0;

	  int16_t area = 0;

	  for (int k=k_start; k<k_stop; k++)
	    {
	      int16_t ampl = ped - wf.adc[k]; // our pulses are negative                                        
                                                                                                                    

	      if ( ampl < 0 ) continue; // exclude negative samples                                             
                                                                                                                    

	      t += ampl*k;

	      if(ampl>=30)

		{
		  area += ampl;
		}

	    }
	  

	  if ( area != 0 )
	    t /= area;
	  defna.time = t + wf.time;
	  defna.area = area;

	  // fill histograms                                                                
	  
	  std::stringstream hName3; hName3 << "h_area" << icalo << "_" << isegment;

	  rootManager_->Get<TH1F*>(dir, hName3.str())->Fill( area );//Pulse area                                
                                                                                                                    

	  std::stringstream hName4; hName4 << "h_time" << icalo << "_" << isegment;

	  rootManager_->Get<TH1F*>(dir, hName4.str())->Fill( wf.time );// island  time                          
                                                                                                                    
	  
	  std::stringstream hName5; hName5 << "h_abs_time" << icalo << "_" << isegment;

	  rootManager_->Get<TH1F*>(dir, hName5.str())->Fill( defna.time );// absolute time                       
	  
	  }//waveforms
 

           
      }//segments

      }//id:caloNUM                                                                                             


  }// segments                                                                          

  

}//namespace



using gm2calo::ReadBankExampleT;
DEFINE_ART_MODULE(ReadBankExampleT)

void gm2calo::ReadBankExampleT::beginJob()
{

  // create a root file and manager                                                                                 
  outputRootFileName_ = "ReadBankExampleT.root";

  outputRootFile_.reset( new TFile(outputRootFileName_.c_str(),"recreate"));

  rootManager_.reset( new RootManager("ReadBankExampleT",outputRootFile_.get()));

  std::string name = " ";
  //create directories                                                                                                
  auto dir = rootManager_->GetDir(dirName_,true);

  unsigned int caloNum = 2;

  for (unsigned int icalo=1; icalo<caloNum; icalo++)
    {

      std::stringstream nameStream; nameStream << "h_Esum" << icalo;

      rootManager_->Add(dir, new TH1F(nameStream.str().c_str(),"h_Esum",300, 0.0, 300000)) ;                        \
                                                                                                                   \
      for (unsigned int isegment=0; isegment<CALO_N_SEGMENTS; isegment++)
	{

	  std::stringstream nameStream1; nameStream1 << "h_pedestal" << icalo << "_" << isegment;

	  rootManager_->Add(dir, new TH1F(nameStream1.str().c_str(),"h_pedestal",4096, -0.5, 4095.5)) ;


	  std::stringstream nameStream2; nameStream2 << "h_amplitude" << icalo << "_" << isegment;

	  rootManager_->Add(dir, new TH1F(nameStream2.str().c_str(),"h_amplitude",4096, -0.5, 4095.5)) ;


	  std::stringstream nameStream3; nameStream3 << "h_area" << icalo << "_" << isegment;

	  rootManager_->Add(dir, new TH1F(nameStream3.str().c_str(),"h_area",4096, -0.5, 4095.5)) ;





	  std::stringstream nameStream4; nameStream4 << "h_time" << icalo << "_" << isegment;

	  rootManager_->Add(dir, new TH1F(nameStream4.str().c_str(),"h_time",10000, 0.0, 350000)) ;


	  std::stringstream nameStream5; nameStream5 << "h_abs_time" << icalo << "_" << isegment;

	  rootManager_->Add(dir, new TH1F(nameStream5.str().c_str(),"h_abs_time",10000, 0.0, 350000)) ;

	}//loop over calorimeters                                                                                    
                                                                                                                   \

    }//loop over calorimeters                                                                                         


}//End of beginJob                                                                                           






void gm2calo::ReadBankExampleT::endJob()
{
  rootManager_->WriteToFile();
  outputRootFile_->Close();

}//End of endJob                                                                                                    

// End of namespace gm2calo                                                                                        








// These are some necessary boilerplate for the ROOT persistency system

//using gm2strawtracker::ReadBankExampleT;

//DEFINE_ART_MODULE(ReadBankExampleT)
