#ifndef STRAWCLUSTERARTRECORD_HH
#define STRAWCLUSTERARTRECORD_HH

#include <vector>
#include "art/Persistency/Common/PtrVector.h"
#include "StrawTimeIslandArtRecord.hh"
#include "StrawDigitArtRecord.hh"

namespace gm2strawtracker {

    struct StrawClusterArtRecord {

        // cluster's time island
        art::Ptr< gm2strawtracker::StrawTimeIslandArtRecord > island;

        // straw digits used in the cluster 
        std::vector< art::Ptr< gm2strawtracker::StrawDigitArtRecord > > strawDigits;

        // number of digits in the cluster
        int nstrawDigs;

        // the mean time of the digits associated with this cluster
        double time;

        // the initial time of the cluster
        double t0;

        // the error associated with the t0
        double errT0;

        // the coordinate position (u or v)
        double coordPos;

        // the error on the coordinate position
        double errCoordPos;

        // the z position
        double zPos;

        // the error on the z position
        double errZPos;

        // the cluster view
        StrawView view;

        // is the cluster's digit shared with another cluster
        bool hasSharedDigit; 
       
        // does one of the cluster's digits overlap another digit in the event
        bool hasOverlapDigit;
 
        // Default constructor
        StrawClusterArtRecord() 
           : island()
           , strawDigits()
           , nstrawDigs(0)
           , time(0.)
           , t0(0.)
           , errT0(0.)
           , coordPos(0.)
           , errCoordPos(0.)
           , zPos(0.)
           , errZPos(0.)
           , view()
           , hasSharedDigit(false)
           , hasOverlapDigit(false)
         {}

#ifndef __GCCXML__
   
        // Copy constructor
        StrawClusterArtRecord(const StrawClusterArtRecord& cluster) 
           : island(cluster.island)
           , strawDigits(cluster.strawDigits)
           , nstrawDigs(cluster.nstrawDigs)
           , time(cluster.time)
           , t0(cluster.t0)
           , errT0(cluster.errT0)
           , coordPos(cluster.coordPos)
           , errCoordPos(cluster.errCoordPos)
           , zPos(cluster.zPos)
           , errZPos(cluster.errZPos)
           , view(cluster.view)
           , hasSharedDigit(cluster.hasSharedDigit)
           , hasOverlapDigit(cluster.hasOverlapDigit)
        {}

        //! special functions so that objects can be compared
        friend bool operator==(const StrawClusterArtRecord& left, const StrawClusterArtRecord& right) {
          if( left.strawDigits     == right.strawDigits  &&
              left.nstrawDigs      == right.nstrawDigs   &&
              left.time            == right.time         &&
              left.t0              == right.t0           &&
              left.errT0           == right.errT0        &&
              left.coordPos        == right.coordPos     &&
              left.errCoordPos     == right.errCoordPos  && 
              left.zPos            == right.zPos         &&
              left.errZPos         == right.errZPos      &&
              left.view            == right.view         &&
              left.hasSharedDigit  == right.hasSharedDigit &&
              left.hasOverlapDigit == right.hasOverlapDigit ) return true;
           return false;
        }

#endif // __GCCXML__ 

    }; // StrawTimeClusterArtRecord struct definition

    typedef std::vector<StrawClusterArtRecord> StrawClusterArtRecordCollection;
    typedef std::vector< art::Ptr<StrawClusterArtRecord> > StrawClusterPtrCollection;
    
} // namespace

#endif

