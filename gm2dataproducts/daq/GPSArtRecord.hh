//Meinberg GPS trigger info (from GPS0 bank)
//Tom Stuttard (4th July 2016)

#ifndef GPSArtRecord_hh
#define GPSArtRecord_hh

namespace gm2common {

  struct GPSArtRecord {

    unsigned int triggerNumber;

    unsigned int unixTimeRecv; //Unix time that trigger was received by Meinberg [s]
    unsigned int unixTimeRecvFraction; //Binary fraction to second since last unix time second that trigger was received by Meinberg [s]

    unsigned int unixTimeFE; //Unix time that trigger was processed by MasterGM2 frontend [s]
    unsigned int unixTimeFEFraction; //Binary fraction to second since last unix time second that trigger was processed by MasterGM2 frontend [s]

    //Default constructor
    GPSArtRecord()
      : triggerNumber(0)
      , unixTimeRecv(0)
      , unixTimeRecvFraction(0.)
      , unixTimeFE(0)
      , unixTimeFEFraction(0.)
    {}

  }; //GPSArtRecord

} // namespace

#endif

