#ifndef TRACKARTRECORD_HH
#define TRACKARTRECORD_HH

#include <vector>
#include "art/Persistency/Common/PtrVector.h"
#include "gm2geom/coordSystems/CoordSystem3Vector.hh"
#include "Helix.hh"
#include "gm2dataproducts/strawtracker/StrawTimeIslandArtRecord.hh"
#include "gm2dataproducts/strawtracker/StrawClusterArtRecord.hh"
#include "gm2dataproducts/strawtracker/TrackStateArtRecord.hh"

namespace gm2strawtracker {

    enum ConvergeType {
        helixStable = 1,
        chi2Test    = 2
    };

    struct TrackArtRecord {

        // time island associated with the track candidate
        art::Ptr< gm2strawtracker::StrawTimeIslandArtRecord > island;

        // straw clusters associated with the track -- these are measured positions on the track
        std::vector< art::Ptr< gm2strawtracker::StrawClusterArtRecord > > strawClusters;

        // track states associated with the track -- these are predicted positions on the track
        std::vector< art::Ptr< gm2strawtracker::TrackStateArtRecord > > states;
        
        // helical parameters of the track 
        Helix helix;

        // the number of iterations it took to find the best fitted track
        int niterations;

        // fit chisquared
        double chi2;

        // number degrees of freedom
        unsigned int dof;

        // chi2/dof
        double chi2DoF;

        // the covariance matrix
        TrackMatrix5x5 covMatrix;
        
        // charge sign of the track
        int charge;

        // momentum associated with the track
        gm2geom::CoordSystem3Vector momentum;

        // initial time of the track
        double time;

        // which convergent test did the track pass
        ConvergeType type;
 
        //! Default constructor
        TrackArtRecord() 
           : island()
           , strawClusters()
           , states()
           , helix()
           , niterations(1)
           , chi2(0.)
           , dof(0)
           , chi2DoF(0.)
           , covMatrix()
           , charge(1)
           , momentum()
           , time(0.)
           , type()
        {}


#ifndef __GCCXML__
   
        //! Copy constructor
        TrackArtRecord(const TrackArtRecord& track) 
           : island(track.island)
           , strawClusters(track.strawClusters)
           , states(track.states)
           , helix(track.helix)
           , niterations(track.niterations)
           , chi2(track.chi2)
           , dof(track.dof)
           , chi2DoF(track.chi2DoF)
           , covMatrix(track.covMatrix)
           , charge(track.charge)
           , momentum(track.momentum)
           , time(track.time)
           , type(track.type)
        {}

        // special functions so that objects can be compared
        friend bool operator==(const TrackArtRecord& left, const TrackArtRecord& right) {
          if( left.island        == right.island        &&
              left.strawClusters == right.strawClusters &&
              left.states        == right.states        &&
              left.helix         == right.helix         &&
              left.niterations   == right.niterations   &&
              left.chi2          == right.chi2          &&
              left.dof           == right.dof           && 
              left.chi2DoF       == right.chi2DoF       && 
              left.charge        == right.charge        &&
              left.momentum      == right.momentum      &&
              left.time          == right.time          &&
              left.type          == right.type          &&
	      left.covMatrix     == right.covMatrix ) return true;
           return false;
        }
 
#endif


    }; // TrackArtRecord struct definition

    typedef std::vector<TrackArtRecord> TrackArtRecordCollection;
    typedef std::vector< art::Ptr<TrackArtRecord> > TrackPtrCollection;
    
} // namespace

#endif

