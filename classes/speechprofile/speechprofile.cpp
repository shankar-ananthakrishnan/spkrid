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
// SPEECHPROFILE.CPP                                                        *
//                                                                          *
// Implementation of `SpeechProfile', a class for creating and handling     *
// profiles of speech parameters.                                           *
// Fundamental component of the Speaker Identification Project.             *
//                                                                          *
// Last modified on April 28, 2002.                                         *
//                                                                          *
//***************************************************************************

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "../globalconstants.hpp"
#include "../signal/signal.hpp"
#include "../filter/median/medianfilter.hpp"
#include "../waveletbox/analyzer/daub2/daub2analyzer.hpp"
#include "../pitch/rgold/pitchdetector.hpp"
#include "../lpanalyzer/lpanalyzer.hpp"
#include "speechprofile.hpp"

#define    PITCH_SMOOTHER_LENGTH    5       // pitch median filter length
#define    NONLINEAR_WEIGHT_BASE    0.94    // fractional energy ratio weight

//***************************************************************************
// Constructor 1 allows the creation of a speech profile from the given     *
// sample of speech by extracting features (pitch, wavelet transform, LPC   *
// coefficients). This is used in the testing phase.                        *
//***************************************************************************
SpeechProfile::SpeechProfile (Signal& SpeechSample,
    PersonalInfo& PInfo) {
    PitchDetector *PD;
    LPAnalyzer *LPA;
    WaveletAnalyzer *WT;
    Filter *PitchSmoother;
    Signal *SmoothPitch;
    unsigned int Index;
    float Temp;

    Pitch = new Signal;
    LPC = new Signal;
    ACR = new Signal;
    DWTCoeff = new Signal * [NLEVELS];
    Personal = new PersonalInfo;

    LPCPerFrame = NLPCOEFFICIENTS;
    NumScales = NLEVELS;

    for (Index = 0; Index < NLEVELS; ++Index) {
        DWTCoeff[Index] = new Signal;
    }

// Extract pitch from the provided speech sample. Filter the pitch using a
// 5-point median filter, and also compute the average pitch. The first few
// pitch estimates are ignored as they are usually in error

    PD = new PitchDetector (SpeechSample);
    Signal& TempPitch = PD->FinalPitch ();

    PitchSmoother = new MedianFilter (PITCH_SMOOTHER_LENGTH);
    SmoothPitch = new Signal;
    PitchSmoother->ApplyFilter (TempPitch, *SmoothPitch);
    delete PitchSmoother;

    AvgPitch = 0.0;

    for (Index = 8; Index < SmoothPitch->SampleCount (); ++Index) {
        Temp = (*SmoothPitch)[Index];
        Pitch->AddSample (Temp);
        AvgPitch += Temp;
    }

    delete SmoothPitch;
    delete PD;

    Pitch->SetReadMode ();
    AvgPitch /= (Index - 8);


// Extract LPC and ACR coefficients from the speech sample.

    LPA = new LPAnalyzer (LPCPerFrame);
    LPA->PredictorCoeff (SpeechSample, *LPC, *ACR);
    delete LPA;


// Perform a discrete-time wavelet transform on the speech sample using the
// Daubechies 2 (D2) filter, upto a maximum depth of 7. This gives a freque-
// ncy resolution of 43 Hz at the highest level.

    WT = new Daub2Analyzer (NumScales);
    WT->AnalyzeDetail (SpeechSample, DWTCoeff);
    delete WT;

    strcpy (Personal->Name, PInfo.Name);
    strcpy (Personal->Address, PInfo.Address);
}

//***************************************************************************
SpeechProfile::SpeechProfile (char *ProfileName)
    throw (SpeechProfileException) {
    unsigned int Scale;
    unsigned int Index;
    float FTemp;
    unsigned int ITemp;
    FILE *Profile;

    if ((Profile = fopen (ProfileName, "rb")) == NULL) {
        throw (SP_LOAD_ERROR);
    }

    Pitch = new Signal;
    LPC = new Signal;
    ACR = new Signal;
    Personal = new PersonalInfo;

    fread (Personal, sizeof(PersonalInfo), 1, Profile);
    fread (&ITemp, sizeof(int), 1, Profile);

    AvgPitch = 0.0;

    for (Index = 0; Index < ITemp; ++Index) {
        fread (&FTemp, sizeof(float), 1, Profile);
        Pitch->AddSample (FTemp);
        AvgPitch += FTemp;
    }

    Pitch->SetReadMode ();
    AvgPitch /= Index;

    fread (&LPCPerFrame, sizeof(int), 1, Profile);
    fread (&ITemp, sizeof(int), 1, Profile);

    for (Index = 0; Index < (LPCPerFrame * ITemp); ++Index) {
        fread (&FTemp, sizeof(float), 1, Profile);
        LPC->AddSample (FTemp);
    }

    LPC->SetReadMode ();

    for (Index = 0; Index < ((LPCPerFrame + 1) * ITemp); ++Index) {
        fread (&FTemp, sizeof(float), 1, Profile);
        ACR->AddSample (FTemp);
    }

    ACR->SetReadMode ();

    fread (&NumScales, sizeof(int), 1, Profile);
    DWTCoeff = new Signal * [NumScales];

    for (Scale = 0; Scale < NumScales; ++Scale) {
        fread (&ITemp, sizeof(int), 1, Profile);
        DWTCoeff[Scale] = new Signal;

        for (Index = 0; Index < ITemp; ++Index) {
            fread (&FTemp, sizeof(float), 1, Profile);
            DWTCoeff[Scale]->AddSample (FTemp);
        }

        DWTCoeff[Scale]->SetReadMode ();
    }

    fclose (Profile);
}

//***************************************************************************
SpeechProfile::~SpeechProfile () {
    unsigned int Index;

    delete Pitch;
    delete LPC;
    delete ACR;
    delete Personal;

    for (Index = 0; Index < NumScales; ++Index) {
        delete DWTCoeff[Index];
    }

    delete[] DWTCoeff;
}

//***************************************************************************
PersonalInfo& SpeechProfile::PersonalRecord (void) {
    return (*Personal);
}

//***************************************************************************
void SpeechProfile::PersonalRecord (PersonalInfo& PInfo) {
    strcpy(Personal->Name, PInfo.Name);
    strcpy(Personal->Address, PInfo.Address);
}

//***************************************************************************
void SpeechProfile::SaveProfile (char *FileName) {
    FILE *Profile;
    float FTemp;
    unsigned int ITemp;
    unsigned int Index;
    unsigned int Scale;

    Profile = fopen (FileName, "wb");
    fwrite (Personal, sizeof(PersonalInfo), 1, Profile);

    ITemp = Pitch->SampleCount ();
    fwrite (&ITemp, sizeof(int), 1, Profile);

    for (Index = 0; Index < Pitch->SampleCount (); ++Index) {
        FTemp = (*Pitch)[Index];
        fwrite (&FTemp, sizeof(float), 1, Profile);
    }

    fwrite (&LPCPerFrame, sizeof(int), 1, Profile);
    ITemp = LPC->SampleCount () / LPCPerFrame;
    fwrite (&ITemp, sizeof(int), 1, Profile);

    for (Index = 0; Index < LPC->SampleCount (); ++Index) {
        FTemp = (*LPC)[Index];
        fwrite (&FTemp, sizeof(float), 1, Profile);
    }

    for (Index = 0; Index < ACR->SampleCount (); ++Index) {
        FTemp = (*ACR)[Index];
        fwrite (&FTemp, sizeof(float), 1, Profile);
    }

    fwrite (&NumScales, sizeof(int), 1, Profile);

    for (Scale = 0; Scale < NumScales; ++Scale) {
        ITemp = DWTCoeff[Scale]->SampleCount ();
        fwrite (&ITemp, sizeof(int), 1, Profile);

        for (Index = 0; Index < DWTCoeff[Scale]->SampleCount (); ++Index) {
            FTemp = (*DWTCoeff[Scale])[Index];
            fwrite (&FTemp, sizeof(float), 1, Profile);
        }
    }    

    fclose (Profile);
}

//***************************************************************************
float SpeechProfile::AveragePitch (void) {
    return (AvgPitch);
}

//***************************************************************************
float SpeechProfile::Euclidean (Signal& OtherPitch) {
    unsigned int Index;
    unsigned int Length;
    float Temp1, Temp2;
    float EuclidDistance;
    
    Pitch->SetReadMode();
    OtherPitch.SetReadMode();

    Length = Pitch->SampleCount ();

    if (Length > OtherPitch.SampleCount ()) {
        Length = OtherPitch.SampleCount ();
    }

    EuclidDistance = 0.0;

    for (Index = 0; Index < Length; ++Index) {
        Temp1 = (*Pitch)[Index];
        Temp2 = OtherPitch[Index];
        EuclidDistance +=
            (Temp1 - Temp2) * (Temp1 - Temp2);
    }

    return (EuclidDistance / Length);
}

//***************************************************************************
float SpeechProfile::Itakura (float *LPC1, float *LPC2, float *ACR2) {
    unsigned int Index;
    float **Toeplitz;
    float *Temp1, *Temp2;
    float Numer, Denom;
    unsigned int I, J;

    Toeplitz = new float * [LPCPerFrame + 1];
    Temp1 = new float [LPCPerFrame + 1];
    Temp2 = new float [LPCPerFrame + 1];

    for (Index = 0; Index <= LPCPerFrame; ++Index) {
        Toeplitz[Index] = new float [LPCPerFrame + 1];
    }

    for (Index = 0; Index <= LPCPerFrame; ++Index) {
        Toeplitz[0][Index] =
            Toeplitz[Index][0] = ACR2[Index];
    }

    for (I = 1; I <= LPCPerFrame; ++I) {
        for (J = I; J <= LPCPerFrame; ++J) {
            Toeplitz[I][J] = Toeplitz[J][I] =
                Toeplitz[I - 1][J - 1];
        }
    }

    Numer = Denom = 0.0;

    for (I = 0; I <= LPCPerFrame; ++I) {
        Temp1[I] = Temp2[I] = 0.0;

        for (J = 0; J <= LPCPerFrame; ++J) {
            Temp1[I] += LPC1[J] * Toeplitz[I][J];
            Temp2[I] += LPC2[J] * Toeplitz[I][J];
        }

        Numer += Temp1[I] * LPC1[I];
        Denom += Temp2[I] * LPC2[I];
    }

    delete[] Temp1;
    delete[] Temp2;

    for (Index = 0; Index <= LPCPerFrame; ++Index) {
        delete[] Toeplitz[Index];
    }

    delete[] Toeplitz;

    return (log (Numer / Denom));
}

//***************************************************************************
float SpeechProfile::ScaleDiff (Signal **OtherDWT) {
    float Energy[2][NLEVELS];
    unsigned int ThisDWTFrame[NLEVELS];
    unsigned int ThatDWTFrame[NLEVELS];
    float TotalEnergy[2];
    float Fraction[2][NLEVELS];
    unsigned int Counter;
    float Difference;
    unsigned int Scale;
    float Temp;

    for (Scale = 0; Scale < NumScales; ++Scale) {
        ThisDWTFrame[Scale] = DWTCoeff[Scale]->SampleCount () / 5;
        ThatDWTFrame[Scale] = OtherDWT[Scale]->SampleCount () / 5;
    }

    Difference = 0.0;

    for (Counter = 0; Counter < 5; ++Counter) {
        TotalEnergy[0] = TotalEnergy[1] = 0.0;

        for (Scale = 0; Scale < NumScales; ++Scale) {
            Energy[0][Scale] =
                DWTCoeff[Scale]->Energy (Counter * ThisDWTFrame[Scale],
                    (Counter + 1) * ThisDWTFrame[Scale]) /
                    ThisDWTFrame[Scale];
            
            Energy[1][Scale] =
                OtherDWT[Scale]->Energy (Counter * ThatDWTFrame[Scale],
                    (Counter + 1) * ThatDWTFrame[Scale]) /
                    ThatDWTFrame[Scale];

            TotalEnergy[0] += Energy[0][Scale];
            TotalEnergy[1] += Energy[1][Scale];
        }
        
        for (Scale = 0; Scale < NumScales; ++Scale) {
            Fraction[0][Scale] = Energy[0][Scale] / TotalEnergy[0];
            Fraction[1][Scale] = Energy[1][Scale] / TotalEnergy[1];
        }

        for (Scale = 0; Scale < NumScales; ++Scale) {
            Temp = Fraction[0][Scale] / Fraction[1][Scale];
            if (Temp < 1.0) Temp = (1.0 / Temp);
            Temp *= pow (NONLINEAR_WEIGHT_BASE, Scale);
            Difference += log (Temp);
        }
    }

    return ((Difference + fabs (Difference)) / 2.0);
}

//***************************************************************************
float SpeechProfile::Distance (SpeechProfile& OtherProfile) {
    float PitchDistance;
    float LPCDistance;
    float WaveDistance;
    unsigned int Index;
    unsigned int Frame;
    unsigned int NFrames, Temp;
    float *LPC1Buf, *LPC2Buf;
    float *ACR2Buf;
    Signal *FrameDist;
    Signal *SmoothLPC;
    Filter *LPCSmoother;
    FILE *LogFile;

    LPC1Buf = new float [LPCPerFrame + 1];
    LPC2Buf = new float [LPCPerFrame + 1];
    ACR2Buf = new float [LPCPerFrame + 1];

    PitchDistance = Euclidean (*(OtherProfile.Pitch));
    WaveDistance = ScaleDiff (OtherProfile.DWTCoeff);

    NFrames = LPC->SampleCount () / LPCPerFrame;
    Temp = OtherProfile.LPC->SampleCount () / LPCPerFrame;
    if (Temp < NFrames) NFrames = Temp;

    FrameDist = new Signal;

    for (Frame = 0; Frame < NFrames; ++Frame) {
        LPC1Buf[0] = LPC2Buf[0] = -1;
        for (Index = 0; Index < LPCPerFrame; ++Index) {
            LPC1Buf[Index + 1] = (*LPC)[Frame * LPCPerFrame + Index];
            LPC2Buf[Index + 1] = (*(OtherProfile.LPC))
                [Frame * LPCPerFrame + Index];
            ACR2Buf[Index] = (*(OtherProfile.ACR))
                [Frame * (LPCPerFrame + 1) + Index];
        }

        ACR2Buf[Index] = (*(OtherProfile.ACR))
            [Frame * (LPCPerFrame + 1) + Index];

        FrameDist->AddSample (Itakura (LPC1Buf, LPC2Buf, ACR2Buf));
    }

    SmoothLPC = new Signal;
    LPCSmoother = new MedianFilter (5);
    LPCSmoother->ApplyFilter (*FrameDist, *SmoothLPC);
    delete LPCSmoother;
    delete FrameDist;

    LPCDistance = 0.0;

    for (Frame = 0; Frame < SmoothLPC->SampleCount (); ++Frame) {
        LPCDistance += (*SmoothLPC)[Frame];
    }

    delete SmoothLPC;

    PitchDistance *= 5000000.0;
    LPCDistance = (LPCDistance * 50.0 / NFrames);
    WaveDistance *= 3.5;

    LogFile = fopen ("log.txt", "at");
    fprintf (LogFile, "Comparing with %s\n", OtherProfile.Personal->Name);
    fprintf (LogFile, "Pitch   : %5.10f\n", PitchDistance);
    fprintf (LogFile, "LPC     : %5.10f\n", LPCDistance);
    fprintf (LogFile, "Wavelet : %5.10f\n\n", WaveDistance);
    fclose (LogFile);

    delete[] LPC1Buf;
    delete[] LPC2Buf;
    delete[] ACR2Buf;

    return (PitchDistance + LPCDistance + WaveDistance);
}

//*************************** SPEECHPROFILE.CPP *****************************
