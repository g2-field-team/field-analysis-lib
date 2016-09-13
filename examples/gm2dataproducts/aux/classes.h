// classes.h
// How ROOT knows how to deal with our data products
//

#include <vector>

#include "art/Persistency/Common/Wrapper.h"
#include "art/Persistency/Common/PtrVector.h"
#include "FredinoArtRecord.hh"
#include "gm2dataproducts/calo/DAQHeader.hh"

//for Fredino
template class std::vector< gm2aux::FredinoArtRecord >;
// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< gm2aux::FredinoArtRecordCollection >;
// Template the art pointer
template class art::Ptr< gm2aux::FredinoArtRecord >;

template class std::vector< gm2aux::FredinoIslandArtRecord >;
// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< gm2aux::FredinoIslandArtRecordCollection >;
// Template the art pointer
template class art::Ptr< gm2aux::FredinoIslandArtRecord >;

