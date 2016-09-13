//!-----------------------------------------------------------------------------------
//! This Helix struct contains the helical parameters and the pivot point
//!
//! @author Leah Welty-Rieger
//! @author Tasha Arvanitis
//! @date July 2013
//!
//!  modified and moved to its home by T. Walton Feb. 2015
//!-----------------------------------------------------------------------------------

#ifndef HELIX_HH
#define HELIX_HH

#include "Line.hh"
#include "Circle.hh"
#include "TrackParamDefs.hh"

namespace gm2strawtracker {

    //! A support struct to encapsulate the idea of a value with an error
    struct ValueWithError {
        double value;
        double sigma;

        //! Default constructor
        ValueWithError() : value(0.), sigma(0.) {}

#ifndef __GCCXML__

        ValueWithError(double value_, double sigma_) 
            : value(value_)
            , sigma(sigma_)
        {}

#endif // __GCCXML__

    }; //! ValueWithError struct definition

    struct Helix : public Circle, Line {

       public :

          //! the pivotal point
          double pivot_x, pivot_y, pivot_z;
         
          //! the signed distance of the helix from the pivotal point the the xy plane (displacement in the radial direction)
          ValueWithError drho;

          //! the azimuthal angle which specifies the pivot point w.r.t the helix center and having a range from 0 to 2pi
          ValueWithError phi0;

          //! reciprocal of the transverse momentum ( Q/pT ), which the signed represents the charge of the track
          ValueWithError kappa;

          //! 1/cB
          ValueWithError alpha;

          //! (1/cB*kappa) 
          ValueWithError rho;

          //! the signed vertical displacement from the pivotal point
          ValueWithError dz;

          //! the tangent of the dip angle (the slope of the track)
          ValueWithError tlambda;
        
          //! The reference time t0 of the particle
          ValueWithError t0;

          //! the deflection angle
          double phi;

          //! default constructor
          Helix() 
            : pivot_x(0.)
            , pivot_y(0.)
            , pivot_z(0.)
            , drho()
            , phi0()
            , kappa()
            , alpha()
            , rho()
            , dz()
            , tlambda()
            , t0()
            , phi()
          {}

#ifndef __GCCXML__

	  //! copy constructor
	  Helix( const Helix& helix ) 
            : pivot_x(helix.pivot_x)
            , pivot_y(helix.pivot_y)
            , pivot_z(helix.pivot_z)
            , drho(helix.drho)
            , phi0(helix.phi0)
            , kappa(helix.kappa)
            , alpha(helix.alpha)
            , rho(helix.rho)
            , dz(helix.dz)
            , tlambda(helix.tlambda)
            , t0(helix.t0)
            , phi(helix.phi)
          {}
       
          Helix( const Helix& helix, const TrackMatrix5x5& covMatrix )
            : drho(helix.drho.value,sqrt(covMatrix(0,0)))
            , phi0(helix.phi0.value,sqrt(covMatrix(1,1)))
            , kappa(helix.kappa.value,sqrt(covMatrix(2,2)))
            , dz(helix.dz.value,sqrt(covMatrix(3,3)))
            , tlambda(helix.tlambda.value,sqrt(covMatrix(4,4)))
          {} 
 
        //! Some helpful overloaded operators
        bool hasSamePivot(const Helix & h1) const {
            if ((fabs(this->pivot_x - h1.pivot_x) > 1.0e-5) ||
                (fabs(this->pivot_y - h1.pivot_y) > 1.0e-5) ||
                (fabs(this->pivot_z - h1.pivot_z) > 1.0e-5))
                return false;
            return true;
        }
       
        TrackVector5 makeVector5() {
            TrackVector5 res((*this).drho.value,
                             (*this).phi0.value,
                             (*this).kappa.value,
                             (*this).dz.value,
                             (*this).tlambda.value);
            return res;
        }
        
        TrackVector6 makeVector6() {
            TrackVector6 res((*this).drho.value,
                             (*this).phi0.value,
                             (*this).kappa.value,
                             (*this).dz.value,
                             (*this).tlambda.value,
                             (*this).t0.value);
            return res;
        }
         
        bool operator==(const Helix & h1) const {
            Helix diff = (*this) - h1;
            
            if (fabs(diff.drho.value)    < 1.0e-5 && 
                fabs(diff.phi0.value)    < 1.0e-5 &&
                fabs(diff.kappa.value)   < 1.0e-5 && 
                fabs(diff.dz.value)      < 1.0e-5 &&
                fabs(diff.tlambda.value) < 1.0e-5 && hasSamePivot(h1))
                return true;
            
            return false;
        }
        
        bool operator!=(const Helix & h1) const {
            return !(*this == h1);
        }
        
        Helix & operator+=(const TrackVector5 & vector) {
            (*this).drho.value += vector[0];
            (*this).phi0.value += vector[1];
            (*this).kappa.value += vector[2];
            (*this).dz.value += vector[3];
            (*this).tlambda.value += vector[4];
            
            return (*this);
        }
        
        Helix & operator-=(const TrackVector5 & vector) {
            (*this).drho.value -= vector[0];
            (*this).phi0.value -= vector[1];
            (*this).kappa.value -= vector[2];
            (*this).dz.value -= vector[3];
            (*this).tlambda.value -= vector[4];
            
            return (*this);
        }
        
        Helix & operator+=(const Helix & h1) {
            if (this != &h1 && hasSamePivot(h1)) {
                (*this).drho.value += h1.drho.value;
                (*this).phi0.value += h1.phi0.value;
                (*this).kappa.value += h1.kappa.value;
                (*this).dz.value += h1.dz.value;
                (*this).tlambda.value += h1.tlambda.value;
            }
            return (*this);
        }

        Helix & operator-=(const Helix & h1) {
            if (this != &h1 && hasSamePivot(h1)) {
                (*this).drho.value -= h1.drho.value;
                (*this).phi0.value -= h1.phi0.value;
                (*this).kappa.value -= h1.kappa.value;
                (*this).dz.value -= h1.dz.value;
                (*this).tlambda.value -= h1.tlambda.value;
            }
            return (*this);
        }
        
        const Helix operator+(const Helix & h1) const {
            return Helix(*this) += h1;
        }
        
        const Helix operator-(const Helix & h1) const {
            return Helix(*this) -= h1;
        }
        
        const Helix operator+(const TrackVector5 & vector) const {
            return Helix(*this) += vector;
        }
        
        const Helix operator-(const TrackVector5 & vector) const {
            return Helix(*this) -= vector;
        }
        
        friend std::ostream & operator<<(std::ostream & os, const Helix & h1) {
            os << "{" << h1.drho.value << ", " << h1.phi0.value << ", "
               << h1.kappa.value << ", " << h1.dz.value << ", "
               << h1.tlambda.value << "}\t";
            os << "{" << h1.pivot_x << ", " << h1.pivot_y << ", " << h1.pivot_z << "}\n";
            return os;
        }
        

#endif // __GCCXML__

    }; //! end definition of struct Helix

} //! namespace

#endif
