#ifndef CLUSTERARTRECORD_HH
#define CLUSTERARTRECORD_HH

/** @file ClusterArtRecord.hh

  @Structure for ring cluster per fill art record
  @Author Kim Siang Khaw, khaw84@uw.edu
  @2016/01/26

 **/

#include <vector>
#include "CrystalHitArtRecord.hh"

namespace gm2calo {

    struct ClusterArtRecord {

        // Fill number
        int fillNum;

        // Calorimeter number
        int caloNum;

        // Island number
        int islandNum;

        // Time of the calo cluster
        double time;

        // Energy of the calo cluster
        double energy;

        // position information variables
        double x, y;

        // Vector of crystal hits contributed to this calo cluster
        CrystalHitArtRecordCollection crystalHits;

        ClusterArtRecord() :
            fillNum(0), caloNum(0), islandNum(0), 
            time(0.0), energy(0.0), x(0), y(0), crystalHits()
        {}

        virtual ~ClusterArtRecord(){};

        // ROOT doesn't need to know the rest
#ifndef __GCCXML__
        ClusterArtRecord(int f_n, int c_n, int i_n, 
                double t, double ene, CrystalHitArtRecordCollection cH ) :
            fillNum(f_n), caloNum(c_n), islandNum(i_n), 
            time(t), energy(ene), x(0), y(0), crystalHits(cH)
        {}
#endif // __GCCXML__

    }; //end of ClusterArtRecord struct

    typedef std::vector<ClusterArtRecord> ClusterArtRecordCollection;

    struct CaloClusterArtRecord {

        // Fill number
        int fillNum;

        // Calorimeter number
        int caloNum;

        // Vector of clusters in this calorimeter
        ClusterArtRecordCollection clusters;

        CaloClusterArtRecord() :
            fillNum(0), caloNum(0), clusters() 
        {}

        virtual ~CaloClusterArtRecord(){};

        // ROOT doesn't need to know the rest
#ifndef __GCCXML__
        CaloClusterArtRecord(int f_n, int c_n,
                ClusterArtRecordCollection clus ) :
            fillNum(f_n), caloNum(c_n), clusters(clus) 
        {}
#endif // __GCCXML__

    }; //end of CaloClusterArtRecord struct

    typedef std::vector<CaloClusterArtRecord> CaloClusterArtRecordCollection;

} // end namespace gm2calo

#endif //  CLUSTERARTRECORD_HH
