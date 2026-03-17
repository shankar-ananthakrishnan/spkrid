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
// DAUB2ANALYZER.HPP                                                        *
//                                                                          *
// Header for class `Daub2Analyzer', which implements the Discrete-Time     *
// Wavelet Transform using the Daubechies 2 wavelet with an FIR filterbank. *
// Fundamental component of the Speaker Identification Project.             *
//                                                                          *
// Last modified on February 8, 2001.                                       *
//                                                                          *
//***************************************************************************

#ifndef __DAUB2ANALYZER_HPP
#define __DAUB2ANALYZER_HPP

#include "../../../signal/signal.hpp"
#include "../../../filter/fir/firfilter.hpp"
#include "../waveletanalyzer.hpp"
#include "../downsampler/downsampler.hpp"

//***************************************************************************
class Daub2Analyzer : public WaveletAnalyzer {
private:
    FIR_Filter *LPF;                    // LPF generates approximations
    FIR_Filter *HPF;                    // HPF generates details (DTWT)
    DownSampler *DNS;                   // reduces scale

    void RecursiveFilterBank (Signal& Approx,
        Signal **Detail, unsigned int Scale);

public:
    Daub2Analyzer (unsigned int MaxScale);
    ~Daub2Analyzer ();

    virtual void AnalyzeDetail (Signal& Raw,
        Signal **Detail);
};

#endif

//************************* DAUB2ANALYZER.HPP *******************************
