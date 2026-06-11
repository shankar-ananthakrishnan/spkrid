//***************************************************************************
//
// ALSAWAVEREADER.CPP
//
// Implementation of ALSAWaveReader: captures audio via ALSA.
// Linux replacement for WinWaveReader.
//
//***************************************************************************

#include <alsa/asoundlib.h>
#include "alsawavereader.hpp"

//***************************************************************************
ALSAWaveReader::ALSAWaveReader (unsigned int SampleRate, float Length)
    : AudioReader ("default") {
    NSampleRate = SampleRate;
    NResolution = 16;
    AudioLength = (unsigned int)(SampleRate * Length);
}

//***************************************************************************
void ALSAWaveReader::NewSampleRate (unsigned int NewRate) {
    NSampleRate = NewRate;
}

//***************************************************************************
void ALSAWaveReader::NewMediaLength (float NewLength) {
    AudioLength = (unsigned int)(NSampleRate * NewLength);
}

//***************************************************************************
Signal& ALSAWaveReader::AudioData (void) throw (AudioReaderException) {
    snd_pcm_t *handle;
    snd_pcm_hw_params_t *params;
    unsigned int rate;
    int dir;
    snd_pcm_uframes_t frames;
    signed short *buffer;
    unsigned int total;
    unsigned int i;
    int rc;

    rc = snd_pcm_open (&handle, "default", SND_PCM_STREAM_CAPTURE, 0);
    if (rc < 0) throw (ARX_NONEXISTENT);

    snd_pcm_hw_params_alloca (&params);
    snd_pcm_hw_params_any (handle, params);
    snd_pcm_hw_params_set_access (handle, params,
        SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format (handle, params,
        SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels (handle, params, 1);
    rate = NSampleRate;
    snd_pcm_hw_params_set_rate_near (handle, params, &rate, &dir);
    frames = 1024;
    snd_pcm_hw_params_set_period_size_near (handle, params, &frames, &dir);
    snd_pcm_hw_params (handle, params);

    buffer = new signed short [frames];
    AudioSignal->SetWriteMode ();

    total = 0;
    while (total < AudioLength) {
        unsigned int toread = (unsigned int)frames;
        if (total + toread > AudioLength) toread = AudioLength - total;
        rc = (int)snd_pcm_readi (handle, buffer, toread);
        if (rc < 0) {
            snd_pcm_recover (handle, rc, 0);
            continue;
        }
        for (i = 0; i < (unsigned int)rc; ++i) {
            AudioSignal->AddSample (buffer[i] / 32768.0f);
        }
        total += (unsigned int)rc;
    }

    delete[] buffer;
    snd_pcm_close (handle);

    AudioSignal->SetReadMode ();
    return (*AudioSignal);
}

//************************** ALSAWAVEREADER.CPP *****************************
