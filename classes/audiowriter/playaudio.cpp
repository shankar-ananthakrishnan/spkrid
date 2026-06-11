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
// PLAYAUDIO.CPP                                                            *
//                                                                          *
// PlayAudio () plays an audio clip from a WAVE audio file.                 *
// Cosmetic user interface component of the Speaker Identification System.  *
//                                                                          *
// Last modified on April 25, 2002.                                         *
//                                                                          *
//***************************************************************************

#include "../audioreader/wavereader/pcm/wavepcmreader.hpp"
#include "alsawavewriter/alsawavewriter.hpp"

// #define     AUDIO_PROMPTS

//***************************************************************************
void PlayAudio (char *Source) {
#ifdef AUDIO_PROMPTS
    WaveReader *WAVESrc;
    ALSAWaveWriter *WAVEDst;

    WAVESrc = new WavePCMReader (Source);
    WAVEDst = new ALSAWaveWriter (WAVESrc->SampleRate (),
        WAVESrc->ChannelCount ());

    Signal& Audio = WAVESrc->AudioData ();      // read source audio
    WAVEDst->WriteAudio (Audio);                // playback!

    delete WAVESrc;
    delete WAVEDst;
#endif
}

//***************************** PLAYAUDIO.CPP *******************************
