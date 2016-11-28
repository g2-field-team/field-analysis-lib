#ifndef GALILARTRECORD_HH
#define GALILARTRECORD_HH

/** @file GalilArtRecord.hh

  @Structure for Galil motion control 
  @Author Ran Hong , rhong@anl.gov
  @11/28/2016

 **/

#include <vector>
#include "canvas/Persistency/Common/Ptr.h"

namespace g2field {

  //Galil record
    struct GalilArtRecord {

	// gps clock
	unsigned long int TimeStamp;
	
	//Tensions
	int Tension_fish;
	int Tension_coax;

	//Positions
	int Pos_fish;
	int Pos_coax;
	int Pos_garage;

	//Velocities
	int Vel_fish;
	int Vel_coax;
	int Vel_garage;

	//Output Voltages
	int OutputV_fish;
	int OutputV_coax;
	int OutputV_garage;

	GalilArtRecord() :
	  TimeStamp(0),Tension_fish(0),Tension_coax(0),Pos_fish(0),Pos_coax(0),Pos_garage(0),\
	  Vel_fish(0),Vel_coax(0),Vel_garage(0),OutputV_fish(0),OutputV_coax(0),OutputV_garage(0),
      {}

	// ROOT doesn't need to know the rest
#ifndef __GCCXML__
	GalilArtRecord(unsigned long int T,int T1,int T2,int P1,int P2,int P3,int V1,int V2,int V3,int O1,int O2,int O3,) :
	  TimeStamp(T),Tension_fish(T1),Tension_coax(T2),Pos_fish(P1),Pos_coax(P2),Pos_garage(P3),\
	  Vel_fish(V1),Vel_coax(V2),Vel_garage(V3),OutputV_fish(O1),OutputV_coax(O2),OutputV_garage(O3),
      {}
#endif // __GCCXML__

    }; //end of GalilArtRecord struct

    typedef std::vector<GalilArtRecord> GalilArtRecordCollection;

} // end namespace g2field

#endif //  GALILARTRECORD_HH
