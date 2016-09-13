#ifndef FREDINOARTRECORD_HH
#define FREDINOARTRECORD_HH

#include "gm2dataproducts/calo/DAQHeader.hh" 

namespace gm2aux {

struct FredinoArtRecord {

  // art::Ptr to the AMCHeader
  art::Ptr<gm2calo::AMCHeader> amcHeader;

  // art::Ptr to the RiderChannelHeader
  art::Ptr<gm2calo::RiderChannelHeader> riderChannelHeader;

  // Fill number
  int fillNum;

  // Detector/channel type (0 = fiber harp, 1 = sync, 3 = SciFi, 4 = t0 counter)
  int detectorType;

  // Island number
  int islandNum;

  // Fiber number
  int fiberNum;

  // Rider number (only for DAQ)
  int riderNum;

  // Rider Channel number (only for DAQ)
  int riderChannelNum;

  // uTCA slot number (only for DAQ)
  int utcaSlotNum;

  // time from the island fit
  double time;

  // energy-like parameters
  double amplitude;

  double area;

  // estimated pedestal
  double pedestal;

  FredinoArtRecord()
      : amcHeader(),
        riderChannelHeader(),
        fillNum(0),
        detectorType(0),
        islandNum(0),
        fiberNum(0),
        riderNum(0),
        riderChannelNum(0),
        utcaSlotNum(0),
        time(0.0),
        amplitude(0.0),
        area(0.0),
        pedestal(0.0) {}

// ROOT doesn't need to know the rest
#ifndef __GCCXML__
  FredinoArtRecord(int f_n, int d_t, int i_n, int fb_n, int r_n, int rc_n,
                    int u_n, double t, double amp, double area, double ped)
      : amcHeader(),
        riderChannelHeader(),
        fillNum(f_n),
        detectorType(d_t),
        islandNum(i_n),
        fiberNum(fb_n),
        riderNum(r_n),
        riderChannelNum(rc_n),
        utcaSlotNum(u_n),
        time(t),
        amplitude(amp),
        area(area),
        pedestal(ped) {}
#endif  // __GCCXML__

};  // end of FredinoArtRecord struct

typedef std::vector<FredinoArtRecord> FredinoArtRecordCollection;


struct FredinoIslandArtRecord {

  // Fill number
  int fillNum;

  // Island number in a single fill
  int islandNum;

  // Sample number of the first sample in each island (e.g. time in clocks since start of fill)
  int firstSampleNum;

  // Number of samples in the islands
  int length;

  // The "hits", e.g. waveforms for a given channel;
  FredinoArtRecordCollection harpHits; //This is the fiber harp
  FredinoArtRecordCollection unusedChannelHits; //These are unused rider channels
  FredinoArtRecordCollection syncLineHits; //This is the sync line
  FredinoArtRecordCollection sciFiHits; //This is the SciFi SLAC detector
  FredinoArtRecordCollection t0CounterHits; //This is the t0 counter

  FredinoIslandArtRecord() :
    fillNum(0), 
    islandNum(0), 
    firstSampleNum(0), 
    length(0), 
    harpHits(), 
    unusedChannelHits(), 
    syncLineHits(), 
    sciFiHits(), 
    t0CounterHits()
  {}

  virtual ~FredinoIslandArtRecord(){};

  // ROOT doesn't need to know the rest
#ifndef __GCCXML__
  FredinoIslandArtRecord(int f_n, int i_n, double i_t, int s_n ,int len, const FredinoArtRecordCollection & h_h, 
    const FredinoArtRecordCollection & u_h, const FredinoArtRecordCollection & s_h, 
    const FredinoArtRecordCollection & sf_h, const FredinoArtRecordCollection & sf_t) :
      fillNum(f_n), 
      islandNum(i_n), 
      firstSampleNum(s_n), 
      length(len), 
      harpHits(h_h), 
      unusedChannelHits(u_h), 
      syncLineHits(s_h), 
      sciFiHits(sf_h), 
      t0CounterHits(sf_t)
  {}
#endif // __GCCXML__

}; //end of FredinoIslandArtRecord struct

typedef std::vector<FredinoIslandArtRecord> FredinoIslandArtRecordCollection;

}  // end namespace gm2aux

#endif
