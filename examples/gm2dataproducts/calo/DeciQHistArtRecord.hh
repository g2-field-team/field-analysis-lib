#ifndef DECIQHISTARTRECORD_HH
#define DECIQHISTARTRECORD_HH

/** @file DeciQHistArtRecord.hh

  @Structure for 24 calos per fill art record
  @Author Kim Siang Khaw, khaw84@uw.edu
  @2016/01/21

 **/

#include <vector>

namespace gm2calo {

    struct CaloDeciQHistArtRecord {

        // Fill number
        int fillNum;

        // Calorimeter number
        int caloNum;

    	// Number of samples in the qhist waveform
        int length;

        // ADC count of pedestal level (only for simulation)
        int pedestalLevel;

        // decimation value
        int decimation;

        // ADC count of each sample
        std::vector<short> qhist;

        CaloDeciQHistArtRecord() :
            fillNum(0), caloNum(0), length(0), pedestalLevel(0), decimation(32), qhist()
        {}

        virtual ~CaloDeciQHistArtRecord(){};

        // ROOT doesn't need to know the rest
#ifndef __GCCXML__
        CaloDeciQHistArtRecord(int f_n, int c_n, int len, int ped, int deci, std::vector<short> qh) :
            fillNum(f_n), caloNum(c_n), length(len), pedestalLevel(ped), decimation(deci), qhist(qh)
        {}
#endif // __GCCXML__

    }; //end of CaloDeciQHistArtRecord struct

    typedef std::vector<CaloDeciQHistArtRecord> CaloDeciQHistArtRecordCollection;

} // end namespace gm2calo

#endif //  DECIQHISTARTRECORD_HH
