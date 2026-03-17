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
// DAUB2ANALYZER.CPP                                                        *
//                                                                          *
// Implementation of class `Daub2Analyzer', which implements the Discrete   *
// Time Wavelet Transform using the Daubechies 2 wavelet with an FIR        *
// filterbank.                                                              *
// Fundamental component of the Speaker Identification Project.             *
//                                                                          *
// Last modified on February 8, 2001.                                       *
//                                                                          *
//***************************************************************************

#include "../../../signal/signal.hpp"
#include "../../../filter/fir/firfilter.hpp"
#include "../waveletanalyzer.hpp"
#include "../downsampler/downsampler.hpp"
#include "daub2analyzer.hpp"

//***************************************************************************
Daub2Analyzer::Daub2Analyzer
        (unsigned int MaxScale) : WaveletAnalyzer (MaxScale) {
    static float LPFCoeff[] = {         // approximation filter
        -0.0915064,
         0.1584936,
         0.5915064,
         0.3415064
    };

    static float HPFCoeff[] = {         // detail filter
         0.3415064,
        -0.5915064,
         0.1584936,
         0.0915064
    };

    LPF = new FIR_Filter (LPFCoeff, 4);     // FIR filterbank
    HPF = new FIR_Filter (HPFCoeff, 4);
    DNS = new DownSampler (DNS_ODD_SAMPLER);
}

//***************************************************************************
Daub2Analyzer::~Daub2Analyzer () {
    delete LPF;                         // get rid of filters
    delete HPF;
    delete DNS;
}

//***************************************************************************
void Daub2Analyzer::AnalyzeDetail (Signal& Raw,
        Signal **Detail) {
    RecursiveFilterBank (Raw, Detail, 0);
}

//***************************************************************************
void Daub2Analyzer::RecursiveFilterBank (Signal& Approx,
        Signal **Detail, unsigned int Scale) {
    Signal *NextDetail;
    Signal *Smoothed;
    Signal *NextApprox;

    if (Scale == HighestScale ()) return;

    NextDetail = new Signal;

    HPF->ApplyFilter (Approx, *NextDetail);
    DNS->SampleDown (*NextDetail, *Detail[Scale]);
    delete NextDetail;

    Smoothed = new Signal;
    NextApprox = new Signal;

    LPF->ApplyFilter (Approx, *Smoothed);
    DNS->SampleDown (*Smoothed, *NextApprox);
    delete Smoothed;
    
    RecursiveFilterBank (*NextApprox, Detail, (Scale + 1));
    delete NextApprox;
}

//*************************** DAUB2ANALYZER.CPP *****************************
