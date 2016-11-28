#ifndef TROLLEYARTRECORD_HH
#define TROLLEYARTRECORD_HH

/** @file TrolleyArtRecord.hh

  @Structure for trolley per NMR measurement
  @Author Ran Hong , rhong@anl.gov
  @11/28/2016

 **/

#include <vector>
#include "canvas/Persistency/Common/Ptr.h"

namespace g2field {

  //Trolley NMR
    struct TrolleyNMRArtRecord {

	// gps clock
	unsigned long int gps_clock;

	//probe index
	unsigned short probe_index;

	//trace length
	unsigned short length;

	//waveform trace
	std::vector<short> trace;

	TrolleyNMRArtRecord() :
	  gps_clock(0),probe_index(0),length(0),trace()
      {}

	// ROOT doesn't need to know the rest
#ifndef __GCCXML__
	TrolleyNMRArtRecord(unsigned long int clock, unsigned short prb_index, unsigned short len, std::vector<short> tr) :
	  gps_clock(clock),probe_index(prb_index),length(len),trace(tr)
      {}
#endif // __GCCXML__

    }; //end of TrolleyArtRecord struct

    typedef std::vector<TrolleyNMRArtRecord> TrolleyNMRArtRecordCollection;

    //Trolley Barcode
    struct TrolleyBarcodeArtRecord {

      // gps clock
      unsigned long int gps_clock;

      //trace length per channel
      unsigned short length_per_ch;

      //waveform traces
      std::vector<std::vector<short>> traces;

      TrolleyBarcodeArtRecord() :
	gps_clock(0),length_per_ch(0),traces()
      {}

      // ROOT doesn't need to know the rest
#ifndef __GCCXML__
      TrolleyBarcodeArtRecord(unsigned long int clock, unsigned short len, std::vector<std::vector<short>> tr) :
	gps_clock(clock),length_per_ch(len),traces(tr)
      {}
#endif // __GCCXML__

    }; //end of TrolleyArtRecord struct

    typedef std::vector<TrolleyMonitorArtRecord> TrolleyMonitorArtRecordCollection;

    //Trolley Monitor
    struct TrolleyMonitorArtRecord {

      // gps clock
      unsigned long int gps_clock_cycle_start;

      //Pressure monitor
      unsigned int PMonitorVal;

      //Pressure monitor temperature
      unsigned int PMonitorTemp;

      //RF Power 1
      unsigned int RFPower1;

      //RF Power 2
      unsigned int RFPower2;

      //NMR Check sum
      unsigned int NMRCheckSum;

      //Frame Check sum
      unsigned int FrameCheckSum;

      //Frame Sum
      unsigned int FrameSum;

      //Frame Index
      unsigned int FrameIndex;

      //Status Bits
      unsigned short StatusBits;

      //Temperature monitors
      unsigned short TMonitorIn;
      unsigned short TMonitorExt1;
      unsigned short TMonitorExt2;
      unsigned short TMonitorExt3;

      //Voltage extrema
      unsigned short V1Min;
      unsigned short V1Max;
      unsigned short V2Min;
      unsigned short V2Max;

      //voltage trace length per channel
      unsigned short length_per_ch;

      //voltage monitor traces
      std::vector<unsigned short> trace_VMonitor1;
      std::vector<unsigned short> trace_VMonitor2;

      TrolleyMonitorArtRecord() :
	gps_clock_cycle_start(0),PMonitorVal(0),PMonitorTemp(0),RFPower1(0),RFPower2(0),NMRCheckSum(0),FrameCheckSum(0),\
	FrameSum(0),FrameIndex(0),StatusBits(0),TMonitorIn(0),TMonitorExt1(0),TMonitorExt2(0),TMonitorExt3(0),\
	V1Min(0),V1Max(0),V2Min(0),V2Max(0),length_per_ch(0),trace_VMonitor1(),trace_VMonitor2()
      {}

      // ROOT doesn't need to know the rest
#ifndef __GCCXML__
      TrolleyMonitorArtRecord(unsigned long int gpc_clock,unsigned int PVal,unsigned int PTemp,unsigned int RF1,unsigned int RF2,unsigned int NMRSumChk,unsigned int FrameSumChk,unsigned int FrameSumVal,unsigned int FIndex,unsigned short Status,unsigned short TIn,unsigned short TExt1,unsigned short TExt2,unsigned short TExt3,unsigned short V1m,unsigned short V1M,unsigned short V2m,unsigned short V2M,unsigned short len,unsigned short trace1,unsigned short trace2
	  ) :
	gps_clock_cycle_start(gps_clock),PMonitorVal(PVal),PMonitorTemp(PTemp),RFPower1(RF1),RFPower2(RF2),NMRCheckSum(NMRSumChk),FrameCheckSum(FrameSumChk),\
	FrameSum(FrameSumVal),FrameIndex(FIndex),StatusBits(Status),TMonitorIn(TIn),TMonitorExt1(TExt1),TMonitorExt2(TExt2),TMonitorExt3(TExt3),\
	V1Min(V1m),V1Max(V1M),V2Min(V2m),V2Max(V2M),length_per_ch(len),trace_VMonitor1(trace1),trace_VMonitor2(trace2)
      {}
#endif // __GCCXML__

    }; //end of TrolleyArtRecord struct

    typedef std::vector<TrolleyMonitorArtRecord> TrolleyMonitorArtRecordCollection;

} // end namespace g2field

#endif //  TROLLEYARTRECORD_HH
