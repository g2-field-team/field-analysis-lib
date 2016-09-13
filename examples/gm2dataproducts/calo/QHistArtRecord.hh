#ifndef QHISTARTRECORD_HH
#define QHISTARTRECORD_HH

/** @file QHistArtRecord.hh

  @Structure for 24 calos per fill art record
  @Author Kim Siang Khaw, khaw84@uw.edu
  @2016/01/21

 **/

#include <vector>
#include "art/Persistency/Common/Ptr.h"
#include "gm2dataproducts/calo/DAQHeader.hh"

namespace gm2calo {

    struct QHistArtRecord {

        // art::Ptr to the AMCHeader
        art::Ptr<AMCHeader> amcHeader;

        // art::Ptr to the RiderChannelHeader
        art::Ptr<RiderChannelHeader> riderChannelHeader;

        // Fill number
        int fillNum;

        // Calorimeter number
        int caloNum;

        // Crystal number
        int xtalNum;

        // Rider number (only for DAQ)
        int riderNum;

        // Rider Channel number (only for DAQ)
        int riderChannelNum;

        // uTCA slot number (only for DAQ)
        int utcaSlotNum;

        // Sample number of the first sample in the qhist waveform 
        int firstSampleNum;

        // Number of samples in the qhist waveform
        int length;

        // ADC count of pedestal level (only for simulation)
        int pedestalLevel;

        // ADC count of each sample
        std::vector<unsigned int> trace;

        QHistArtRecord() :
            amcHeader(), riderChannelHeader(), fillNum(0), caloNum(0), xtalNum(0), riderNum(0), 
            riderChannelNum(0), utcaSlotNum(0), firstSampleNum(0), length(0), pedestalLevel(0), trace()
        {}

        virtual ~QHistArtRecord(){};

        // ROOT doesn't need to know the rest
#ifndef __GCCXML__
        QHistArtRecord(int f_n, int c_n, int x_n, int r_n,  int rc_n, 
                int u_n, int s_n, int len, int ped, std::vector<unsigned int> tr) :
            amcHeader(), riderChannelHeader(), fillNum(f_n), caloNum(c_n), xtalNum(x_n), 
            riderNum(r_n), riderChannelNum(rc_n), utcaSlotNum(u_n), firstSampleNum(s_n), length(len), pedestalLevel(ped), trace(tr)
        {}
#endif // __GCCXML__

    }; //end of QHistArtRecord struct

    typedef std::vector<QHistArtRecord> QHistArtRecordCollection;

    struct CaloQHistArtRecord {

        // Fill number
        int fillNum;

        // Calorimeter number
        int caloNum;

        // Sample number of the first sample in each qhist waveform
        int firstSampleNum;

        // Number of samples in the qhist waveform
        int length;

        // The qhist matrix
        QHistArtRecordCollection qHists;

        CaloQHistArtRecord() :
            fillNum(0), caloNum(0), firstSampleNum(0), length(0), qHists()
        {}

        virtual ~CaloQHistArtRecord(){};

        // ROOT doesn't need to know the rest
#ifndef __GCCXML__
        CaloQHistArtRecord(int f_n, int c_n, int s_n ,int len, 
                QHistArtRecordCollection rs) :
            fillNum(f_n), caloNum(c_n),  firstSampleNum(s_n), length(len), qHists(rs)
        {}
#endif // __GCCXML__

    }; //end of CaloQHistArtRecord struct

    typedef std::vector<CaloQHistArtRecord> CaloQHistArtRecordCollection;

} // end namespace gm2calo

#endif //  QHISTARTRECORD_HH
