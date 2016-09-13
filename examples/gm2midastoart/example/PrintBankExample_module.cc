//Example of how to open a MIDAS bank art record
//Tom Stuttard (3rd August 2016)

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
#include "gm2dataproducts/daq/MidasEventArtRecord.hh"

//C/C++ includes                                                                                
#include <memory>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <boost/format.hpp>

//! namespace
namespace gm2calo {

  class ReadBankExample : public art::EDAnalyzer {

    public:

      explicit ReadBankExample(fhicl::ParameterSet const & p);
      virtual ~ReadBankExample();

      void analyze(const art::Event& event ) override; 

    private:

      //FCL params
      std::string inputModuleLabel_;

  };




  //! standard constructor
  ReadBankExample::ReadBankExample(fhicl::ParameterSet const & pset)
   : art::EDAnalyzer(pset)
   , inputModuleLabel_( pset.get<std::string>("inputModuleLabel","MidasBankInput") )
  {}

  //! standard destructor
  ReadBankExample::~ReadBankExample() {}

  //! analyzes data
  void ReadBankExample::analyze(const art::Event& event)
  {

    //
    // Example of reading a MIDAS bank
    //

    //In this example, getting CaloHeaderBank from the art event
    //CaloHeaderBank is a numbered bank, e.g. there are multiple banks of this type in the event (one per calo)
    //e.g. BC01, BC02, etc
    //To distinguish between these in the art event, the MIDAS 4 character bank name is used as the instance label
    //Get this name using GetBankName(int) with the bank number as the argument
    //If this bank was not numbered (e.g. TRIG or STRW banks), just use GetBankName() with no args as the instance name
    unsigned int caloNum = 1; //Only using one calo here
    art::Handle< std::vector< gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloHeaderBank> > > caloBankHandle;
    event.getByLabel(inputModuleLabel_, gm2midastoart::CaloHeaderBank::GetBankName(caloNum), caloBankHandle);
    auto caloBankCollection = *caloBankHandle;

    //Check there is only one element in collection (only expect one bank of each name per event)
    if( caloBankCollection.size() != 1 ) { 
      throw cet::exception("ReadBankExample") << "Expected exactly one \"" 
          << gm2midastoart::CaloHeaderBank::GetBankName(caloNum) 
          << "\" bank in event, found " << caloBankCollection.size();
    }
    const auto& caloHeaderData = caloBankCollection.begin()->words;

    //Print bank words
    mf::LogVerbatim("ReadBankExample") << "  Calo " << caloNum << " header bank :";
    for(auto word : caloHeaderData) {
      std::stringstream ss; ss << boost::format("    0x%08lx") % word;
      mf::LogVerbatim("ReadBankExample") << ss.str();
    }
    mf::LogVerbatim("ReadBankExample"); //New line

  }//analyze


}//namespace

// These are some necessary boilerplate for the ROOT persistency system
using gm2calo::ReadBankExample;
DEFINE_ART_MODULE(ReadBankExample)

