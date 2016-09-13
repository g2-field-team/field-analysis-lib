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
#include "TMultiGraph.h"
#include "TRootEmbeddedCanvas.h"
#include "TCanvas.h"
#include "TGFrame.h"
#include "TGraph.h"


#include <boost/format.hpp>




/*-- Parameters ----------------------------------------------------*/


// vector of Defna parameters parametrizing wavefroms                                                              
                                                                                                         
std::vector<CALO_WAVEFORM>calo_waveforms[CALO_N_STATIONS+1][CALO_N_SEGMENTS+1];

std::vector<DEFNA>calo_defna[CALO_N_STATIONS+1][CALO_N_SEGMENTS+1];

// dummy defna structure                                                                                              
static DEFNA defna_dummy;


//! namespace

namespace gm2calo {

  class ReadBankExampleTGraph : public art::EDAnalyzer {

  public:

    explicit ReadBankExampleTGraph(fhicl::ParameterSet const & p);

    virtual ~ReadBankExampleTGraph();

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

  ReadBankExampleTGraph::ReadBankExampleTGraph(fhicl::ParameterSet const & pset)

    : art::EDAnalyzer(pset)

    , inputModuleLabel_( pset.get<std::string>("inputModuleLabel","MidasBankInput") )

  {}

  //! standard destructor

  ReadBankExampleTGraph::~ReadBankExampleTGraph() {}


  //! analyzes data

  void ReadBankExampleTGraph::analyze(const art::Event& event)

  {

    //initialize dummy variable                                                                                     
    defna_dummy.time = 0;
    defna_dummy.pedestal = 0;
    defna_dummy.amplitude = 0;
    defna_dummy.area = 0;
    defna_dummy.width = 0;

    std::vector<uint16_t>  datablock;

     unsigned int caloNum = 2;

    //! get histogram directory                                                                                     
    auto dir = rootManager_->GetDir(dirName_);




    for(unsigned int icalo=1; icalo<caloNum; icalo++)


      {


    //Get the Master CaloT MIDAS bank art record

    art::Handle< std::vector< gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloTBank> > > bankHandle;

    event.getByLabel(inputModuleLabel_, gm2midastoart::CaloTBank::GetBankName(icalo), bankHandle);

    auto bankCollection = *bankHandle;

    //Check there is only one in collection (only expect one per event from trigger)

    if( bankCollection.size() != 1 ) { 

      throw cet::exception("ReadBankExampleTGraph") << "Expected exactly one FC bank in event, found " << bankCollection.size();

      return;

    }


    uint16_t foo = 0;
    uint16_t *pdata = &foo;


    //Print bank values

    // mf::LogVerbatim("ReadBankExampleT") << "  FC bank :";

    auto bank = bankCollection.front();

    for(auto word : bank.words) {

      datablock.push_back(word);
      pdata = & datablock[0];

      //      std::stringstream ss; ss << boost::format(" ") % word;

      //mf::LogVerbatim("ReadBankExampleT") << ss.str();

    }

    mf::LogVerbatim("ReadBankExampleTGraph"); //New line


    for(auto word : bank.words) {
      std::stringstream ss; ss << boost::format("    0x%08lx") % word;
      datablock.clear();
    }




    int16_t data_size   = *pdata++;
    int16_t next1 = *pdata++;
    mf::LogVerbatim("ReadBankExampleTGraph") << "next1" << next1;


    int16_t n_islands   = *pdata++;
    mf::LogVerbatim("ReadBankExampleTGraph") << "data_size:" << data_size;
    mf::LogVerbatim("ReadBankExampleTGraph") << "n_islands:" << n_islands; 

    double evt = event.event();

    // mf::LogVerbatim("ReadBankExampleTGraph") << "event no:\n" << event.event();

    std::stringstream hName; hName << "IslandNo Vs. EventNo" << icalo;
    mf::LogVerbatim("ReadBankExampleTGraph") << "event no:\n" << event.event();
    //rootManager_->Get<TH2F*>(dir,hName.str())->GetXaxis()->SetTitle("Event No.");
    //rootManager_->Get<TH2F*>(dir,hName.str())->GetYaxis()->SetTitle("Island No.");
    //rootManager_->Get<TH2F*>(dir,hName.str())->SetMarkerColor(kRed);
    //rootManager_->Get<TH2F*>(dir,hName.str())->SetMarkerStyle(6);
    rootManager_->Get<TGraph*>(dir,"g_MyGraph")->SetPoint(rootManager_->Get<TGraph*>(dir,"g_MyGraph")->GetN(), evt, n_islands);

    //mf::LogVerbatim("ReadBankExampleTGraph") << "event no:\n" << event.event();

      
      }//id:caloNUM                                                                                             
  }//namespace



using gm2calo::ReadBankExampleTGraph;
DEFINE_ART_MODULE(ReadBankExampleTGraph)

void gm2calo::ReadBankExampleTGraph::beginJob()
{

  // create a root file and manager                                                                                 
  outputRootFileName_ = "ReadBankExampleTGraph.root";

  outputRootFile_.reset( new TFile(outputRootFileName_.c_str(),"recreate"));

  rootManager_.reset( new RootManager("ReadBankExampleTGraph",outputRootFile_.get()));

  std::string name = " ";

  //create directories                                                                                    
  auto dir = rootManager_->GetDir(dirName_,true);

  for (unsigned int icalo=1; icalo<2; icalo++)
    {

      std::stringstream nameStream; nameStream << "IslandNo Vs. EventNo" << icalo;

      //TGraph
      auto gr = new TGraph();
      gr->SetName("g_MyGraph");
      gr->SetTitle("Island No. vs. Event No. ; Event No. ; Island No.");
      gr->SetMarkerStyle(31);
      gr->SetMarkerColor(3);
      rootManager_->Add(dir, gr);
                                                                                                         
                           

    }//loop over calorimeters                                                                                         


}//End of beginJob                                                                                           






void gm2calo::ReadBankExampleTGraph::endJob()
{
  rootManager_->WriteToFile();
  outputRootFile_->Close();

}//End of endJob                                                                                                    

}// End of namespace gm2calo                                                                                        





