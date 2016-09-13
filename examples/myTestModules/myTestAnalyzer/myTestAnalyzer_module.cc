//
//  Exercise 1: print the event id.
//

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Services/Optional/TFileService.h"
#include "myTestModules/myTestDataProduct/myTestDataProduct.hh"

#include "TH1D.h"

#include <iostream>

namespace myTestModules {

  class myTestAnalyzer : public art::EDAnalyzer {

  public:

    explicit myTestAnalyzer(fhicl::ParameterSet const& );

    void beginJob() override;
    void analyze(art::Event const& event) override;
  private:
    art::InputTag inputTag_;
    TH1D *hTest_;

  };

}

myTestModules::myTestAnalyzer::myTestAnalyzer(fhicl::ParameterSet const& pset):
  art::EDAnalyzer(pset),
  inputTag_(pset.get<std::string>("inputTag")),
  hTest_(nullptr){
  std::cout << "Hello from myTestAnalyzer::constructor." << std::endl;
}

void myTestModules::myTestAnalyzer::beginJob(){

    art::ServiceHandle<art::TFileService> tfs;

      hTest_ = tfs->make<TH1D>( "hTest", "Test histogram", 100, 0., 1000.);

}


void myTestModules::myTestAnalyzer::analyze(art::Event const& event){
  auto input = event.getValidHandle<myTestModules::myTestDataProduct>(inputTag_);
  hTest_->Fill(input->TimeStamp[0]%1000);
  std::cout << "Hello from myTestAnalyzer::analyze. Event id: "
            << event.id()
            << std::endl;
}

DEFINE_ART_MODULE(myTestModules::myTestAnalyzer)
