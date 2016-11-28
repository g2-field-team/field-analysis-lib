////////////////////////////////////////////////////////////////////////
// Class:       TrolleyUnpacker
//
// Module Type: producer
//
//// File:        TrolleyUnpacker_module.cc
//
//// Generated at Sun Nov 27 by Ran Hong, rhong@anl.gov 
//
//////////////////////////////////////////////////////////////////////////


#include "gm2dataproducts/field/TrolleyArtRecord.hh"
#include "gm2dataproducts/daq/MidasEventArtRecord.hh"
#include "field_constants.hh"
#include "field_structs.hh"

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Run.h"

#include <iostream>
#include <vector>
#include <memory>

namespace g2field {

  class TrolleyUnpacker : public art::EDProducer {

  public:

    explicit TrolleyUnpacker(fhicl::ParameterSet const& pset);

    void produce( art::Event& event) override;
    void endRun ( art::Run&   run  ) override;

  private:

    // Input tags for the two input data products.
    art::InputTag inputTag_;

  };

}

g2field::TrolleyUnpacker::TrolleyUnpacker(fhicl::ParameterSet const& pset ):
  inputTag_(pset.get<std::string>("inputTag")){

  produces<g2field::TrolleyNMRArtRecord>();
}

void g2field::TrolleyUnpacker::produce( art::Event& event){
  // Get the input data products
  art::Handle< gm2midastoart::MidasBankArtRecordCollection > bankHandle;
  event.getByLabel("MidasBankInput", "TLNP", bankHandle);
  const auto & TrolleyNMRBankCollection = *bankHandle; //Should have exactly one entry
  const auto & TrolleyNMRBank = TrolleyNMRBankCollection.front();

 // auto input = event.getValidHandle<std::vector<gm2midastoart::MidasEventArtRecord<gm2midastoart::GalilBank>>>(inputTag_);
  auto output = std::make_unique<g2field::TrolleyNMRArtRecord>(TrolleyNMRBank.gps_clock,TrolleyNMRBank.probe_index,TrolleyNMRBank.length,TrolleyNMRBank.trace);

  // Create empty data product and reserve the required size.

  // Add the product to the event
  event.put( std::move(output) );
}

void g2field::TrolleyUnpacker::endRun( art::Run& run){
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


DEFINE_ART_MODULE(g2field::TrolleyUnpacker)
