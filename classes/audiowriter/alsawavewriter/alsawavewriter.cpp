//***************************************************************************
//
// ALSAWAVEWRITER.CPP
//
// Implementation of ALSAWaveWriter: plays audio via ALSA.
// Linux replacement for WinWaveWriter.
//
//***************************************************************************

#include <alsa/asoundlib.h>
#include "alsawavewriter.hpp"

//***************************************************************************
ALSAWaveWriter::ALSAWaveWriter (unsigned int SampleRate,
    unsigned short int Channels) : AudioWriter ("default", SampleRate, 16) {
    NChannels = Channels;
}

//***************************************************************************
void ALSAWaveWriter::NewSampleRate (unsigned int NewRate) {
    NSampleRate = NewRate;
}

//***************************************************************************
void ALSAWaveWriter::NewNumberOfChannels (unsigned short int NewChannels) {
    NChannels = NewChannels;
}

//***************************************************************************
void ALSAWaveWriter::WriteAudio (Signal& AudioData)
    throw (AudioWriterException) {
    snd_pcm_t *handle;
    snd_pcm_hw_params_t *params;
    unsigned int rate;
    int dir;
    snd_pcm_uframes_t frames;
    signed short *buffer;
    unsigned int nsamples;
    unsigned int i;
    int rc;

    rc = snd_pcm_open (&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
    if (rc < 0) throw (AWX_NONEXISTENT);

    snd_pcm_hw_params_alloca (&params);
    snd_pcm_hw_params_any (handle, params);
    snd_pcm_hw_params_set_access (handle, params,
        SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format (handle, params,
        SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels (handle, params, NChannels);
    rate = NSampleRate;
    snd_pcm_hw_params_set_rate_near (handle, params, &rate, &dir);
    frames = 1024;
    snd_pcm_hw_params_set_period_size_near (handle, params, &frames, &dir);
    snd_pcm_hw_params (handle, params);

    nsamples = AudioData.SampleCount ();
    buffer = new signed short [frames];

    unsigned int offset = 0;
    while (offset < nsamples) {
        unsigned int chunk = (unsigned int)frames;
        if (offset + chunk > nsamples) chunk = nsamples - offset;
        for (i = 0; i < chunk; ++i) {
            float s = AudioData[offset + i];
            if (s > 1.0f) s = 1.0f;
            if (s < -1.0f) s = -1.0f;
            buffer[i] = (signed short)(s * 32767.0f);
        }
        rc = (int)snd_pcm_writei (handle, buffer, chunk);
        if (rc < 0) snd_pcm_recover (handle, rc, 0);
        offset += chunk;
    }

    delete[] buffer;
    snd_pcm_drain (handle);
    snd_pcm_close (handle);
}

//************************** ALSAWAVEWRITER.CPP *****************************
