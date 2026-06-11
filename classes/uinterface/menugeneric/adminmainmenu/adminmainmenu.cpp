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
// ADMINMAINMENU.CPP                                                        *
//                                                                          *
// Implementation of class `AdminMainMenu', which consolidates the various  *
// components of the speaker id project accessible through a central menu.  *
// Key component of the administrator module.                               *
//                                                                          *
// Last modified on April 20, 2002.                                         *
//                                                                          *
//***************************************************************************

#include <string.h>

#include "../../../globalconstants.hpp"
#include "../../../audioreader/alsawavereader/alsawavereader.hpp"
#include "../../../audiowriter/playaudio.hpp"
#include "../../../silenceremover/silenceremover.hpp"
#include "../../../profilemanager/profilemanager.hpp"
#include "../../../speechprofile/speechprofile.hpp"
#include "../../console/console.hpp"
#include "../../console/keycode.hpp"
#include "../../infobox/infobox.hpp"
#include "../../dialogbox/dialogbox.hpp"
#include "../menugeneric.hpp"
#include "adminmainmenu.hpp"

#define    ADM_ADDPROFILE           1
#define    ADM_DELETEPROFILE        2
#define    ADM_PURGEPROFILE         3
#define    ADM_VIEWPROFILE          4
#define    ADM_EXPORTPROFILE        5
#define    ADM_IMPORTPROFILE        6
#define    ADM_TERMINATE            7

//***************************************************************************
AdminMainMenu::AdminMainMenu (unsigned int Y,
    unsigned int X) : MenuGeneric (MENU_PATH "adminmainmenu", Y, X) {}

//***************************************************************************
void AdminMainMenu::Execute (void) {
    unsigned int Choice;
    ProfileManager *SpeechProfileArray;
    DialogBox *DlgProfile;
    AudioReader *Reader;
    AudioReader *SReader;
    PersonalInfo UserInfo;
    InfoBox *Prompt;
    InfoBox *Status;
    InfoBox *PView;
    char **Result;
    SpeechProfile *UserProfile;
    Signal *ClippedAudio;
    SilenceRemover *Clipper;
    bool Exists;
    bool InsuffAudio;
    unsigned int Dummy;
    unsigned int Key;
    char *TempBuffer;

    SpeechProfileArray =
        new ProfileManager (DATABASE_PATH DATABASE_INDEX, DATABASE_PATH);

    do {
        Choice = Interact();
      
        switch (Choice) {
            case ADM_ADDPROFILE:
                DlgProfile = new DialogBox (DIALOG_PATH "addprofile", 10, 15);

                do {
                    if ((Result = DlgProfile->Engage ()) != NULL) {
                        Exists =
                            SpeechProfileArray->ExistsProfile (Result[2],
                                &Dummy);
                        if (Exists) {
                            Prompt = new InfoBox (" Whoops! ", 10, 10, 15, 70);
                            Prompt->MoveCursor (2, 2);
                            Prompt->AlignCentre ("This profile already exists. "
                                "Please choose another name.");
                            Prompt->MoveCursor (4, 2);
                            PlayAudio (AUDIO_PATH "oops.wav");
                            // PlayAudio (AUDIO_PATH "errorexists.wav");
                            Prompt->WriteText ("Hit any key to edit profile "
                                "details...");
                            ReadKey ();
                            delete Prompt;
                        }
                    }
                } while (Exists && (Result != NULL));

                if (Result != NULL) {
                    strcpy (UserInfo.Name, Result[0]);
                    strcpy (UserInfo.Address, Result[1]);
    
                    Prompt = new InfoBox (" InfoBox ", 10, 12, 16, 75);
                    Prompt->WriteText ("The program will now take a sample of "
                        "the ambient noise level. Please make sure that there "
                        "is no disturbance till the next instruction appears.");
                    Prompt->MoveCursor (5, 1);
                    PlayAudio (AUDIO_PATH "usersilencenotify.wav");
                    Prompt->WriteText ("Hit any key to proceed (ESC Cancels)...");

                    Key = ReadKey ();
                    delete Prompt;

                    if (Key == KEY_ESCAPE) {
                        delete DlgProfile;
                        continue;
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
                    PlayAudio (AUDIO_PATH "useraudioprompt.wav");
                    Prompt->WriteText ("Hit any key to begin recording (ESC cancels)...");

                    Key = ReadKey ();
                    delete Prompt;
                    if (Key == KEY_ESCAPE) continue;

                    Status = new InfoBox (" InfoBox ", 10, 27, 14, 50);
                    Status->MoveCursor (2, 3);
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
                            Prompt->WriteText ("There was not sufficient "
                                "audio to work with. Please retry recording "
                                "the vowels a, e, i, o, u clearly and "
                                "distinctly.");
                            Prompt->MoveCursor (4, 1);
                            PlayAudio (AUDIO_PATH "userinsuffaudio.wav");
                            Prompt->WriteText ("Hit any key to begin recording...");
                            ReadKey ();
                            delete Prompt;
                        }
                    } while (InsuffAudio);

                    delete Clipper;

                    Status->WipeClean ();
                    Status->MoveCursor (2, 3);
                    Status->WriteText ("Building profile...");

                    UserProfile = new SpeechProfile (*ClippedAudio, UserInfo);
                    SpeechProfileArray->AddProfile (*UserProfile, Result[2]);
                    delete ClippedAudio;

                    delete Status;
                    delete UserProfile;
                }

                delete DlgProfile;

                break;

            case ADM_DELETEPROFILE:
                DlgProfile = new DialogBox (DIALOG_PATH "getprofile", 10, 15);
                try {
                    if ((Result = DlgProfile->Engage ()) != NULL) {
                        SpeechProfileArray->DeleteProfile (Result[0]);
                    }
                }
                catch (ProfileManagerException PME) {
                    Prompt = new InfoBox (" Whoops! ", 10, 10, 15, 70);
                    Prompt->MoveCursor (2, 1);
                    Prompt->AlignCentre ("An error occurred while deleting the profile.");
                    Prompt->MoveCursor (4, 1);
                    PlayAudio (AUDIO_PATH "oops.wav");
                    PlayAudio (AUDIO_PATH "errordelete.wav");
                    Prompt->WriteText ("Hit any key to proceed...");
                    ReadKey ();
                    delete Prompt;
                }

                delete DlgProfile;
                break;

            case ADM_PURGEPROFILE:
                DlgProfile = new DialogBox (DIALOG_PATH "getprofile", 10, 15);
                try {
                    if ((Result = DlgProfile->Engage ()) != NULL) {
                        SpeechProfileArray->PurgeProfile (Result[0]);
                    }
                }
                catch (ProfileManagerException PME) {
                    Prompt = new InfoBox (" Whoops! ", 10, 10, 15, 70);
                    Prompt->MoveCursor (2, 1);
                    Prompt->AlignCentre ("An error occurred while purging the profile.");
                    Prompt->MoveCursor (4, 1);
                    PlayAudio (AUDIO_PATH "oops.wav");
                    PlayAudio (AUDIO_PATH "errorpurge.wav");
                    Prompt->WriteText ("Hit any key to proceed...");
                    ReadKey ();
                    delete Prompt;
                }

                delete DlgProfile;
                break;
                
            case ADM_VIEWPROFILE:
                DlgProfile = new DialogBox (DIALOG_PATH "getprofile", 10, 15);
                if ((Result = DlgProfile->Engage ()) != NULL) {
                    TempBuffer = new char [strlen (DATABASE_PATH) +
                        MAX_PROFILE_LEN + 5];
                    strcpy (TempBuffer, DATABASE_PATH);
                    strcat (TempBuffer, Result[0]);
                    strcat (TempBuffer, PROFILE_EXTENSION);

                    try {
                        UserProfile = new SpeechProfile (TempBuffer);
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
                        delete DlgProfile;
                        continue;
                    }

                    delete[] TempBuffer;

                    PersonalInfo& Record = UserProfile->PersonalRecord ();
                    PView = new InfoBox (" InfoBox ", 10, 15, 17, 65);
                    PView->MoveCursor (2, 1);
                    PView->WriteText ("Name    : ");
                    PView->WriteText (Record.Name);
                    PView->MoveCursor (4, 1);
                    PView->WriteText ("Address : ");
                    PView->WriteText (Record.Address);
                    PView->MoveCursor (6, 1);
                    PView->WriteText ("Hit any key to proceed...");
                    ReadKey ();
                    delete PView;
                    delete UserProfile;
                }

                delete DlgProfile;
                break;
                
            case ADM_EXPORTPROFILE:
                DlgProfile = new DialogBox (DIALOG_PATH "ixprofile", 10, 15);
                try {
                    if ((Result = DlgProfile->Engage ()) != NULL) {
                        SpeechProfileArray->ExportProfile (Result[0], Result[1]);
                    }
                }
                catch (ProfileManagerException PME) {
                    Prompt = new InfoBox (" Whoops! ", 10, 10, 15, 70);
                    Prompt->MoveCursor (2, 1);
                    Prompt->AlignCentre ("An error occurred while exporting the profile.");
                    Prompt->MoveCursor (4, 1);
                    PlayAudio (AUDIO_PATH "oops.wav");
                    PlayAudio (AUDIO_PATH "errorexport.wav");
                    Prompt->WriteText ("Hit any key to proceed...");
                    ReadKey ();
                    delete Prompt;
                }

                delete DlgProfile;
                break;

            case ADM_IMPORTPROFILE:
                DlgProfile = new DialogBox (DIALOG_PATH "ixprofile", 10, 15);
                try {
                    if ((Result = DlgProfile->Engage ()) != NULL) {
                        SpeechProfileArray->ImportProfile (Result[0], Result[1]);
                    }
                }
                catch (ProfileManagerException PME) {
                    Prompt = new InfoBox (" Whoops! ", 10, 10, 15, 70);
                    Prompt->MoveCursor (2, 1);
                    Prompt->AlignCentre ("An error occurred while importing the profile.");
                    Prompt->MoveCursor (4, 1);
                    PlayAudio (AUDIO_PATH "oops.wav");
                    PlayAudio (AUDIO_PATH "errorimport.wav");
                    Prompt->WriteText ("Hit any key to proceed...");
                    ReadKey ();
                    delete Prompt;
                }

                delete DlgProfile;
                break;
        }
    } while (Choice != 0 && Choice != ADM_TERMINATE);

    delete SpeechProfileArray;
}

//**************************** ADMINMAINMENU.CPP ****************************
