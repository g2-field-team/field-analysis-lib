//MasterGM2 trigger info (from TRIG bank)
//Tom Stuttard (4th July 2016)

#ifndef TriggerArtRecord_hh
#define TriggerArtRecord_hh

namespace gm2common {

  struct TriggerArtRecord {

    unsigned int triggerNumber;
    unsigned int triggerMask;

    //TODO Add more variable, this is a palceholder

    //Default constructor
    TriggerArtRecord()
      : triggerNumber(0)
      , triggerMask(0)
    {}

  }; //TriggerArtRecord

} // namespace

#endif


