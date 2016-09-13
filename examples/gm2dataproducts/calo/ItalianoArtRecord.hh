#ifndef ITALIANOARTRECORD_HH
#define ITALIANOARTRECORD_HH

namespace gm2calo {
struct ItalianoArtRecord {
  // art::Ptr to the AMCHeader
  art::Ptr<AMCHeader> amcHeader;

  // art::Ptr to the RiderChannelHeader
  art::Ptr<RiderChannelHeader> riderChannelHeader;

  // Fill number
  int fillNum;

  // Calorimeter number
  int caloNum;

  // Island number
  int islandNum;

  // Crystal number
  int xtalNum;

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

  ItalianoArtRecord()
      : amcHeader(),
        riderChannelHeader(),
        fillNum(0),
        caloNum(0),
        islandNum(0),
        xtalNum(0),
        riderNum(0),
        riderChannelNum(0),
        utcaSlotNum(0),
        time(0.0),
        amplitude(0.0),
        area(0.0),
        pedestal(0.0) {}

// ROOT doesn't need to know the rest
#ifndef __GCCXML__
  ItalianoArtRecord(int f_n, int c_n, int i_n, int x_n, int r_n, int rc_n,
                    int u_n, double t, double amp, double area, double ped)
      : amcHeader(),
        riderChannelHeader(),
        fillNum(f_n),
        caloNum(c_n),
        islandNum(i_n),
        xtalNum(x_n),
        riderNum(r_n),
        riderChannelNum(rc_n),
        utcaSlotNum(u_n),
        time(t),
        amplitude(amp),
        area(area),
        pedestal(ped) {}
#endif  // __GCCXML__

};  // end of ItalianoArtRecord struct

typedef std::vector<ItalianoArtRecord> ItalianoArtRecordCollection;

}  // end namespace gm2calo

#endif
