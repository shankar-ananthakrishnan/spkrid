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
// SPEECHPROFILE.HPP                                                        *
//                                                                          *
// Header file for `SpeechProfile', a class for creating and handling       *
// profiles of speech parameters.                                           *
// Fundamental component of the Speaker Identification Project.             *
//                                                                          *
// Last modified on April 25, 2002.                                         *
//                                                                          *
//***************************************************************************

#ifndef __SPEECHPROFILE_HPP
#define __SPEECHPROFILE_HPP

#include "../signal/signal.hpp"

#define    MAX_NAME_LEN    50
#define    MAX_ADDRESS_LEN 60

enum SpeechProfileException {
    SP_LOAD_ERROR
};

//***************************************************************************
struct PersonalInfo {
    char Name[MAX_NAME_LEN];
    char Address[MAX_ADDRESS_LEN];
};

//***************************************************************************
class SpeechProfile {
private:
    Signal *Pitch;
    Signal *LPC, *ACR;
    Signal **DWTCoeff;
    PersonalInfo *Personal;
    float AvgPitch;
    unsigned int LPCPerFrame;
    unsigned int NumScales;

    float Euclidean (Signal& OtherPitch);
    float Itakura (float *LPC1, float *LPC2, float *ACR2);
    float ScaleDiff (Signal **OtherDWT);

public:
    SpeechProfile (Signal& SpeechSample,
        PersonalInfo& PersonalDetails);

    SpeechProfile (char *ProfileName)
        throw (SpeechProfileException);

    ~SpeechProfile ();

    void SaveProfile (char *FileName);
    float AveragePitch (void);
    float Distance (SpeechProfile& OtherProfile);

    PersonalInfo& PersonalRecord (void);
    void PersonalRecord (PersonalInfo& PInfo);
};

#endif

//*************************** SPEECHPROFILE.HPP *****************************
