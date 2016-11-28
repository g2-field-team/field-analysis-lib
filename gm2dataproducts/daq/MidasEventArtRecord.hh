#ifndef MidasEventArtRecord_hh
#define MidasEventArtRecord_hh

#include <string>
#include <sstream>
#include <iomanip>
#include "art/Utilities/Exception.h"

//Define length of a MIDAS bank name
const unsigned int MIDAS_BANK_NAME_LENGTH = 4;


//Macro to define a bank
//
//Has functions:
//  string GetBankName() : Returns bank name (may be partial name is numbered bank)
//  bool CompareBankName(string) : Check if bank name matches supplied string (accounts for numbering)
//
//
#define DEFINE_BANK(BankName,BankMidasID,BankType,MidasBankType) \
struct BankName { \
\
typedef BankType WordType; \
typedef MidasBankType MidasWordType; \
\
static std::string GetBankName() { return #BankMidasID; } \
\
static std::string GetBankName(unsigned int bankNumber) { \
if ( GetBankName().size() >= MIDAS_BANK_NAME_LENGTH ) { \
throw art::Exception(art::errors::InvalidNumber) << "MIDAS bank \"" << GetBankName() \
<< "\" not numbered, cannot get name by number"; \
return ""; \
} \
else { \
std::stringstream ss; \
ss << GetBankName() << std::setfill('0') << std::setw(MIDAS_BANK_NAME_LENGTH - GetBankName().size()) << bankNumber; \
return ss.str(); \
} \
} \
\
static bool CompareBankName(const std::string& s) { \
std::string name = GetBankName(); \
if ( name.size() < MIDAS_BANK_NAME_LENGTH ) { return name == s.substr(0,name.size());  } \
else { return s == name; } \
} \
\
} ;


namespace gm2midastoart {

  //
  // Art record
  //

  template<typename D> //Detector bank
  struct MidasEventArtRecord {

    //std::name

    //Store each word in vector this this MIDAS bank event
    std::vector< typename D::WordType > words;

    //Index (if numbered bank)
    int index;

    //Default constructor
    MidasEventArtRecord()
      : words()
      , index(-1)
    {}

    //A nicer constructor: ROOT doesn't see it, but other classes can.
#ifndef __GCCXML__
    MidasEventArtRecord(typename D::WordType * firstWordPtr_, unsigned int numWords_,  int index_=-1)
      : words( firstWordPtr_, firstWordPtr_ + numWords_ ) //Minimise memory copies using vector constructor
      , index(index_)
    {}
#endif // __GCCXML__

  }; // MidasEventArtRecord struct definition


  //
  // Individual detector bank definitions 
  //

  //DEFINE_BANK args are: ( Class name, MIDAS bank 4 character name, Bank word type, MIDAS bank word type)

  //MIDAS trigger
  DEFINE_BANK(MasterTriggerBank,TRIG,uint32_t,uint32_t)
  DEFINE_BANK(GPSBank,GPS0,uint32_t,uint32_t)

  //Tracker
  DEFINE_BANK(TrackerBank,STRW,uint64_t,uint32_t) //64-bit AMC13 data written to 32-bit MIDAS bank
  DEFINE_BANK(TrackerGLIBBank,STRG,uint32_t,uint32_t) //When using GLIB for straw readout
  DEFINE_BANK(TrackerSlowBank,PRSD,uint32_t,uint32_t)

  //Calorimeter
  DEFINE_BANK(CaloHeaderBank,CB,uint32_t,uint32_t)
  DEFINE_BANK(CaloRawBank,CR,int16_t,int16_t)
  DEFINE_BANK(CaloTBank,CT,int16_t,int16_t)
  DEFINE_BANK(CaloCQBank,CQ,int16_t,int16_t)
  DEFINE_BANK(CaloCHBank,CH,uint32_t,uint32_t)
  DEFINE_BANK(CaloCPBank,CP,int16_t,int16_t)
  DEFINE_BANK(GalilBank,GALI,uint32_t,uint32_t)

} // namespace

#endif

