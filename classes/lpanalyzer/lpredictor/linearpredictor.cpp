//***************************************************************************
//                                                                          *
// SPKRID, A Prototype Text-Dependent Speaker Identification System.        *
// Copyright (C) 2002 A.Sankaranarayanan.                                   *
//                                                                          *
// This file is part of SPKRID.                                             *
//                                                                          *
// SPKRID is free software; you can redistribute it and/or                  *
// modify it under the terms of the GNU General Public License              *
// as published by the Free Software Foundation; either version 2           *
// of the License, or (at your option) any later version.                   *
//                                                                          *
// This program is distributed in the hope that it will be useful,          *
// but WITHOUT ANY WARRANTY; without even the implied warranty of           *
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
// GNU General Public License for more details.                             *
//                                                                          *
// You should have received a copy of the GNU General Public License        *
// along with this program; if not, write to the Free Software              *
// Foundation, Inc., 59 Temple Place - Suite 330, Boston,                   *
// MA  02111-1307, USA.                                                     *
//                                                                          *
// My e-mail address is: total_nerd@hotmail.com.                            *
//                                                                          *
//                                                                          *
// LINEARPREDICTOR.CPP                                                      *
//                                                                          *
// Implementation of the LinearPredictor class, which generates linear      *
// predictor coefficients for a single frame of speech data.                *
// Fundamental component of the Speaker Identification Project.             *
//                                                                          *
// Last modified on February 24, 2002.                                      *
//                                                                          *
//***************************************************************************

#include "../../globalconstants.hpp"
#include "linearpredictor.hpp"
#include "../../dspwindows/softwindows.hpp"

//***************************************************************************
LinearPredictor::LinearPredictor
    (unsigned int NCoeff) : NLPCoeff(NCoeff) {}

//****************************************************************************
void LinearPredictor::FrameLPCoeff (float *SFrame,
            unsigned int Length, float *LPC, float *ACR) {
    float Error;                        // predictor error
    float k;
    float *Frame;                       // weighted samples
    float TempL, TempR;                 // temp storage for in-place calc.
    unsigned int Index;
    unsigned int i;                     // shouldn't name them this way, but
    unsigned int j;                     //   they're straight from the formula

    Frame = new float [Length];         // memory for frame

                                        // soften signal to reduce error
    for (Index = 0; Index < Length; Index++){
        Frame[Index] = SFrame[Index] * Hamming (Index, Length);
    }
	
    for(i = 0; i <= NLPCoeff; ++i) {    // lags up to no. of coeffs
        ACR[i] = 0.0;                   // initialize sum
        for (Index = 0; Index < (Length - i); ++Index) {
            ACR[i] += Frame[Index] * Frame[Index + i];
        }
    }

//
// Implementation of Durbin's recursive solution for a Toeplitz matrix. The
// predictor coefficients are solutions of a system of linear equations whose
// matrix is symmetric and also has equal diagonal elements. The method is
// detailed below.
//
//  0.  E(0) = R(0)
//                  i-1
//  1.  k = [R(i) - sum (a(j,i-1) * R(i-j))] / E(i-1),      1 <= i <= p
//                  j=1
//  2.  a(i,i) = k
//  3.  a(j,i) = a(j,i-1) - k * a(i-j,i-1),                 1 <= j <= i-1
//  4.  E(i) = (1 - k^2) * E(i-1)
//
//  At this point, the array `a' contains the linear predictor coefficients.
//

    Error = ACR[0];                                 // step 0

    for(i = 1; i <= NLPCoeff; ++i) {                // step 1
        for(j = 1, k = ACR[i]; j <= (i - 1); ++j){
            k -= LPC[j - 1] * ACR[i - j];
        }

        k /= Error;
        LPC[i - 1] = k;                             // step 2

        for(j = 1; j <= ((i - 1) / 2); ++j) {       // step 3
            TempL = LPC[j - 1] - (k * LPC[i - j - 1]);
            TempR = LPC[i - j - 1] - (k * LPC[j - 1]);
            LPC[j - 1] = TempL;         // club two calculations
            LPC[i - j - 1] = TempR;     //   for in-place
        }

        if ((i - 1) & 1) {
            LPC[(i / 2) - 1] *= (1 - k);
        }

        Error *= (1 - (k * k));                     // step 4
    }

    delete[] Frame;
}

//*************************** LINEARPREDICTOR.CPP ***************************
