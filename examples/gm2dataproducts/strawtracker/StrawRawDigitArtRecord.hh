//A single straw raw digit as unpacked from MIDAS data
//First stage of unpacking produces blocks of data per TDC. This stage is that information reorganised into "hits"
//
//Next step for this guy is to fill a StraDigitArtRecord. At this point, analysis can be run 
//on either simulation or real data.

#ifndef StrawRawDigitArtRecord_hh
#define StrawRawDigitArtRecord_hh

#include <vector>
#include "art/Persistency/Common/PtrVector.h"

namespace gm2strawtracker {

  struct StrawRawDigitArtRecord {

    //Define a sub-struct representing the hierarchical channel ID
    struct ChannelID {
      ChannelID() : amc13Num(0), fc7Num(0), logicBoardNum(0), tdcNum(), channelNum(0) {}
      ChannelID(unsigned short a, unsigned short f, unsigned short l, unsigned short t, unsigned short c)
        : amc13Num(a), fc7Num(f), logicBoardNum(l), tdcNum(t), channelNum(c) {}
      unsigned short amc13Num;
      unsigned short fc7Num;
      unsigned short logicBoardNum;
      unsigned short tdcNum;
      unsigned short channelNum; //e.g. ASDQ channel, so straw/wire
    };

    //Unique (within fill) hit id
    unsigned int uid;

    //Electronics channel
    ChannelID channelID;

    //Raw hit info
    double tdcTime; //Raw TDC hit time [ns] (combines find and course time bits)
    bool bothEdges; //If you record a hit on both the rising and falling edge of the pulse (otherwise just rising)
    double width; //Width of pulse [ns] (if bothEdges is set), as recorded by ASDQ/TDC (not true width, but proxy for signal size)

    //Trigger info
    double triggerNum; //Input C5 trigger number [ns] (as recorded by TDC)
    double triggerTime; //Input C5 trigger time [ns] (as recorded by TDC)

    //Default constructor
    StrawRawDigitArtRecord()
      : uid(0)
      , channelID()
      , tdcTime(0.)
      , bothEdges(false)
      , width(0.)
      , triggerNum(0.)
      , triggerTime(0.)
    {}

  }; //StrawRawDigitArtRecord

  typedef std::vector<StrawRawDigitArtRecord> StrawRawDigitArtRecordCollection;
  typedef std::vector< art::Ptr<StrawRawDigitArtRecord> > StrawRawDigitPtrCollection;

} // namespace

#endif

