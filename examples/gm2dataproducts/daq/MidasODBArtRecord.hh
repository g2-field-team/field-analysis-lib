#ifndef MIDASODBARTRECORD_HH
#define MIDASODBARTRECORD_HH

/** @file MidasODBArtRecord.hh

  @Structure for storing MIDAS ODB
  @Author Kim Siang Khaw, khaw84@uw.edu
  @2016/06/27

 **/

namespace gm2midastoart {

    struct MidasODBArtRecord {

        // Run number
        unsigned int runNum;

        // XML dump stored in a std::string 
        std::string odb_string;

        MidasODBArtRecord() :
            runNum(), odb_string() 
        {}

        virtual ~MidasODBArtRecord(){};

        // ROOT doesn't need to know the rest
#ifndef __GCCXML__
        MidasODBArtRecord(unsigned int r_n, std::string o_s) :
            runNum(r_n), odb_string(o_s)
        {}
#endif // __GCCXML__

    }; //end of MidasODBArtRecord struct

    typedef std::vector<MidasODBArtRecord> MidasODBArtRecordCollection;

} // end namespace gm2midastoart

#endif //  MIDASODBARTRECORD_HH
