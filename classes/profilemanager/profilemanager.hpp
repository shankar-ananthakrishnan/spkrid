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
// PROFILEMANAGER.HPP                                                       *
//                                                                          *
// Header file for ProfileManager, which manages speech profiles in a disk  *
// database.                                                                *
// Fundamental component of the Speaker Identification Project.             *
//                                                                          *
// Last modified on April 25, 2002.                                         *
//                                                                          *
//***************************************************************************

// The ProfileManager allows a client to access the speech profile database
// as an array indexed by average pitch period. This allows fast binary
// searches (or derivatives thereof) on the database. Besides the overloaded
// operator to make the database look like a sorted array, the class also
// provides several methods to manage the database (incl. addition, deletion,
// purging, updation, import and export)

#ifndef __PROFILEMANAGER_HPP
#define __PROFILEMANAGER_HPP

#include "../globalconstants.hpp"
#include "../speechprofile/speechprofile.hpp"

#define     PROFILE_EXTENSION   ".prf"

enum ProfileManagerException {
    PM_BOUNDS_ERROR,
    PM_PROFILE_ERROR,
    PM_PROFILE_EXISTS,
    PM_DELETE_FAILURE,
    PM_PURGE_FAILURE,
    PM_EXPORT_FAILURE,
    PM_UPDATE_FAILURE,
    PM_IMPORT_FAILURE
};

//***************************************************************************
class ProfileManager {
private:
    struct IndexEntry {                         // entry in index file
        char ProfileName[MAX_PROFILE_LEN + 1];
        unsigned int ArrayIndex;
        float AveragePitch;
    };

    char *IndexFileName;                        // local persistence
    char *ProfilePath;
    unsigned int NProfiles;

public:
    ProfileManager (char *IndexFile, char *DataPath);
    ~ProfileManager ();

    SpeechProfile *operator[] (unsigned int Index)
        throw (ProfileManagerException);

    unsigned int ProfileCount (void);

// Self-explanatory messages that can be used to manipulate the database
// of speech profiles.

    bool ExistsProfile (char *SpecProfileName, unsigned int *Index);

    void AddProfile (SpeechProfile& NewProfile, char *SpecProfileName)
        throw (ProfileManagerException);

    void DeleteProfile (char *SpecProfileName)
        throw (ProfileManagerException);

    void PurgeProfile (char *SpecProfileName)
        throw (ProfileManagerException);

    void ExportProfile (char *SourceProfile, char *TargetProfile)
        throw (ProfileManagerException);

    void ImportProfile (char *SourceProfile, char *TargetProfile);

    void UpdateProfile (SpeechProfile& NewProfile, unsigned int Index)
        throw (ProfileManagerException);
};

#endif

//************************** PROFILEMANAGER.HPP *****************************
