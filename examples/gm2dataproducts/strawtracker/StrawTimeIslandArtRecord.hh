#ifndef STRAWTIMEISLANDARTRECORD_HH
#define STRAWTIMEISLANDARTRECORD_HH

#include <vector>
#include "art/Persistency/Common/PtrVector.h"
#include "StrawDigitArtRecord.hh"

namespace gm2strawtracker {

    enum T0Finder {
        island  = 0,
        cluster = 1
    };

    struct StrawTimeIslandArtRecord {

        //! the associated digits
        std::vector< art::Ptr< gm2strawtracker::StrawDigitArtRecord > > strawDigits;

        //! the most upstream digit on the island
        art::Ptr< gm2strawtracker::StrawDigitArtRecord > upstreamDigit;

        //! the island number
        int islandNumber;

        //! the mean time (ns) of the hits on the island
        double meanTime;

        //! the minimum time of the hits on the island
        double minTime;

        //! the maximum time of the hits on the island
        double maxTime;

        //! the extracted initial time of the hits on the island
        double t0;

        //! algorithm use to extract t0
        T0Finder t0Type;

        //! the tracker station which the island is located
        int station;

        //! Default constructor
        StrawTimeIslandArtRecord() 
            : strawDigits()
            , upstreamDigit()
            , islandNumber(0)
            , meanTime(0.)
            , minTime(0.)
            , maxTime(0.)
            , t0(0.)
            , t0Type()
            , station(0)
         {}

        //! A nicer constructor: ROOT doesn't see it, but other classes can.
#ifndef __GCCXML__

        //! Copy constructor
        StrawTimeIslandArtRecord(const StrawTimeIslandArtRecord& island) 
            : strawDigits(island.strawDigits)
            , upstreamDigit(island.upstreamDigit)
            , islandNumber(island.islandNumber)
            , meanTime(island.meanTime)
            , minTime(island.minTime)
            , maxTime(island.maxTime)
            , t0(island.t0)
            , t0Type(island.t0Type)
            , station(island.station)
        {}

        StrawTimeIslandArtRecord(const StrawTimeIslandArtRecord& island, double time, T0Finder type)
            : StrawTimeIslandArtRecord(island)
        {
            t0     = time;
            t0Type = type;
        }

        //! special functions so that objects can be compared
        friend bool operator==(const StrawTimeIslandArtRecord& left, const StrawTimeIslandArtRecord& right) {
          if( left.strawDigits  == right.strawDigits  &&
              left.islandNumber == right.islandNumber &&
              left.meanTime     == right.meanTime     &&
              left.minTime      == right.minTime      &&
              left.maxTime      == right.maxTime      &&
              left.t0           == right.t0           &&
              left.t0Type       == right.t0Type       && 
              left.station      == right.station ) return true;
           return false;
        }


#endif // __GCCXML__

    }; // StrawTimeIslandArtRecord struct definition

    typedef std::vector<StrawTimeIslandArtRecord> StrawTimeIslandArtRecordCollection;
    typedef std::vector< art::Ptr<StrawTimeIslandArtRecord> > StrawTimeIslandPtrCollection;
    
} // namespace

#endif

