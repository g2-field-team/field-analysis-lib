#ifndef DECAYVERTEXARTRECORD_HH
#define DECAYVERTEXARTRECORD_HH

#include <vector>
#include "art/Persistency/Common/PtrVector.h"
#include "gm2geom/coordSystems/CoordSystem3Vector.hh"
#include "gm2dataproducts/strawtracker/StrawTimeIslandArtRecord.hh"
#include "gm2dataproducts/strawtracker/TrackArtRecord.hh"

namespace gm2strawtracker {

    // the types of the vertex extrapolation procedures
    enum ExtrapolateType {
      Undefined     = 0, 
      MomentumBased = 1  // use the track momentum and most upstream position
    }; 

    struct DecayVertexArtRecord {

        // time island associated with the vertex
        art::Ptr< gm2strawtracker::StrawTimeIslandArtRecord > island;

        // track associated with vertex
        art::Ptr< gm2strawtracker::TrackArtRecord > track;

        // decay position of the track
        gm2geom::CoordSystem3Vector position;

        // decay position error of the track
        gm2geom::CoordSystem3Vector positionErr;

        // decay vertex time of the track
        double time;

        // decay vertex mometum of the track
        gm2geom::CoordSystem3Vector momentum;

        // vertex fit chisquared
        double chi2;
 
        // angle between the track momentum and the tangential vector of the circular muon trajectory
        double theta;

        // minimum distance from the circle of the ring ideal orbit
        gm2geom::CoordSystem3Vector distance;

        // type of vertex extrapolation procedure
        ExtrapolateType type;

        // Default constructor
        DecayVertexArtRecord() 
           : island()
           , track()
           , position()
           , positionErr()
           , time(0.)
           , momentum()
           , chi2(0.)
           , theta(0.)
           , distance()
           , type()
        {}

#ifndef __GCCXML__
   
        // Copy constructor
        DecayVertexArtRecord(const DecayVertexArtRecord& vertex) 
           : island(vertex.island)
           , track(vertex.track)
           , position(vertex.position)
           , positionErr(vertex.positionErr)
           , time(vertex.time)
           , momentum(vertex.momentum)
           , chi2(vertex.chi2)
           , theta(vertex.theta)
           , distance(vertex.distance)
           , type(vertex.type)
        {}

        // special functions so that objects can be compared
        friend bool operator==(const DecayVertexArtRecord& left, const DecayVertexArtRecord& right) {
          if( left.island      == right.island      &&
              left.track       == right.track       &&
              left.position    == right.position    &&
              left.positionErr == right.positionErr &&
              left.time        == right.time        &&
              left.momentum    == right.momentum    &&
              left.theta       == right.theta       &&
              left.distance    == right.distance    &&
              left.chi2        == right.chi2        && 
              left.type        == right.type  ) return true; 
           return false;
        }
 
#endif


    }; // DecayVertexArtRecord struct definition

    typedef std::vector<DecayVertexArtRecord> DecayVertexArtRecordCollection;
    typedef std::vector< art::Ptr<DecayVertexArtRecord> > DecayVertexPtrCollection;
    
} // namespace

#endif

