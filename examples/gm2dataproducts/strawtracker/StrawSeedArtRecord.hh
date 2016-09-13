#ifndef STRAWSEEDARTRECORD_HH
#define STRAWSEEDARTRECORD_HH

#include <vector>
#include "art/Persistency/Common/PtrVector.h"
#include "gm2geom/coordSystems/CoordSystem3Vector.hh"
#include "StrawDigitArtRecord.hh"
#include "StrawTimeIslandArtRecord.hh"
#include "StrawClusterArtRecord.hh"

using namespace std;

namespace gm2strawtracker {
  
    enum SeedType {
        unknown = 0,
        singlet = 1, // seed contains one cluster in any view
        doublet = 2, // seed contains two clusters in different views
        triplet = 3  // seed contains three clusters in the same view
    };

    enum CoordinateFrame {
        tracker = 0,
        ring    = 1
    };

    // a support struct which better encapsulated the positions and errors associated with a state
    struct SeedNode {
        double time;

        double rError;
   
        gm2geom::CoordSystem3Vector position;
        gm2geom::CoordSystem3Vector posError;

        art::Assns< gm2strawtracker::StrawDigitArtRecord, gm2strawtracker::StrawClusterArtRecord > strawDigitToCluster;

        SeedNode()
           : time(0.)
           , rError(0.)
           , position()
           , posError()
           , strawDigitToCluster()
        {}
        
#ifndef __GCCXML__

        SeedNode( const SeedNode& node )
           : time(node.time)
           , rError(node.rError)
           , position(node.position)
           , posError(node.posError)
           , strawDigitToCluster(node.strawDigitToCluster)
        {}

        friend bool operator==(const SeedNode& left, const SeedNode& right) {
          if( left.time     == right.time     &&
              left.rError   == right.rError   && 
              left.position == right.position &&
              left.posError == right.posError ) return true;
           return false;
        }
 

#endif 

    }; 

    struct StrawSeedArtRecord {

        // cluster's time island
        art::Ptr< gm2strawtracker::StrawTimeIslandArtRecord > island;

        // digits associated with the seed
        std::vector< art::Ptr<gm2strawtracker::StrawDigitArtRecord> > strawDigits;

        // clusters associated with the seed
        std::vector< art::Ptr<gm2strawtracker::StrawClusterArtRecord> > strawClusters;

        // the mean of the time, position, etc associated with the seed
        SeedNode node;

        // a container of the time, position, etc associated the the seed
        std::vector< SeedNode > nodes;

        // the line slope of the digits on the seed
        double slope;

        // the line intercept of the digits on the seed
        double intercept;

        // contain a shared cluster
        bool hasSharedCluster;

        // number and type of clusters associated with the seed
        SeedType type;

        // coordinate frame of the measured positions
        CoordinateFrame frame;

        // the module number pertaining to the seed's most upstream cluster
        int frontModule;

        // the module number pertaining to the seed's most downstream cluster
        int backModule;
        
        // the view pertaining to the seed's most upstream cluster
        StrawView frontView;

        // the view pertaining to the seed's most downstream cluster
        StrawView backView;
         
        // Default constructor
        StrawSeedArtRecord() 
            : island()
            , strawDigits()
            , strawClusters()
            , node()
            , nodes()
            , slope(0.)
            , intercept(0.)
            , hasSharedCluster(false)
            , type()
            , frame()
            , frontModule(-1)
            , backModule(-1)
            , frontView()
            , backView()
        {}

#ifndef __GCCXML__

        // copy constructor
        StrawSeedArtRecord( const StrawSeedArtRecord& seed ) 
            : island(seed.island)
            , strawDigits(seed.strawDigits)
            , strawClusters(seed.strawClusters)
            , node(seed.node)
            , nodes(seed.nodes)
            , slope(seed.slope)
            , intercept(seed.intercept)
            , hasSharedCluster(seed.hasSharedCluster)
            , type(seed.type)
            , frame(seed.frame)
            , frontModule(seed.frontModule)
            , backModule(seed.backModule)
            , frontView(seed.frontView)
            , backView(seed.backView)
        {}

        //! special functions so that objects can be compared
        friend bool operator==(const StrawSeedArtRecord& left, const StrawSeedArtRecord& right) {
          if( left.hasSharedCluster == right.hasSharedCluster &&
              left.strawDigits   == right.strawDigits   &&
              left.strawClusters == right.strawClusters &&
              left.node          == right.node          &&
              left.slope         == right.slope         &&
              left.intercept     == right.intercept     &&
              left.type          == right.type          &&
              left.frame         == right.frame         &&
              left.frontModule   == right.frontModule   &&
              left.backModule    == right.backModule    &&
              left.frontView     == right.frontView     &&
              left.backView      == right.backView      &&
              left.island        == right.island ) return true;
           return false;
        }

#endif //__GCCXML__

    }; // StrawTrackSeedArtRecord struct definition

    typedef std::vector<StrawSeedArtRecord> StrawSeedArtRecordCollection;
    typedef std::vector< art::Ptr<StrawSeedArtRecord> > StrawSeedPtrCollection;
    
} // namespace

#endif

