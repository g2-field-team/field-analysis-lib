#ifndef MidasEventHeaderArtRecord_hh
#define MidasEventHeaderArtRecord_hh

#include <string>

namespace gm2midastoart {

  struct MidasEventHeaderArtRecord {

    //The MIDAS event header (parsed from EVENT_HEADER in midas.h)
    //See here for details: https://midas.triumf.ca/MidasWiki/index.php/Event_Structure
    int16_t eventType; //event_id in EVENT_HEADER
    int16_t triggerMask; //trigger_mask in EVENT_HEADER
    uint32_t eventNumber; //serial_number in EVENT_HEADER + 1
    uint32_t timeStamp; //time_stamp in EVENT_HEADER
    uint32_t eventDataSize; //data_size in EVENT_HEADER [bytes]

    //The MIDAS global bank header (parsed from BANK_HEADER in midas.h)
    uint32_t allBankDataSize; //data_size in BANK_HEADER [bytes]
    uint32_t flags; //flags in BANK_HEADER

    //List of all banks parsed from the event
    //So this is all banks that the user specified to parse that were found in the event
    std::vector<std::string> banksList;

    //Default constructor
    MidasEventHeaderArtRecord()
      : eventType(0)
      , triggerMask(0)
      , eventNumber(0)
      , timeStamp(0)
      , eventDataSize(0)
      , allBankDataSize(0)
      , flags(0)
      , banksList()
    {}

    //Hide all other functions from ROOT
#ifndef __GCCXML__
 
#endif // __GCCXML__

  }; // MidasEventHeaderArtRecord struct definition

} // namespace

#endif

