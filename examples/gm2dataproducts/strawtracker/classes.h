// classes.h
// How ROOT knows how to deal with our data products
//

#include <vector>

#include "art/Persistency/Common/Wrapper.h"
#include "art/Persistency/Common/Ptr.h"
#include "art/Persistency/Common/PtrVector.h"
#include "art/Persistency/Common/Assns.h"

#include "Line.hh"
#include "Circle.hh"
#include "Helix.hh"
#include "StrawDigitArtRecord.hh"
#include "StrawRawDigitArtRecord.hh"
#include "StrawTimeIslandArtRecord.hh"
#include "StrawClusterArtRecord.hh"
#include "StrawSeedArtRecord.hh"
#include "TrackStateArtRecord.hh"
#include "TrackCandidateArtRecord.hh"
#include "TrackArtRecord.hh"
#include "DecayVertexArtRecord.hh"

// For WireID
template class std::vector< gm2strawtracker::WireID >;

// For Line
template class std::vector< gm2strawtracker::Line >;

// For Circle
template class std::vector< gm2strawtracker::Circle >;

// For Helix
template class std::vector< gm2strawtracker::Helix >;

// For Seed Node
template class std::vector< gm2strawtracker::SeedNode >;

// For Straw Tracker MC Digits
template class std::vector< gm2strawtracker::StrawMCDigit >;

// For Straw Tracker Raw Digits
template class std::vector< gm2strawtracker::StrawRawDigitArtRecord >;
template class art::Wrapper< gm2strawtracker::StrawRawDigitArtRecord >;
template class art::Wrapper< gm2strawtracker::StrawRawDigitArtRecordCollection >;
template class art::Ptr< gm2strawtracker::StrawRawDigitArtRecord >;
template class std::vector< art::Ptr< gm2strawtracker::StrawRawDigitArtRecord > >;

// For Straw Tracker Digits
template class std::vector< gm2strawtracker::StrawDigitArtRecord >;
template class art::Wrapper< gm2strawtracker::StrawDigitArtRecord >;
template class art::Wrapper< gm2strawtracker::StrawDigitArtRecordCollection >;
template class art::Ptr< gm2strawtracker::StrawDigitArtRecord >;
template class std::vector< art::Ptr< gm2strawtracker::StrawDigitArtRecord > >;

// For Straw Tracker Time Island
template class art::Wrapper< gm2strawtracker::StrawTimeIslandArtRecord >;
template class std::vector< gm2strawtracker::StrawTimeIslandArtRecord >;
template class art::Wrapper< gm2strawtracker::StrawTimeIslandArtRecordCollection >;
template class art::Ptr< gm2strawtracker::StrawTimeIslandArtRecord >;
template class std::vector< art::Ptr< gm2strawtracker::StrawTimeIslandArtRecord > >;

// For Straw Tracker Clusters
template class std::vector< gm2strawtracker::StrawClusterArtRecord >;
template class art::Wrapper< gm2strawtracker::StrawClusterArtRecord >;
template class art::Wrapper< gm2strawtracker::StrawClusterArtRecordCollection >;
template class art::Ptr< gm2strawtracker::StrawClusterArtRecord >;
template class std::vector< art::Ptr< gm2strawtracker::StrawClusterArtRecord > >;

// For Straw Tracker Seeds
template class std::vector< gm2strawtracker::StrawSeedArtRecord >;
template class art::Wrapper< gm2strawtracker::StrawSeedArtRecord >;
template class art::Wrapper< gm2strawtracker::StrawSeedArtRecordCollection >;
template class art::Ptr< gm2strawtracker::StrawSeedArtRecord >;
template class std::vector< art::Ptr< gm2strawtracker::StrawSeedArtRecord > >;
template class art::Assns< gm2strawtracker::StrawDigitArtRecord, gm2strawtracker::StrawClusterArtRecord, void >;

// For Straw Tracker Track States
template class std::vector< gm2strawtracker::TrackStateArtRecord >;
template class art::Wrapper< gm2strawtracker::TrackStateArtRecord >;
template class art::Wrapper< gm2strawtracker::TrackStateArtRecordCollection >;
template class art::Ptr< gm2strawtracker::TrackStateArtRecord >;
template class std::vector< art::Ptr< gm2strawtracker::TrackStateArtRecord > >;

// For Straw Tracker Track Candidates
template class std::vector< gm2strawtracker::TrackCandidateArtRecord >;
template class art::Wrapper< gm2strawtracker::TrackCandidateArtRecord >;
template class art::Wrapper< gm2strawtracker::TrackCandidateArtRecordCollection >;
template class art::Ptr< gm2strawtracker::TrackCandidateArtRecord >;
template class std::vector< art::Ptr< gm2strawtracker::TrackCandidateArtRecord > >;

// For Straw Tracker Track 
template class std::vector< gm2strawtracker::TrackArtRecord >;
template class art::Wrapper< gm2strawtracker::TrackArtRecord >;
template class art::Wrapper< gm2strawtracker::TrackArtRecordCollection >;
template class art::Ptr< gm2strawtracker::TrackArtRecord >;
template class std::vector< art::Ptr< gm2strawtracker::TrackArtRecord > >;

// For Straw Tracker DecayVertex 
template class std::vector< gm2strawtracker::DecayVertexArtRecord >;
template class art::Wrapper< gm2strawtracker::DecayVertexArtRecord >;
template class art::Wrapper< gm2strawtracker::DecayVertexArtRecordCollection >;
template class art::Ptr< gm2strawtracker::DecayVertexArtRecord >;
template class std::vector< art::Ptr< gm2strawtracker::DecayVertexArtRecord > >;

// And for the vector of booleans
template class std::vector<bool>;

