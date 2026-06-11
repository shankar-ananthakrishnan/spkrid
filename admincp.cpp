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
// ADMINCP.CPP                                                              *
//                                                                          *
// Administrator module for the Speaker Identification Project.             *
// A front-end utility for managing user profiles.                          *
//                                                                          *
// Last modified on April 18, 2002.                                         *
//                                                                          *
//***************************************************************************

#include <stdio.h>
#include <string.h>
#include "classes/uinterface/console/conio_compat.h"

#include "classes/globalconstants.hpp"
#include "classes/signal/signal.hpp"
#include "classes/audioreader/alsawavereader/alsawavereader.hpp"
#include "classes/silenceremover/silenceremover.hpp"
#include "classes/audiowriter/playaudio.hpp"
#include "classes/speechprofile/speechprofile.hpp"
#include "classes/uinterface/console/keycode.hpp"
#include "classes/uinterface/console/console.hpp"
#include "classes/uinterface/infobox/infobox.hpp"
#include "classes/uinterface/menugeneric/adminmainmenu/adminmainmenu.hpp"

int main (void) {
    FILE *AdminPrf;
    MenuGeneric *Admin;
    SpeechProfile *AdminProfile;
    SpeechProfile *TestProfile;
    char ProfileName[MAX_PROFILE_LEN + 50];
    bool AdminVerified = false;
    InfoBox *Prompt;
    unsigned int Key;
    AudioReader *Reader;
    AudioReader *SReader;
    SilenceRemover *Clipper;
    Signal *ClippedAudio;
    InfoBox *Status;
    bool InsuffAudio;
    PersonalInfo UserInfo;

    initscr ();
    cbreak ();
    noecho ();
    keypad (stdscr, TRUE);

    clrscr ();
    textattr (REVERSE_VIDEO);
    clreol ();
    cputs (" Speaker Identification System (Administrator Control Panel)");
    textattr (NORMAL_VIDEO);

    if ((AdminPrf = fopen ("adminprf", "rt")) != NULL) {
        Prompt = new InfoBox (" InfoBox ", 10, 18, 16, 70);
        Prompt->WriteText ("The program will now verify whether you are a valid "
            "administrator or not.");
        Prompt->MoveCursor (5, 1);
        PlayAudio (AUDIO_PATH "checkadmin.wav");
        Prompt->WriteText ("Hit any key to proceed (ESC cancels)...");
        Key = ReadKey ();
        delete Prompt;
        if (Key == KEY_ESCAPE) {
            clrscr ();
            endwin ();
            return (1);
        }

        Prompt = new InfoBox (" InfoBox ", 10, 12, 16, 75);
        Prompt->WriteText ("The program will now take a sample of the ambient "
            "noise level. Please make sure "
            "that there is no disturbance till the next instruction appears.");
        Prompt->MoveCursor (5, 1);
        Prompt->WriteText ("Hit any key to proceed (ESC cancels)...");
        Key = ReadKey ();
        delete Prompt;
        if (Key == KEY_ESCAPE) {
            clrscr ();
            endwin ();
            return (1);
        }

        Status = new InfoBox (" InfoBox ", 10, 28, 14, 50);
        Status->MoveCursor (2, 2);
        Status->WriteText ("Sampling silence...");
    
        SReader = new ALSAWaveReader (AUDIO_SAMPLE_RATE, 0.20);
        Signal& Silence = SReader->AudioData ();
        delete Status;
    
        Prompt = new InfoBox (" InfoBox ", 10, 15, 15, 70);
        Prompt->WriteText
            ("You will now be given 5 seconds to record the vowels "
             " a, e, i, o, u. Please speak clearly and distinctly.");
    
        Prompt->MoveCursor (4, 1);
        Prompt->WriteText ("Hit any key to begin recording (ESC cancels)...");
        
        Key = ReadKey ();
        delete Prompt;
    
        if (Key == KEY_ESCAPE) {
            delete SReader;
            endwin ();
            return (1);
        }
    
        Status = new InfoBox (" InfoBox ", 10, 28, 14, 50);
        Status->MoveCursor (2, 2);
        Status->WriteText ("Recording speech...");
    
        Clipper = new SilenceRemover (Silence);
        delete SReader;
    
        do {
            Reader = new ALSAWaveReader (AUDIO_SAMPLE_RATE, 5.0);
            Signal& Audio = Reader->AudioData ();
            ClippedAudio = new Signal;
            Clipper->ClipSilence (Audio, *ClippedAudio);
            InsuffAudio = ClippedAudio->SampleCount () < MILLISECOND(1700);
            delete Reader;
    
            if (InsuffAudio) {
                delete ClippedAudio;
                Prompt = new InfoBox (" InfoBox ", 10, 13, 15, 72);
                Prompt->WriteText
                    ("There was not sufficient audio to work with. Please retry "
                     "recording the vowels a, e, i, o, u clearly and distinctly.");
        
                Prompt->MoveCursor (4, 1);
                PlayAudio (AUDIO_PATH "userinsuffaudio.wav");
                Prompt->WriteText ("Hit any key to begin recording...");
                ReadKey ();
                delete Prompt;
            }
        } while (InsuffAudio);
    
        delete Clipper;
    
        Status->WipeClean ();
        Status->MoveCursor (2, 2);
        Status->WriteText ("Building profile...");
    
        strcpy (UserInfo.Name, "Unknown");
        strcpy (UserInfo.Address, "Unknown");
        TestProfile = new SpeechProfile (*ClippedAudio, UserInfo);
        delete ClippedAudio;
        delete Status;
    
        while ((fscanf (AdminPrf, "%s", ProfileName) != EOF) && !AdminVerified) {
            AdminProfile = new SpeechProfile (ProfileName);
            
            if (TestProfile->Distance (*AdminProfile) < ADMIN_THRESHOLD) {
                AdminVerified = true;
            }
    
            delete AdminProfile;
        }
    
        delete TestProfile;
    
        if (!AdminVerified) {
            Prompt = new InfoBox (" InfoBox ", 10, 22, 15, 62);
            Prompt->MoveCursor (2, 1);
            Prompt->WriteText ("You are denied access to the system!");
            PlayAudio (AUDIO_PATH "accessdenied.wav");
            Prompt->MoveCursor (4, 1);
            Prompt->WriteText ("Hit any key to exit...");
            ReadKey ();
            delete Prompt;
            clrscr ();
            endwin ();
            return (1);
        }
    }

    Admin = new AdminMainMenu (8, 28);
    Admin->Execute ();
    delete Admin;

    clrscr ();
    endwin ();
    return (0);
}

//***************************** ADMINCP.CPP *********************************
