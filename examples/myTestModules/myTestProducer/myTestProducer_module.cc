//
// Create a new data product that is the union of the two Intersection
// data products.
//

//#include "/home/rhong/gm2/midas2art/srcs/myTestModules/myTestDataProduct/myTestDataProduct.hh"
#include "myTestModules/myTestDataProduct/myTestDataProduct.hh"
#include "gm2dataproducts/daq/MidasEventArtRecord.hh"

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Run.h"

#include <iostream>
#include <vector>
#include <memory>

namespace myTestModules {

  class myTestProducer : public art::EDProducer {

  public:

    explicit myTestProducer(fhicl::ParameterSet const& pset);

    void produce( art::Event& event) override;
    void endRun ( art::Run&   run  ) override;

  private:

    // Input tags for the two input data products.
    art::InputTag inputTag_;

  };

}

myTestModules::myTestProducer::myTestProducer(fhicl::ParameterSet const& pset ):
  inputTag_(pset.get<std::string>("inputTag")){

  produces<myTestModules::myTestDataProduct>();
}

void myTestModules::myTestProducer::produce( art::Event& event){
  // Get the input data products
  auto input = event.getValidHandle<std::vector<gm2midastoart::MidasEventArtRecord<gm2midastoart::GalilBank>>>(inputTag_);
  auto output = std::make_unique<myTestModules::myTestDataProduct>();
  myTestModules::myTestDataProduct OutObj;
  auto bankcollection = *input;
  OutObj.TimeStamp.clear();
  for (int i=0;i<50;i++){
    output->TimeStamp.push_back(bankcollection[0].words[9*i]);
    output->Position.push_back(bankcollection[0].words[9*i+3]);
    output->Velocity.push_back(bankcollection[0].words[9*i+5]);
  }

  // Create empty data product and reserve the required size.

  // Add the product to the event
  event.put( std::move(output) );
}

void myTestModules::myTestProducer::endRun( art::Run& run){
;
  // There is no run.ValidHandle<T>(); get the input data product the old way.
/*  art::Handle<MCRunSummary> summaryHandle;
  run.getByLabel(innertag_,summaryHandle);

  // Create an output data product that is a copy of the input one.
  auto output = std::make_unique<MCRunSummary>(*summaryHandle);

  // Add the product to the run.
  run.put( std::move(output) );
*/
}


DEFINE_ART_MODULE(myTestModules::myTestProducer)
