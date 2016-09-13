//
// Filter that returns true if the event number is odd.
//

#include "art/Framework/Core/EDFilter.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"

#include <iostream>

namespace myTestModules {

  class myTestFilter : public art::EDFilter {

  public:

    explicit myTestFilter(fhicl::ParameterSet const& );

    bool filter(art::Event& event) override;

  };

}

myTestModules::myTestFilter::myTestFilter(fhicl::ParameterSet const& ){
}

bool myTestModules::myTestFilter::filter(art::Event& event ){
  bool isOdd = event.id().event()%2 == 1;
  std::cout << "Event: "
            << event.id()
            << ((isOdd) ? " Pass" : " Fail")
            << std::endl;
  return isOdd;
}

DEFINE_ART_MODULE(myTestModules::myTestFilter)
