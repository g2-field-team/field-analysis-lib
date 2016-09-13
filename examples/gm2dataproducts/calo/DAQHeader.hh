#ifndef DAQHEADERARTRECORD_HH
#define DAQHEADERARTRECORD_HH

/** @file DAQHeaderArtRecord.hh

  @ Structure for storing DAQ Header 
  @Author Kim Siang Khaw, khaw84@uw.edu
  @2016/05/16

 **/

#include <vector>

namespace gm2calo {

    struct CDFHeader{

	// Crate Number
	unsigned int crateNum;

	// FOV
	unsigned int FOV;

	// Source_id(12)
	unsigned int sourceID;

	// LV1_id(24)
	unsigned int lv1ID;

	// Evt_ty;
	unsigned int eventType; 

	// OrN(32)
	unsigned long clockCounter;

	// nAMC
	unsigned int nAMC;

	CDFHeader() :
	    crateNum(0), FOV(0), sourceID(0),  lv1ID(0), eventType(0), clockCounter(0), nAMC(0) {}

	virtual ~CDFHeader() {};

	// ROOT doesn't need to know the rest
#ifndef __GCCXML__
	CDFHeader(unsigned int c_n, unsigned int fov,  unsigned int s_i, unsigned int l_i, unsigned int e_t,
		unsigned long c_c, unsigned int n_a) :
	    crateNum(c_n), FOV(fov), sourceID(s_i), lv1ID(l_i), eventType(e_t), clockCounter(c_c), nAMC(n_a)	
	{}
#endif // __GCCXML__
    };

    typedef std::vector<CDFHeader> CDFHeaderCollection;

    struct AMCHeader{

	// uTCA slot number (AMCNo) 
	unsigned int amcNum;

	// trigger number of this rider 
	unsigned int triggerNum;

	// clock counter of this rider ([BXid, OrN])
	unsigned long clockCounter;

	// fill type of this event
	unsigned int fillType;

	// board type of this amc (rider, ccc fc7. tracker fc7)
	unsigned int boardType;

	// board ID
	unsigned int boardID;

	AMCHeader() :
	    amcNum(0), triggerNum(0), clockCounter(0), fillType(0), boardType(0), boardID(0) {}

	virtual ~AMCHeader(){};

	// ROOT doesn't need to know the rest
#ifndef __GCCXML__
	AMCHeader(unsigned int a_n,  unsigned int t_n, unsigned long c_c, unsigned int f_t, unsigned int b_t, unsigned int b_i) :
	    amcNum(a_n), triggerNum(t_n), clockCounter(c_c), fillType(f_t), boardType(b_t), boardID(b_i)
	{}
#endif // __GCCXML__

    };

    typedef std::vector<AMCHeader> AMCHeaderCollection;

    struct FC7Header{

	// uTCA slot number (AMCNo) 
	unsigned int amcNum;

	// trigger number of this rider 
	unsigned int triggerNum;

	// clock counter of this rider ([BXid, OrN])
	unsigned long clockCounter;

	// fill type of this event
	unsigned int fillType;

	// board type of this amc (rider, ccc fc7. tracker fc7)
	unsigned int boardType;

	// board ID
	unsigned int boardID;

	FC7Header() :
	    amcNum(0), triggerNum(0), clockCounter(0), fillType(0), boardType(0), boardID(0) {}

	virtual ~FC7Header(){};

	// ROOT doesn't need to know the rest
#ifndef __GCCXML__
	FC7Header(unsigned int a_n,  unsigned int t_n, unsigned long c_c, unsigned int f_t, unsigned int b_t, unsigned int b_i) :
	    amcNum(a_n), triggerNum(t_n), clockCounter(c_c), fillType(f_t), boardType(b_t), boardID(b_i)
	{}
#endif // __GCCXML__

    };

    typedef std::vector<FC7Header> FC7HeaderCollection;



    struct RiderChannelHeader{

	// 12 bit Waveform Count
	unsigned int waveformCount;

	// 22 bit Waveform Gap
	unsigned int waveformGap;

	// 16 bit Channel Tag
	unsigned int channelTag;

	// 24 bit Trigger Number
	unsigned int triggerNum; 

	// 3 bit Fill Type
	unsigned int fillType;

	// 23 bit Waveform Length
	unsigned int waveformLength;

	RiderChannelHeader() :
	    waveformCount(0), waveformGap(0), channelTag(0), triggerNum(0), fillType(0), waveformLength(0) {}

	virtual ~RiderChannelHeader(){};

	// ROOT doesn't need to know the rest
#ifndef __GCCXML__
	RiderChannelHeader(unsigned int w_c,  unsigned int w_g, unsigned int c_t, unsigned int t_n, unsigned int f_t, unsigned int w_l ) :
	    waveformCount(w_c), waveformGap(w_g), channelTag(c_t), triggerNum(t_n), fillType(f_t), waveformLength(w_l)
	{}
#endif // __GCCXML__
    };

    typedef std::vector<RiderChannelHeader> RiderChannelHeaderCollection;

} // end of gm2calo namespace


#endif // DAQHEADERARTRECORD_HH
