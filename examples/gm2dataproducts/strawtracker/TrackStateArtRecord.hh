#ifndef TRACKSTATEARTRECORD_HH
#define TRACKSTATEARTRECORD_HH

#include <vector>
#include "art/Persistency/Common/PtrVector.h"
#include "gm2geom/coordSystems/CoordSystem3Vector.hh"
#include "StrawTimeIslandArtRecord.hh"
#include "StrawClusterArtRecord.hh"
#include "StrawSeedArtRecord.hh"
#include "TrackCandidateArtRecord.hh"
#include "TrackParamDefs.hh"
#include "Helix.hh"

namespace gm2strawtracker {


    // the types of fitting procedures
    enum FitType {
         Unknown   = 0, //! do not know the procedure use for fitting
         LSQ       = 1, //! least squares minimizer
         Kalman    = 2, //! Kalman filter
         KalmanDCA = 3,
         Assigned1 = 4  //! replace with your fitter
    };

    struct TrackStateArtRecord {

        // time island associated with the state
        art::Ptr< gm2strawtracker::StrawTimeIslandArtRecord > island;

        // straw digit associated with the state
        art::Ptr< gm2strawtracker::StrawDigitArtRecord > strawDigit;

        // straw cluster associated with the state
        art::Ptr< gm2strawtracker::StrawClusterArtRecord > strawCluster;

        // straw seed associated with the state
        art::Ptr< gm2strawtracker::StrawSeedArtRecord > strawSeed;

        // track candidate associated with the state
        art::Ptr< gm2strawtracker::TrackCandidateArtRecord > trackCandidate;

        // the type of fitting procedures
        FitType type;

        // position associate with state
        gm2geom::CoordSystem3Vector position;
        
        // helix parameter per state
        Helix helix;

        // the covariance (error) matrix        
        TrackMatrix5x5 covMatrix;
        
        // the chi2 at the state
        double chi2;

        // momentum associated with the state
        gm2geom::CoordSystem3Vector momentum;
        
        // the magnitude of the magnetic field       
        double BfieldValue;

        // the magnetic field vector
        gm2geom::CoordSystem3Vector BfieldVect;

        // Default constructor
        TrackStateArtRecord() 
            : island()
            , strawDigit()
            , strawCluster()
            , trackCandidate()
            , type()
            , position()
            , helix()
            , covMatrix()
            , chi2(0.0)
            , momentum()
            , BfieldValue(0.0)
            , BfieldVect()
        {}

#ifndef __GCCXML__

        // Copy constructor
        TrackStateArtRecord(const TrackStateArtRecord& state) 
            : island(state.island)
            , strawDigit(state.strawDigit)
            , strawCluster(state.strawCluster)
            , trackCandidate(state.trackCandidate)
            , type(state.type)
            , position(state.position)
            , helix(state.helix)
            , covMatrix(state.covMatrix)
            , chi2(state.chi2)
            , momentum(state.momentum)
            , BfieldValue(state.BfieldValue)
            , BfieldVect(state.BfieldVect)
        {}

#endif // __GCCXML__

    }; // TrackStateArtRecord struct definition

    typedef std::vector<TrackStateArtRecord> TrackStateArtRecordCollection;
    typedef std::vector< art::Ptr<TrackStateArtRecord> > TrackStatePtrCollection;
    
} // namespace

#endif

