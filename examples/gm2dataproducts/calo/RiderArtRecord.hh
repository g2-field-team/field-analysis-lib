#ifndef RIDERARTRECORD_HH
#define RIDERARTRECORD_HH

/** @file RiderArtRecord.hh

  @Structure for 24 calos per fill art record
  @Author Kim Siang Khaw, khaw84@uw.edu
  @2016/01/21

 **/

#include <vector>
#include "art/Persistency/Common/Ptr.h"
#include "gm2dataproducts/calo/DAQHeader.hh"

namespace gm2calo {

    struct RiderArtRecord {

	// art::Ptr to the AMCHeader
	art::Ptr<AMCHeader> amcHeader;

	// art::Ptr to the RiderChannelHeader
	art::Ptr<RiderChannelHeader> riderChannelHeader;

	// Fill number
	unsigned int fillNum;

	// Calorimeter number
	unsigned int caloNum;

	// Crystal number
	unsigned int xtalNum;

	// Rider number (only for DAQ)
	unsigned int riderNum;

	// Rider Channel number (only for DAQ)
	unsigned int riderChannelNum;

	// uTCA slot number (only for DAQ)
	unsigned int utcaSlotNum;

	// clock counter of this rider (only for DAQ, [BXid, OrN])
	long clockCounter;

	// trigger number of this rider waveform (only for DAQ)
	unsigned int triggerNum;

	// Number of samples in the rider waveform
	unsigned int length;

	// ADC count of pedestal level
	int pedestalLevel;

	// ADC count of each sample
	std::vector<short> trace;

	RiderArtRecord() :
	    amcHeader(), riderChannelHeader(), fillNum(0), caloNum(0), xtalNum(0), riderNum(0), 
	    riderChannelNum(0), utcaSlotNum(0), clockCounter(0), triggerNum(0), length(0), pedestalLevel(0), trace()
	{}

	virtual ~RiderArtRecord(){};

	// ROOT doesn't need to know the rest
#ifndef __GCCXML__
	RiderArtRecord(unsigned int f_n, unsigned int c_n, unsigned int x_n, unsigned int r_n,  unsigned int rc_n, 
		unsigned int u_n, long c_c, unsigned int t_n, unsigned int len, int ped, std::vector<short> tr) :
	    amcHeader(), riderChannelHeader(), fillNum(f_n), caloNum(c_n), xtalNum(x_n), 
	    riderNum(r_n), riderChannelNum(rc_n), utcaSlotNum(u_n), clockCounter(c_c),  triggerNum(t_n), length(len), pedestalLevel(ped), trace(tr)
	{}
#endif // __GCCXML__

    }; //end of RiderArtRecord struct

    typedef std::vector<RiderArtRecord> RiderArtRecordCollection;

    struct CaloRiderArtRecord {

	// Fill number
	unsigned int fillNum;

	// Calorimeter number
	unsigned int caloNum;

	// Number of samples in the rider waveform
	unsigned int length;

	// The rider matrix
	RiderArtRecordCollection riders;

	CaloRiderArtRecord() :
	    fillNum(0), caloNum(0),  length(0), riders()
	{}

	virtual ~CaloRiderArtRecord(){};

	// ROOT doesn't need to know the rest
#ifndef __GCCXML__
	CaloRiderArtRecord(unsigned int f_n, unsigned int c_n, unsigned int len, 
		RiderArtRecordCollection rs) :
	    fillNum(f_n), caloNum(c_n), length(len), riders(rs)
	{}
#endif // __GCCXML__

    }; //end of CaloRiderArtRecord struct

    typedef std::vector<CaloRiderArtRecord> CaloRiderArtRecordCollection;

} // end namespace gm2calo

#endif //  RIDERARTRECORD_HH
