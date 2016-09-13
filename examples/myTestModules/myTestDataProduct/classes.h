// classes.h
// How ROOT knows how to deal with our data products
//

#include <vector>

#include "art/Persistency/Common/Wrapper.h"
#include "art/Persistency/Common/PtrVector.h"
#include "myTestModules/myTestDataProduct/myTestDataProduct.hh"


// For Cluster
// Template the vector (no typedef)
template class std::vector< myTestModules::myTestDataProduct >;
// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< myTestModules::myTestDataProduct >;
template class art::Wrapper< myTestModules::myTestDataProductCollection >;
// Template the art pointer
template class art::Ptr< myTestModules::myTestDataProduct >;

