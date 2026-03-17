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
// USERMAINMENU.CPP                                                         *
//                                                                          *
// Implementation of class 'UserMainMenu', which implements the main menu   *
// of the user module of the Speaker Identification Project.                *
// User interface component.                                                *
//                                                                          *
// Last modified on April 26, 2002.                                         *
//                                                                          *
//***************************************************************************

#include <string.h>

#include "../../../globalconstants.hpp"
#include "../../../audioreader/winwavereader/winwavereader.hpp"
#include "../../../audiowriter/playaudio.hpp"
#include "../../../silenceremover/silenceremover.hpp"
#include "../../../speechprofile/speechprofile.hpp"
#include "../../../profilemanager/profilemanager.hpp"
#include "../../console/console.hpp"
#include "../../console/keycode.hpp"
#include "../../infobox/infobox.hpp"
#include "../../dialogbox/dialogbox.hpp"
#include "../menugeneric.hpp"
#include "usermainmenu.hpp"

#define    USR_IDENTIFY    1
#define    USR_VERIFY      2
#define    USR_TERMINATE   3

//***************************************************************************
UserMainMenu::UserMainMenu (unsigned int Y,
    unsigned int X) : MenuGeneric (MENU_PATH "usermainmenu", Y, X) {}

//***************************************************************************
void UserMainMenu::Execute (void) {
    InfoBox *Prompt;
    InfoBox *Result;
    InfoBox *Status;
    AudioReader *SReader;
    AudioReader *Reader;
    SilenceRemover *Clipper;
    Signal *ClippedAudio;
    bool InsuffAudio;
    float *Distance;
    float MinDistance;
    unsigned int MinDistIndex;
    unsigned int Index;
    unsigned int Choice;
    unsigned int Top, Bottom, Mid, Size;
    unsigned int ProfileIndex;
    ProfileManager *SpeechProfileArray;
    SpeechProfile *UserProfile;
    SpeechProfile *RefProfile;
    DialogBox *ProfileReader;
    char **Profile;
    PersonalInfo UserInfo;
    unsigned int Key;
    char *TempBuffer;

    do {
        Choice = Interact ();

        if ((Choice == 0) || (Choice == USR_TERMINATE)) break;

        Prompt = new InfoBox (" InfoBox ", 10, 12, 16, 75);
        Prompt->WriteText ("The program will now take a sample of the "
            "ambient noise level. Please make sure that there is no "
            "disturbance till the next  instruction appears.");
        Prompt->MoveCursor (5, 1);
        PlayAudio (AUDIO_PATH "usersilencenotify.wav");
        Prompt->WriteText ("Hit any key to proceed (ESC cancels)...");

        Key = ReadKey ();
        delete Prompt;

        if (Key == KEY_ESCAPE) continue;

        Status = new InfoBox (" InfoBox ", 10, 28, 14, 50);
        Status->MoveCursor (2, 2);
        Status->WriteText ("Sampling silence...");

        SReader = new WinWaveReader (AUDIO_SAMPLE_RATE, 0.20);
        Signal& Silence = SReader->AudioData ();
        delete Status;

        Prompt = new InfoBox (" InfoBox ", 10, 15, 15, 70);
        Prompt->WriteText
            ("You will now be given 5 seconds to record the vowels "
             " a, e, i, o, u. Please speak clearly and distinctly.");

        Prompt->MoveCursor (4, 1);
        PlayAudio (AUDIO_PATH "useraudioprompt.wav");
        Prompt->WriteText ("Hit any key to begin recording (ESC cancels)...");
        
        Key = ReadKey ();
        delete Prompt;

        if (Key == KEY_ESCAPE) {
            delete SReader;
            continue;
        }

        Status = new InfoBox (" InfoBox ", 10, 28, 14, 50);
        Status->MoveCursor (2, 2);
        Status->WriteText ("Recording speech...");

        Clipper = new SilenceRemover (Silence);
        delete SReader;

        do {
            Reader = new WinWaveReader (AUDIO_SAMPLE_RATE, 5.0);
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
        UserProfile = new SpeechProfile (*ClippedAudio, UserInfo);
        delete ClippedAudio;
        delete Status;

        SpeechProfileArray =
            new ProfileManager (DATABASE_PATH DATABASE_INDEX, DATABASE_PATH);

        switch (Choice) {
            case USR_IDENTIFY:
                Status = new InfoBox (" InfoBox ", 10, 27, 14, 50);
                Status->MoveCursor (2, 2);
                Status->WriteText ("Scanning database...");

                Top = 0;
                Size = SpeechProfileArray->ProfileCount ();
                Bottom = Size - 1;

                while (((Bottom - Top) > (0.2 * Size)) &&
                    ((Bottom - Top) > 20)) {
                    Mid = (Top + Bottom) / 2;
                    RefProfile = (*SpeechProfileArray)[Mid];

                    if (UserProfile->AveragePitch () <
                        RefProfile->AveragePitch ()) {
                        Bottom = (Bottom + Mid) / 2;
                    }
                    else {
                        Top = (Top + Mid) / 2;
                    }

                    delete RefProfile;
                }

                Distance = new float [Bottom - Top + 1];

                for (Index = Top; Index <= Bottom; ++Index) {
                    RefProfile = (*SpeechProfileArray)[Index];
                    Distance[Index - Top] =
                        UserProfile->Distance (*RefProfile);

                    delete RefProfile;
                }

                MinDistance = Distance[0];
                MinDistIndex = 0;

                for (Index = 1; Index <= (Bottom - Top); ++Index) {
                    if (MinDistance > Distance[Index]) {
                        MinDistance = Distance[Index];
                        MinDistIndex = Index;
                    }
                }

                delete[] Distance;
                delete Status;

                if (MinDistance < THRESHOLD) {
                    RefProfile = (*SpeechProfileArray)[Top + MinDistIndex];
                    PersonalInfo& Record = RefProfile->PersonalRecord ();

                    Result = new InfoBox (" Match ", 9, 14, 18, 66);
                    Result->WriteText ("Search successful. You have been "
                        "identified as");
                    Result->MoveCursor (3, 2);
                    Result->WriteText ("Name    : ");
                    Result->WriteText (Record.Name);
                    Result->MoveCursor (5, 2);
                    Result->WriteText ("Address : ");
                    Result->WriteText (Record.Address);
                    Result->MoveCursor (7, 1);
                    PlayAudio (AUDIO_PATH "identifysuccess.wav");
                    Result->WriteText ("Hit any key if you wish to update "
                        "your profile, or Esc to retain the old profile.");

                    if (ReadKey () != KEY_ESCAPE) {
                        UserInfo = RefProfile->PersonalRecord ();
                        UserProfile->PersonalRecord (UserInfo);
                        SpeechProfileArray->UpdateProfile (*UserProfile,
                            (Top + MinDistIndex));
                    }

                    delete RefProfile;
                }
                else {
                    Result = new InfoBox (" No Match ", 10, 24, 15, 60);
                    Result->MoveCursor (2, 1);
                    Result->AlignCentre ("No matching entries in database.");
                    Result->MoveCursor (4, 1);
                    PlayAudio (AUDIO_PATH "nomatch.wav");
                    Result->WriteText ("Hit any key to proceed...");
                    ReadKey ();
                }
                
                delete Result;
                break;

            case USR_VERIFY:
                ProfileReader = new DialogBox (DIALOG_PATH "getprofile", 8, 10);
                Profile = ProfileReader->Engage ();

                if (Profile != NULL) {
                    TempBuffer = new char [strlen (DATABASE_PATH) +
                        MAX_PROFILE_LEN + 5];
                    strcpy (TempBuffer, DATABASE_PATH);
                    strcat (TempBuffer, Profile[0]);
                    strcat (TempBuffer, PROFILE_EXTENSION);

                    try {
                        RefProfile = new SpeechProfile (TempBuffer);
                    }
                    catch (SpeechProfileException SPE) {
                        Prompt = new InfoBox (" Whoops! ", 10, 10, 15, 70);
                        Prompt->MoveCursor (2, 1);
                        Prompt->AlignCentre ("An error occurred while loading the profile.");
                        Prompt->MoveCursor (4, 1);
                        PlayAudio (AUDIO_PATH "oops.wav");
                        PlayAudio (AUDIO_PATH "errorload.wav");
                        Prompt->WriteText ("Hit any key to proceed...");
                        ReadKey ();
                        delete Prompt;
                        delete[] TempBuffer;
                        delete ProfileReader;
                        break;
                    }

                    delete[] TempBuffer;

                    if (UserProfile->Distance (*RefProfile) < THRESHOLD) {
                        PersonalInfo& Record = RefProfile->PersonalRecord ();
                        Result = new InfoBox (" Match ", 9, 14, 18, 66);
                        Result->WriteText ("Search successful. You have been verified as");
                        Result->MoveCursor (3, 2);
                        Result->WriteText ("Name    : ");
                        Result->WriteText (Record.Name);
                        Result->MoveCursor (5, 2);
                        Result->WriteText ("Address : ");
                        Result->WriteText (Record.Address);
                        PlayAudio (AUDIO_PATH "verifysuccess.wav");
                        Result->MoveCursor (7, 1);
                        Result->WriteText ("Hit any key if you wish to update "
                            "your profile, or Esc to retain the old profile.");
    
                        if (ReadKey () != KEY_ESCAPE) {
                            UserInfo = RefProfile->PersonalRecord ();
                            UserProfile->PersonalRecord (UserInfo);
                            SpeechProfileArray->ExistsProfile (Profile[0],
                                &ProfileIndex);
                            SpeechProfileArray->UpdateProfile (*UserProfile,
                                ProfileIndex);
                        }

                    }
                    else {
                        Result = new InfoBox (" No Match ", 9, 27, 15, 57);
                        Result->MoveCursor (2, 1);
                        Result->AlignCentre ("Your profile could not be verified!");
                        PlayAudio (AUDIO_PATH "verifyfailure.wav");
                        Result->MoveCursor (4, 1);
                        Result->WriteText ("Hit any key to proceed...");
                        ReadKey ();
                    }
                    
                    delete Result;
                    delete RefProfile;
                }

                delete ProfileReader;
                break;
        }

        delete SpeechProfileArray;
        delete UserProfile;
    } while ((Choice != 0) && (Choice != USR_TERMINATE));
}
   
//*************************** USERMAINMENU.CPP ******************************
