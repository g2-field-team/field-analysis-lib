//!-----------------------------------------------------------------------------------
//! This Circle struct contains the effective circle fitting parameters 
//! based on Karimaki's parameterization
//!-----------------------------------------------------------------------------------

#ifndef CIRCLE_HH
#define CIRCLE_HH

#include "TrackParamDefs.hh"

namespace gm2strawtracker {

    struct Circle {

       public :

          // the signed distance of the point the the xy plane closest to the origin of the circle
          double rho;

          // the angle between the x-axis and the projected track tangent
          double phi;

          // the signed curvature of the track in the x-y plane
          double kappa;

          // the chi2
          double chi2;

          // the number of degrees of freedom
          int dof;

          // the reduced chi2
          double chi2DoF;

          // check if the covariance matrix was successfully inverted
          bool invertSuccess;

          // retreive the errors on the circle parameters (rho, phi, kappa)
          TrackMatrix3x3 errorMatrix;

          // default constructor 
          Circle() 
            : rho()
            , phi()
            , kappa()
            , chi2()
            , dof()
            , chi2DoF()
            , invertSuccess()
            , errorMatrix()
          {}


#ifndef __GCCXML__

	  // copy constructor
	  Circle( const Circle& circle ) 
            : rho(circle.rho)
            , phi(circle.phi)
            , kappa(circle.kappa)
            , chi2(circle.chi2)
            , dof(circle.dof)
            , chi2DoF(circle.chi2DoF)
            , invertSuccess(circle.invertSuccess)
            , errorMatrix(circle.errorMatrix)
          {}


#endif // __GCCXML__



    }; //! end definition of struct Circle

} //! namespace

#endif
