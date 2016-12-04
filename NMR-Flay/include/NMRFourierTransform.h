#ifndef NMRFOURIERTRANSFORM_H
#define NMRFOURIERTRANSFORM_H

// A class for computing the fourier transform of data 

#include <cstdlib>
#include <cmath> 
#include <vector>
#include "NMRMath.h"

#define MAXSIZE 1E+7

namespace g2field{
  class NMRFourierTransform{

    private:
      double fZeroPaddingTime;                                      // time used to zero-pad data  
      double fSampleFreq;                                           // sampling frequency (Hz) 
      std::vector<double> fComplex;                                             // complex array used in computing the FFT 

      void ClearArray(); 
      void four1(int isign,int nn,std::vector<double>& data); 

      int GetNFFT(int N); 

    public: 
      NMRFourierTransform();
      ~NMRFourierTransform(); 

      void SetSampleFrequency(double f){fSampleFreq      = f;} 
      void SetZeroPaddingTime(double t){fZeroPaddingTime = t;} 

      int Transform(int N,const std::vector<double>& f,std::vector<double>& F);                     // N = size of f; f = input (time domain), F = output (freq domain); returns number of FFT points  
      void InverseTransform(int N,const std::vector<double>& F,std::vector<double>& f);             // N = size of f; F = input (freq domain), f = output (time domain) 

  };

};
#endif 
