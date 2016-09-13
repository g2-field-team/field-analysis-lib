// classes.h
// How ROOT knows how to deal with our data products
//

#include <vector>

#include "art/Persistency/Common/Wrapper.h"
#include "art/Persistency/Common/PtrVector.h"
#include "DAQHeader.hh"
#include "RiderArtRecord.hh"
#include "IslandArtRecord.hh"
#include "FitResultArtRecord.hh"
#include "CrystalHitArtRecord.hh"
#include "ClusterArtRecord.hh"
#include "QHistArtRecord.hh"
#include "DeciQHistArtRecord.hh"
#include "ItalianoArtRecord.hh"

// For CDFHeader
// Template the vector (no typedef)
template class std::vector< gm2calo::CDFHeader >;
// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< gm2calo::CDFHeaderCollection >;
// Template the art pointer
template class art::Ptr< gm2calo::CDFHeader >;

// For AMCHeader
// Template the vector (no typedef)
template class std::vector< gm2calo::AMCHeader >;
// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< gm2calo::AMCHeaderCollection >;
// Template the art pointer
template class art::Ptr< gm2calo::AMCHeader >;

// For FC7Header
// Template the vector (no typedef)
template class std::vector< gm2calo::FC7Header >;
// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< gm2calo::FC7HeaderCollection >;
// Template the art pointer
template class art::Ptr< gm2calo::FC7Header >;

// For RiderChannelHeader
// Template the vector (no typedef)
template class std::vector< gm2calo::RiderChannelHeader >;
// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< gm2calo::RiderChannelHeaderCollection >;
// Template the art pointer
template class art::Ptr< gm2calo::RiderChannelHeader >;

// For Rider
// Template the vector (no typedef)
template class std::vector< gm2calo::RiderArtRecord >;
// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< gm2calo::RiderArtRecordCollection >;
// Template the art pointer
template class art::Ptr< gm2calo::RiderArtRecord >;

// For CaloRider
// Template the vector (no typedef)
template class std::vector< gm2calo::CaloRiderArtRecord >;
// Template the wrapper (typedef okay)
template class art::Wrapper< gm2calo::CaloRiderArtRecord >;
// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< gm2calo::CaloRiderArtRecordCollection >;
// Template the art pointer
template class art::Ptr< gm2calo::CaloRiderArtRecord >;

// For Islands
// Template the vector (no typedef)
template class std::vector< gm2calo::IslandArtRecord >;
// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< gm2calo::IslandArtRecordCollection >;
// Template the art pointer
template class art::Ptr< gm2calo::IslandArtRecord >;

// For IslandMatrices
// Template the vector (no typedef)
template class std::vector< gm2calo::MatrixIslandArtRecord >;
// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< gm2calo::MatrixIslandArtRecordCollection >;
// Template the art pointer
template class art::Ptr< gm2calo::MatrixIslandArtRecord >;

// For CaloIsland
// Template the vector (no typedef)
template class std::vector< gm2calo::CaloIslandArtRecord >;
// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< gm2calo::CaloIslandArtRecordCollection >;
// Template the art pointer
template class art::Ptr< gm2calo::CaloIslandArtRecord >;

// For FitResults
// Template the vector (no typedef)
template class std::vector< gm2calo::FitResultArtRecord >;
// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< gm2calo::FitResultArtRecordCollection >;
// Template the art pointer
template class art::Ptr< gm2calo::FitResultArtRecord >;

//for italiano
template class std::vector< gm2calo::ItalianoArtRecord >;
// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< gm2calo::ItalianoArtRecordCollection >;
// Template the art pointer
template class art::Ptr< gm2calo::ItalianoArtRecord >;

// For MatrixFitResults
// Template the vector (no typedef)
template class std::vector< gm2calo::MatrixFitResultArtRecord >;
// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< gm2calo::MatrixFitResultArtRecordCollection >;
// Template the art pointer
template class art::Ptr< gm2calo::MatrixFitResultArtRecord >;

// For CaloFitResults
// Template the vector (no typedef)
template class std::vector< gm2calo::CaloFitResultArtRecord >;
// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< gm2calo::CaloFitResultArtRecordCollection >;
// Template the art pointer
template class art::Ptr< gm2calo::CaloFitResultArtRecord >;

// For CrystalHit
// Template the vector (no typedef
template class std::vector< gm2calo::CrystalHitArtRecord >;
// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< gm2calo::CrystalHitArtRecordCollection >;
// Template the art pointer
template class art::Ptr< gm2calo::CrystalHitArtRecord >;

// For MatrixCrystalHit
// Template the vector (no typedef
template class std::vector< gm2calo::MatrixCrystalHitArtRecord >;
// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< gm2calo::MatrixCrystalHitArtRecordCollection >;
// Template the art pointer
template class art::Ptr< gm2calo::MatrixCrystalHitArtRecord >;

// For CaloCrystalHit
// Template the vector (no typedef
template class std::vector< gm2calo::CaloCrystalHitArtRecord >;
// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< gm2calo::CaloCrystalHitArtRecordCollection >;
// Template the art pointer
template class art::Ptr< gm2calo::CaloCrystalHitArtRecord >;

// For Cluster
// Template the vector (no typedef)
template class std::vector< gm2calo::ClusterArtRecord >;
// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< gm2calo::ClusterArtRecordCollection >;
// Template the art pointer
template class art::Ptr< gm2calo::ClusterArtRecord >;

// For MatrixCluster
// Template the vector (no typedef)
/*template class std::vector< gm2calo::MatrixClusterArtRecord >;
// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< gm2calo::MatrixClusterArtRecordCollection >;
// Template the art pointer
template class art::Ptr< gm2calo::MatrixClusterArtRecord >;*/

// For CaloCluster
// Template the vector (no typedef)
template class std::vector< gm2calo::CaloClusterArtRecord >;
// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< gm2calo::CaloClusterArtRecordCollection >;
// Template the art pointer
template class art::Ptr< gm2calo::CaloClusterArtRecord >;

// For Qhist
// Template the vector (no typedef)
template class std::vector< gm2calo::QHistArtRecord >;
// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< gm2calo::QHistArtRecordCollection >;
// Template the art pointer
template class art::Ptr< gm2calo::QHistArtRecord >;

// For CaloQhist
// Template the vector (no typedef)
template class std::vector< gm2calo::CaloQHistArtRecord >;
// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< gm2calo::CaloQHistArtRecordCollection >;
// Template the art pointer
template class art::Ptr< gm2calo::CaloQHistArtRecord >;

// For CaloDecimated Qhist
// Template the vector (no typedef)
template class std::vector< gm2calo::CaloDeciQHistArtRecord >;
// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< gm2calo::CaloDeciQHistArtRecordCollection >;
// Template the art pointer
template class art::Ptr< gm2calo::CaloDeciQHistArtRecord >;
