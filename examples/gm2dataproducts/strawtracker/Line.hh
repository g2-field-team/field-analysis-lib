//!-----------------------------------------------------------------------------------
//! This Line struct contains a straight line fitting parameters 
//!-----------------------------------------------------------------------------------

#ifndef LINE_HH
#define LINE_HH

namespace gm2strawtracker {

    struct Line {

       public :

          // the z-position of the initial position
          double z0;

          // the tangent of the angle between the projected track tangent in the x-y plane and track tangent 
          double tlambda;

          // the error on z0
          double z0Error;

          // the error on the dip angle
          double tlambdaError;

          // the chi2
          double chi2;

          // the number of degrees of freedom
          int dof;

          // the reduced chi2
          double chi2DoF;

          //! default constructor 
          Line() 
            : z0(9999)
            , tlambda(9999)
            , z0Error()
            , tlambdaError()
            , chi2()
            , dof()
            , chi2DoF()
          {}

#ifndef __GCCXML__

	  //! copy constructor
	  Line( const Line& line ) 
            : z0(line.z0)
            , tlambda(line.tlambda)
            , z0Error(line.z0Error)
            , tlambdaError(line.tlambdaError)
            , chi2(line.chi2)
            , dof(line.dof)
            , chi2DoF(line.chi2DoF)
          {}

#endif // __GCCXML__

    }; //! end definition of struct Line

} //! namespace

#endif
