#ifndef TRACKPARAMDEFS_HH
#define TRACKPARAMDEFS_HH

#include "Math/SMatrix.h"

namespace gm2strawtracker {
    
    typedef ROOT::Math::SMatrix<double,1> TrackMatrix1x1;
    typedef ROOT::Math::SMatrix<double,3> TrackMatrix3x3;
    typedef ROOT::Math::SMatrix<double,5> TrackMatrix5x5;
    typedef ROOT::Math::SMatrix<double,2> TrackMatrix2x2;
    typedef ROOT::Math::SMatrix<double,6> TrackMatrix6x6;
    typedef ROOT::Math::SMatrix<double,3,5> TrackMatrix3x5;
    typedef ROOT::Math::SMatrix<double,5,3> TrackMatrix5x3;
    typedef ROOT::Math::SMatrix<double,5,2> TrackMatrix5x2;
    typedef ROOT::Math::SMatrix<double,2,5> TrackMatrix2x5;
    typedef ROOT::Math::SMatrix<double,1,2> TrackMatrix1x2;
    typedef ROOT::Math::SMatrix<double,2,1> TrackMatrix2x1;
    typedef ROOT::Math::SMatrix<double,1,6> TrackMatrix1x6;
    typedef ROOT::Math::SMatrix<double,6,1> TrackMatrix6x1;
    typedef ROOT::Math::SMatrix<double,1,3> TrackMatrix1x3;
    typedef ROOT::Math::SMatrix<double,3,1> TrackMatrix3x1;

    typedef ROOT::Math::SVector<double,6> TrackVector6;    
    typedef ROOT::Math::SVector<double,5> TrackVector5;
    typedef ROOT::Math::SVector<double,3> TrackVector3;
    typedef ROOT::Math::SVector<double,2> TrackVector2;

    const double cmtomm = 10.;
    const double mmtocm = 0.1;
    const double mmtom  = 0.001;

} //! end namespace

#endif
