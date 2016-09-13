#ifndef STRAWDIGITARTRECORD_HH
#define STRAWDIGITARTRECORD_HH

#include <vector>
#include "art/Persistency/Common/PtrVector.h"
#include "gm2ringsim/strawtracker/StrawArtRecord.hh"
#include "gm2ringsim/actions/track/TrajectoryArtRecord.hh"
#include "gm2geom/coordSystems/CoordSystem3Vector.hh"
#include "gm2geom/strawtracker/WireID.hh"
#include "StrawRawDigitArtRecord.hh"

namespace gm2strawtracker {

    struct StrawMCDigit {

        // primary Geant4 particle that contributed to the digit
        std::string particleName; 

        // is Geant4 primary particle responsible for the creation of this digit
        bool isCreatedFromPrimary;

        // Geant4 time particle causing the rising edge
        double time; 

        // Geant4 average energy deposited in the straw chamber
        double energy;

        // drift distance of charge from particle causing the rising edg to wire (DCA of track to wire)
        double driftDistance;

        // time taken to drift distance
        double driftTime;

        // "position" of digit, e.g. point of closest approach of particle (the one causing the rising edge) to wire
        gm2geom::CoordSystem3Vector position; 

        // Geant4 hits associated with the digit
        std::vector< art::Ptr< gm2ringsim::StrawArtRecord > > strawMCHits;

        // default constructor
        StrawMCDigit() 
          : particleName("")
          , isCreatedFromPrimary(false)
          , time(0.)
          , energy(0.)
          , driftDistance(0.)
          , driftTime(0.)
          , position()
          , strawMCHits()
        {}

#ifndef __GCCXML__

        // copy constructor
        StrawMCDigit(const StrawMCDigit& digit) 
          : particleName(digit.particleName)
          , isCreatedFromPrimary(digit.isCreatedFromPrimary)
          , time(digit.time)
          , energy(digit.energy)
          , driftDistance(digit.driftDistance)
          , driftTime(digit.driftTime)
          , position(digit.position)
          , strawMCHits(digit.strawMCHits)
        {}

        friend bool operator==(const StrawMCDigit& left, const StrawMCDigit& right) {
          if( left.particleName          == right.particleName          &&
              left.time                  == right.time                  && 
              left.energy                == right.energy                &&
              left.driftDistance         == right.driftDistance         &&
              left.driftTime             == right.driftTime             &&
              left.position              == right.position              &&
              left.strawMCHits           == right.strawMCHits           && 
              left.isCreatedFromPrimary  == right.isCreatedFromPrimary ) return true;
           return false;
        }

        // get trajectories that caused digit (from the truth MC hits)
        // note that which trajectories are stored depends on TrackingAction fcl params
        std::vector< art::Ptr<gm2ringsim::TrajectoryArtRecord> > getTrajectories() const {
           std::vector< art::Ptr<gm2ringsim::TrajectoryArtRecord> > trajectories;
           for(auto hit : strawMCHits) {
             if( hit->trajectory.isNonnull() ) trajectories.push_back( hit->trajectory );
             else throw cet::exception("StrawDigitArtRecord") << "Null trajectory pointer in StrawMCDigit\n";
           }
           return trajectories;
        }
  
        // get trajectories from primary mu+ that caused digit 
        // note that primary mu+ trajectories are only stored if StorePrimaryTrajectory set on TrackingAction
        std::vector< art::Ptr<gm2ringsim::TrajectoryArtRecord> > getPrimaryAntiMuonTrajectories() const { 
           std::vector< art::Ptr<gm2ringsim::TrajectoryArtRecord> > trajectories;
           for(auto hit : strawMCHits) {
             if( hit.isNonnull() ) { //Check truth hit art::Ptr is set
               if( hit->trajectory.isNonnull() ) { //Check trajectory art::Ptr is set
                 if( hit->trajectory.get()->isPrimaryAntiMuon() ) {
                   trajectories.push_back( hit->trajectory );
                 }
               }
             }
             else throw cet::exception("StrawDigitArtRecord") << "Null trajectory pointer in StrawMCDigit\n";
           }
           return trajectories;
        }
 
        // get trajectories from e+ from initial mu+ decay that caused digit 
        // note that pimary decay product trajectories are only stored if StorePrimaryDaughterTrajectories set on TrackingAction
        std::vector< art::Ptr<gm2ringsim::TrajectoryArtRecord> > getPrimaryDecayPositronTrajectories() const { 
           std::vector< art::Ptr<gm2ringsim::TrajectoryArtRecord> > trajectories;
           for(auto hit : strawMCHits) {
             if( hit.isNonnull() ) { //Check truth hit art::Ptr is set
               if( hit->trajectory.isNonnull() ) { //Check trajectory art::Ptr is set
                 if( hit->trajectory.get()->isPrimaryDecayPositron() ) {
                   trajectories.push_back( hit->trajectory );
                 }
               }
             }
             else throw cet::exception("StrawDigitArtRecord") << "Null trajectory pointer in StrawMCDigit\n";
           }
           return trajectories;
        }

        // get trajectories from all secondaries (e.g. not initial mu+ or primary decay products) that caused digit
        // note that secondary trajectories are only stored if StoreOrphanTrajectories set on TrackingAction
        std::vector< art::Ptr<gm2ringsim::TrajectoryArtRecord> > getSecondaryTrajectories() const { 
           std::vector< art::Ptr<gm2ringsim::TrajectoryArtRecord> > trajectories;
           for(auto hit : strawMCHits) {
             if( hit.isNonnull() ) { //Check truth hit art::Ptr is set
               if( hit->trajectory.isNonnull() ) { //Check trajectory art::Ptr is set
                if( hit->trajectory.get()->isSecondary() ) {
                   trajectories.push_back( hit->trajectory );
                 }
               }
             }
             else throw cet::exception("StrawDigitArtRecord") << "Null trajectory pointer in StrawMCDigit\n";
           }
           return trajectories;
        }

        // check if at least one truth hit used to form digit was from primary mu+ 
        bool formedFromPrimaryAntiMuon() const {
          for ( auto hit : strawMCHits ) {
            if( hit->parent_ID == 0 && hit->particle_name == "mu+" ) return true;
          }
          return false;
        }

        // check if at least one truth hit used to form digit was from e+ from parimart mu+ decay 
        bool formedFromPrimaryDecayPositron() const {
          for ( auto hit : strawMCHits ) {
            if( hit->parent_ID == 1 && hit->particle_name == "e+" ) return true;
          }
          return false;
        }

        // check if digit was formed entirely from secondaries (not the mu+ or decay e+)
        // TODO the decay neutrinos are missed here, write filter to remove nu hits
        bool formedFromSecondary() const {
          return ! ( formedFromPrimaryAntiMuon() && formedFromPrimaryDecayPositron() );
        }
  
#endif

    }; //-- end of straw mc digit struct          


    struct StrawDigitArtRecord {

        // mc digit additional information
        StrawMCDigit strawMCDigit;

        // raw hit information, empty for MC
        art::Ptr< gm2strawtracker::StrawRawDigitArtRecord > strawRawDigit;

        // wire geometric information 
        WireID wireID;

        // raw time (ns)
        double hitTime;

        // reconstructed drift time using a pre-determine t0 
        double driftTime;

        // error on the drift time
        double driftTimeError;

        // reconstructed drift distance calculated using the reconstructed drift distance
        double driftDistance;

        // error on the drift distance
        double driftDistanceError;

        // time the particle enters the straw tube (hitTime + driftTime)
        double calTime;

        // the wire coordinate position
        double coordPos;

        // the wire z position
        double zPos;

        // the wire center position in the tracker reference frame
        gm2geom::CoordSystem3Vector trackerPosition;

        // the wire position with respect to the ring
        gm2geom::CoordSystem3Vector globalPosition;

        // Default constructor
        StrawDigitArtRecord() 
           : strawMCDigit()
           , strawRawDigit()
           , wireID()
           , hitTime(0.)
           , driftTime(0.)
           , driftTimeError(0.)
           , driftDistance(0.)
           , driftDistanceError(0.)
           , calTime(0.)
           , coordPos(0.)
           , zPos(0.)
           , trackerPosition()
           , globalPosition()
         {}

        //! A nicer constructor: ROOT doesn't see it, but other classes can.
#ifndef __GCCXML__

        // Copy constructor
        StrawDigitArtRecord(const StrawDigitArtRecord& digit) 
           : strawMCDigit(digit.strawMCDigit)
           , strawRawDigit(digit.strawRawDigit)
           , wireID(digit.wireID)
           , hitTime(digit.hitTime)
           , driftTime(digit.driftTime)
           , driftTimeError(digit.driftTimeError)
           , driftDistance(digit.driftDistance)
           , driftDistanceError(digit.driftDistanceError)
           , calTime(digit.calTime)
           , coordPos(digit.coordPos)
           , zPos(digit.zPos)
           , trackerPosition(digit.trackerPosition)
           , globalPosition(digit.globalPosition)
        {}

        StrawDigitArtRecord(const StrawDigitArtRecord& digit, double time, double eTime, double distance, double eDistance) 
           : StrawDigitArtRecord(digit)
        {
           driftTime          = time;
           driftTimeError     = eTime;
           driftDistance      = distance;
           driftDistanceError = eDistance;
           calTime            = digit.hitTime + time;
        }

        // special functions so that objects can be compared
        friend bool operator==(const StrawDigitArtRecord& left, const StrawDigitArtRecord& right) {
          if( left.wireID          == right.wireID         &&
              left.hitTime         == right.hitTime        &&
              left.driftTime       == right.driftTime      &&
              left.driftTimeError  == right.driftTimeError &&
              left.driftDistance   == right.driftDistance  &&
              left.driftDistanceError == right.driftDistanceError && 
              left.coordPos        == right.coordPos        &&
              left.zPos            == right.zPos            &&
              left.strawMCDigit    == right.strawMCDigit    &&  
              left.trackerPosition == right.trackerPosition &&
              left.globalPosition  == right.globalPosition  ) return true; 
           return false;
        }
 
        // check if digit has MC hits (e.g. is from sim not data)
        bool hasMCHits() const {
          return strawMCDigit.strawMCHits.empty() ? false : true;
        }

#endif // __GCCXML__

    }; // StrawDigitArtRecord struct definition

    typedef std::vector<StrawDigitArtRecord> StrawDigitArtRecordCollection;
    typedef std::vector< art::Ptr<StrawDigitArtRecord> > StrawDigitPtrCollection;
    
} // namespace

#endif

