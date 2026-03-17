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
// WINWAVEREADER.HPP                                                        *
//                                                                          *
// Header file for `WinWaveReader', a class which reads audio through a     *
// Windows device driver. Uses Win32 API calls.                             *
// Fundamental component of the Speaker Identification Project.             *
//                                                                          *
// Last modified on March 15, 2002.                                         *
//                                                                          *
//***************************************************************************

#ifndef __WINWAVEREADER_HPP
#define __WINWAVEREADER_HPP

#include "../audioreader.hpp"

//***************************************************************************
class WinWaveReader : public AudioReader {
private:
    unsigned int AudioLength;

public:
    WinWaveReader (unsigned int SampleRate, float Length);

    void NewSampleRate (unsigned int NewRate);
    void NewMediaLength (float NewLength);

    virtual Signal& AudioData (void)
        throw (AudioReaderException);
};

#endif

//***************************************************************************
