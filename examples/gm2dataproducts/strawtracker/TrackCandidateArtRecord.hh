#ifndef TRACKCANDIDATEARTRECORD_HH
#define TRACKCANDIDATEARTRECORD_HH

#include <vector>
#include "art/Persistency/Common/PtrVector.h"
#include "Helix.hh"
#include "gm2dataproducts/strawtracker/StrawClusterArtRecord.hh"
#include "gm2dataproducts/strawtracker/StrawSeedArtRecord.hh"
#include "gm2dataproducts/strawtracker/StrawTimeIslandArtRecord.hh"
#include "gm2geom/coordSystems/CoordSystem3Vector.hh"

namespace gm2strawtracker {

    struct TrackCandidateArtRecord {
       
        // time island associated with the track candidate
        art::Ptr< gm2strawtracker::StrawTimeIslandArtRecord > island;

        // straw clusters associated with the track candidate
        std::vector< art::Ptr< gm2strawtracker::StrawClusterArtRecord > > strawClusters;

        // straw seeds associated with the track candidate
        std::vector< art::Ptr< gm2strawtracker::StrawSeedArtRecord > > strawSeeds;
 
        // helical parameters of the track candidate
        Helix helix;

        // parameters from fitting the track candidate to a circle
        Circle circle;

        // parameter from fitting the track candidate to a line
        Line line;

        // was the track fitted to a helix or has helix parameters
        bool helixFit;

        // was the track fitted to a circle
        bool circleFit;

        // was the track fitted to a line
        bool linearFit;

        // fit chisquared
        double chi2;

        // number degrees of freedom
        unsigned int dof;

        // chi2/dof
        double chi2DoF;

        // the covariance matrix
        TrackMatrix5x5 covMatrix;
        
        // charge of the track
        int charge;

        // momentum of track
        gm2geom::CoordSystem3Vector momentum;
        
        // Default constructor
        TrackCandidateArtRecord() 
           : island()
           , strawClusters()
           , strawSeeds()
           , helix()
           , circle()
           , line()
           , helixFit(false)
           , circleFit(false)
           , linearFit(false)
           , chi2(0.)
           , dof(0)
           , chi2DoF(0.)
           , covMatrix()
           , charge(1)
           , momentum()
        {}


#ifndef __GCCXML__
   
        // Copy constructor
        TrackCandidateArtRecord(const TrackCandidateArtRecord& track) 
           : island(track.island)
           , strawClusters(track.strawClusters)
           , strawSeeds(track.strawSeeds)
           , helix(track.helix)
           , circle(track.circle)
           , line(track.line)
           , helixFit(track.helixFit)
           , circleFit(track.circleFit)
           , linearFit(track.linearFit)
           , chi2(track.chi2)
           , dof(track.dof)
           , chi2DoF(track.chi2DoF)
           , covMatrix(track.covMatrix)
           , charge(track.charge)
           , momentum(track.momentum)
        {}
 
        // special functions so that objects can be compared
        friend bool operator==(const TrackCandidateArtRecord& left, const TrackCandidateArtRecord& right) {
          if( left.island        == right.island        &&
              left.strawClusters == right.strawClusters &&
              left.strawSeeds    == right.strawSeeds    &&
              left.helix         == right.helix         &&
              left.chi2          == right.chi2          &&
              left.dof           == right.dof           && 
              left.chi2DoF       == right.chi2DoF       && 
              left.momentum      == right.momentum      &&
	      left.covMatrix     == right.covMatrix ) return true;
           return false;
        }

#endif


    }; // TrackCandidateArtRecord struct definition

    typedef std::vector<TrackCandidateArtRecord> TrackCandidateArtRecordCollection;
    typedef std::vector< art::Ptr<TrackCandidateArtRecord> > TrackCandidatePtrCollection;
    
} // namespace

#endif

