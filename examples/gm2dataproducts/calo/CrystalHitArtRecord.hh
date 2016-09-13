#ifndef CRYSTALHITARTRECORD_HH
#define CRYSTALHITARTRECORD_HH

/** @file CrystalHitArtRecord.hh

  @Structure for 24 calos per fill art record
  @Author Kim Siang Khaw, khaw84@uw.edu
  @2016/01/21

 **/

#include <vector>
#include "art/Persistency/Common/Ptr.h"
#include "gm2dataproducts/calo/DAQHeader.hh"

namespace gm2calo {

    struct CrystalHitArtRecord {

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

	// time of the crystal hit
	double time;

	// energy of the crystal hit
	double energy;

	CrystalHitArtRecord() :
	    amcHeader(), riderChannelHeader(), fillNum(0), caloNum(0), islandNum(0), xtalNum(0),
	    riderNum(0),riderChannelNum(0), utcaSlotNum(0), time(0.0), energy(0.0)
	{}

	virtual ~CrystalHitArtRecord(){};

	// ROOT doesn't need to know the rest
#ifndef __GCCXML__
	CrystalHitArtRecord(int f_n, int c_n, int i_n, int x_n, int r_n,  int rc_n, 
		int u_n, double t, double ene) :
	    amcHeader(), riderChannelHeader(), fillNum(f_n), caloNum(c_n), islandNum(i_n), xtalNum(x_n), 
	    riderNum(r_n), riderChannelNum(rc_n), utcaSlotNum(u_n), time(t), energy(ene) 
	{}
#endif // __GCCXML__

    }; //end of CrystalHitArtRecord struct

    typedef std::vector<CrystalHitArtRecord> CrystalHitArtRecordCollection;

    struct MatrixCrystalHitArtRecord {

	// Fill number
	int fillNum;

	// Calorimeter number
	int caloNum;

	// Island number in a single fill
	int islandNum;

	// The island matrix
	CrystalHitArtRecordCollection crystalHits;

	MatrixCrystalHitArtRecord() :
	    fillNum(0), caloNum(0), islandNum(0), crystalHits()
	{}

	virtual ~MatrixCrystalHitArtRecord(){};

	// ROOT doesn't need to know the rest
#ifndef __GCCXML__
	MatrixCrystalHitArtRecord(int f_n, int c_n, int i_n,
		CrystalHitArtRecordCollection chs) :
	    fillNum(f_n), caloNum(c_n), islandNum(i_n), crystalHits(chs)
	{}
#endif // __GCCXML__

    }; //end of MatrixCrystalHitArtRecord struct

    typedef std::vector<MatrixCrystalHitArtRecord> MatrixCrystalHitArtRecordCollection;

    struct CaloCrystalHitArtRecord {

	// Fill number
	int fillNum;

	// Calorimeter number
	int caloNum;

	// Vector of CrystalHitMatrices, one for each island this fill
	MatrixCrystalHitArtRecordCollection matrixCrystalHits;

	CaloCrystalHitArtRecord():
	    fillNum(0), caloNum(0),  matrixCrystalHits()
	{}

	virtual ~CaloCrystalHitArtRecord(){};

	// ROOT doesn't need to know the rest
#ifndef __GCCXML__
	CaloCrystalHitArtRecord(int f_n, int c_n, 
		MatrixCrystalHitArtRecordCollection mch) :
	    fillNum(f_n), caloNum(c_n), matrixCrystalHits(mch)
	{}
#endif // __GCCXML__

    }; // end of CaloCrystalHitArtRecord struct

    typedef std::vector<CaloCrystalHitArtRecord> CaloCrystalHitArtRecordCollection;

} // end namespace gm2calo

#endif //  CRYSTALHITARTRECORD_HH
