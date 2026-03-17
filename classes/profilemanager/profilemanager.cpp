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
// PROFILEMANAGER.CPP                                                       *
//                                                                          *
// Implementation of ProfileManager, which manages speech profiles in a     *
// disk database.                                                           *
// Fundamental component of the Speaker Identification Project.             *
//                                                                          *
// Last modified on April 25, 2002.                                         *
//                                                                          *
//***************************************************************************

#include <stdio.h>
#include <string.h>

#include "../globalconstants.hpp"
#include "../speechprofile/speechprofile.hpp"
#include "profilemanager.hpp"

#define     INDEX_ENTRY_SIZE    sizeof(IndexEntry)
#define     WORK_TEMP_FILE      "pindex.tmp"

//***************************************************************************
ProfileManager::ProfileManager (char *IndexFile,
    char *DataPath) {
    FILE *IndFile;

    IndexFileName = new char [strlen (IndexFile) + 1];
    ProfilePath = new char [strlen (DataPath) + 1];

    strcpy (IndexFileName, IndexFile);      // save index file name
    strcpy (ProfilePath, DataPath);         //   and profile path

    IndFile = fopen (IndexFileName, "rb");  // find number of profiles

    if (IndFile == NULL) {
        IndFile = fopen (IndexFileName, "wb");
        NProfiles = 0;
        fclose (IndFile);
    }
    else {
        fseek (IndFile, 0, SEEK_END);
        NProfiles = ftell (IndFile) / INDEX_ENTRY_SIZE;
        fclose (IndFile);
    }
}

//***************************************************************************
ProfileManager::~ProfileManager () {
    delete[] IndexFileName;
    delete[] ProfilePath;
}

//***************************************************************************
unsigned int ProfileManager::ProfileCount (void) {
    return NProfiles;
}

//***************************************************************************
// `[]' allows the speech profiles in the database to be treated as a       *
// sorted (by average pitch) array of profiles that can be accessed by an   *
// "array index". A new `SpeechProfile' is created everytime [] is called;  *
// it is the duty of the caller to `delete' these `SpeechProfile's.         *
//***************************************************************************
SpeechProfile *ProfileManager::operator[] (unsigned int Index)
    throw (ProfileManagerException) {
    unsigned int Record;
    FILE *IndexFile;
    SpeechProfile *Profile;
    IndexEntry IEntry;
    char *TempBuffer;

    if (Index >= NProfiles) {           // number of profiles
        throw (PM_BOUNDS_ERROR);        // out of bounds index
    }

    IndexFile = fopen (IndexFileName, "rb");   // open the index file
    Record = 0;
    IEntry.ArrayIndex = 0xFFFFFFFF;     // an impossible value    

    while ((Record++ < NProfiles) && (Index != IEntry.ArrayIndex)) {
        fread (&IEntry, INDEX_ENTRY_SIZE, 1, IndexFile);
    }                                   // loop till index found

    fclose (IndexFile);
    TempBuffer = new char [strlen (ProfilePath) + MAX_PROFILE_LEN + 6];
    strcpy (TempBuffer, ProfilePath);
    strcat (TempBuffer, IEntry.ProfileName);
    strcat (TempBuffer, PROFILE_EXTENSION);

    Profile = new SpeechProfile (TempBuffer);
    delete[] TempBuffer;

    return (Profile);
}

//***************************************************************************
void ProfileManager::AddProfile (SpeechProfile& NewProfile,
    char *NewProfileName) throw (ProfileManagerException) {
    FILE *IndexFile;
    FILE *WorkFile;
    unsigned int Record;
    IndexEntry IEntry;
    float AvgPitch;
    float PrevPitch;                    // to determine location to insert
    unsigned int Insert;                // insertion index
    char *TempBuffer;
    unsigned int Dummy;

    if (ExistsProfile (NewProfileName, &Dummy)) {
        throw (PM_PROFILE_EXISTS);
    }

    IndexFile = fopen (IndexFileName, "rb");
    WorkFile = fopen (WORK_TEMP_FILE, "wb");

    PrevPitch = 0.0;
    AvgPitch = NewProfile.AveragePitch ();
    Insert = 0;                         // initial insert locn. assumption

    for (Record = 0; Record < NProfiles; ++Record) {
        fread (&IEntry, INDEX_ENTRY_SIZE, 1, IndexFile);

// We find here the largest pitch that is still smaller than the profile
// that we wish to add to the profile database. If the read average pitch
// is larger than the average pitch of the profile we are adding, increment
// its index by 1. Do this till the end of the index file is reached.

        if (IEntry.AveragePitch > AvgPitch) {
            ++IEntry.ArrayIndex;
        }
        else if ((IEntry.AveragePitch > PrevPitch) &&
            (IEntry.AveragePitch < AvgPitch)) {
            Insert = IEntry.ArrayIndex + 1;
            PrevPitch = IEntry.AveragePitch;
        }

        fwrite (&IEntry, INDEX_ENTRY_SIZE, 1, WorkFile);
    }

// Initialize the IndexEntry structure with information from the new profile
// and increment the number of profiles by 1. Write it to the end of the
// index file. Write also the profile to disk.

    strcpy (IEntry.ProfileName, NewProfileName);
    IEntry.ArrayIndex = Insert;
    IEntry.AveragePitch = AvgPitch;
    ++NProfiles;                        // increment number of profiles

    TempBuffer = new char [strlen (ProfilePath) + MAX_PROFILE_LEN + 6];
    strcpy (TempBuffer, ProfilePath);
    strcat (TempBuffer, NewProfileName);
    strcat (TempBuffer, PROFILE_EXTENSION);
    NewProfile.SaveProfile (TempBuffer);    // save profile
    delete[] TempBuffer;

    fwrite (&IEntry, INDEX_ENTRY_SIZE, 1, WorkFile);
    fclose (IndexFile);
    fclose (WorkFile);

    remove (IndexFileName);
    rename (WORK_TEMP_FILE, IndexFileName);
}

//***************************************************************************
void ProfileManager::DeleteProfile (char *SpecProfileName)
    throw (ProfileManagerException) {
    FILE *IndexFile;                    // index file pointer
    FILE *WorkFile;                     // for temp. work file
    unsigned int Record;
    unsigned int DeletedIndex;          // index of deleted profile
    IndexEntry IEntry;

    if (!ExistsProfile (SpecProfileName, &DeletedIndex)) {
         throw (PM_DELETE_FAILURE);     // profile not found
    }

    IndexFile = fopen (IndexFileName, "rb");    // open index file
    WorkFile = fopen (WORK_TEMP_FILE, "wb");    // temp work file

    for (Record = 0; Record < NProfiles; ++Record) {
        fread (&IEntry, INDEX_ENTRY_SIZE, 1, IndexFile);

// Scan the index file; if we find any entries with index values exceeding
// the deleted index value, decrement them. Also, we do not copy the index
// that corresponds to the deleted entry.

        if (IEntry.ArrayIndex != DeletedIndex) {
            if (IEntry.ArrayIndex > DeletedIndex) {
                --IEntry.ArrayIndex;
            }
    
            fwrite (&IEntry, INDEX_ENTRY_SIZE, 1, WorkFile);
        }
    }

    fclose (IndexFile);
    fclose (WorkFile);
    --NProfiles;                        // reduce number of profiles

    remove (IndexFileName);
    rename (WORK_TEMP_FILE, IndexFileName);
}

//***************************************************************************
void ProfileManager::PurgeProfile (char *SpecProfileName)
    throw (ProfileManagerException) {
    char *TempBuffer;

    try {
        DeleteProfile (SpecProfileName);    // delete the profile
    }
    catch (...) {                       // profile may (not) exist, so we
    }                                   //   have to take care of exception

    TempBuffer = new char [strlen (ProfilePath) + MAX_PROFILE_LEN + 6];
    strcpy (TempBuffer, ProfilePath);       // construct full path
    strcat (TempBuffer, SpecProfileName);   //   for profile
    strcat (TempBuffer, PROFILE_EXTENSION);

    if (remove (TempBuffer) != 0) {     // remove the profile from disk
        throw (PM_PURGE_FAILURE);
    }

    delete[] TempBuffer;
}

//***************************************************************************
void ProfileManager::ExportProfile (char *SourceProfile,
    char *TargetProfile) throw (ProfileManagerException) {
    char *SrcBuffer;
    char *DstBuffer;
    SpeechProfile *Export;

    SrcBuffer = new char [strlen (ProfilePath) + MAX_PROFILE_LEN + 6];
    DstBuffer = new char [strlen (ProfilePath) + MAX_PROFILE_LEN + 6];

    strcat (strcat (strcpy (SrcBuffer, ProfilePath),
        SourceProfile), PROFILE_EXTENSION);
    strcat (strcpy (DstBuffer, TargetProfile), PROFILE_EXTENSION);

    Export = new SpeechProfile (SrcBuffer);
    Export->SaveProfile (DstBuffer);

    delete Export;
    delete SrcBuffer;
    delete DstBuffer;
}

//***************************************************************************
void ProfileManager::ImportProfile (char *SourceProfile,
    char *TargetProfile) {
    SpeechProfile *Imported;
    char *TempBuffer;

    TempBuffer = new char [MAX_PROFILE_LEN + 5];
    strcpy (TempBuffer, SourceProfile);
    strcat (TempBuffer, PROFILE_EXTENSION);

    try {
        Imported = new SpeechProfile (TempBuffer);  // read profile
    }
    catch (SpeechProfileException SPE) {
        throw (PM_IMPORT_FAILURE);
    }

    AddProfile (*Imported, TargetProfile);      // add it to our database
    delete TempBuffer;
    delete Imported;
}

//***************************************************************************
void ProfileManager::UpdateProfile (SpeechProfile& NewProfile,
    unsigned int Index) throw (ProfileManagerException) {
    unsigned int Record;
    IndexEntry IEntry;
    FILE *IndexFile;

    if (Index >= NProfiles) {
        throw (PM_UPDATE_FAILURE);
    }

    IndexFile = fopen (IndexFileName, "rb");   // open the index file
    Record = 0;
    IEntry.ArrayIndex = 0xFFFFFFFF;     // an impossible value    

    while ((Record++ < NProfiles) && (Index != IEntry.ArrayIndex)) {
        fread (&IEntry, INDEX_ENTRY_SIZE, 1, IndexFile);
    }                                   // loop till index found

    fclose (IndexFile);

    PurgeProfile (IEntry.ProfileName);
    AddProfile (NewProfile, IEntry.ProfileName);
}

//***************************************************************************
bool ProfileManager::ExistsProfile (char *SpecProfileName,
    unsigned int *Index) {
    FILE *IndexFile;
    unsigned int Record;
    IndexEntry IEntry;
    bool Exists;

    IndexFile = fopen (IndexFileName, "rb");
    Exists = false;                     // assume doesn't exist

    for (Record = 0; (Record < NProfiles) && (!Exists); ++Record) {
        fread (&IEntry, INDEX_ENTRY_SIZE, 1, IndexFile);

        if (strcmp (IEntry.ProfileName, SpecProfileName) == 0) {
            Exists = true;              // yes! profile exists
            *Index = IEntry.ArrayIndex;
        }
    }

    fclose (IndexFile);
    return (Exists);                    // return status
}

//************************** PROFILEMANAGER.CPP *****************************
