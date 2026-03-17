#include <stdio.h>

#include "winwavereader.hpp"
#include "../../audiowriter/wavewriter/pcm/wavepcmwriter.hpp"
#include "../../audiowriter/winwavewriter/winwavewriter.hpp"

int main (void) {
    AudioReader *WWR;
    AudioWriter *WWW;
    AudioWriter *Wr;

    printf ("Hit Enter to record...\n");
    getchar ();

    WWR = new WinWaveReader (11025, 15.0);
    Signal& Audio = WWR->AudioData ();

    printf ("Hit Enter to play...\n");
    getchar ();

    WWW = new WinWaveWriter (11025, 1);
    WWW->WriteAudio (Audio);

    Wr = new WavePCMWriter ("test.wav", 11025, 16, 1);
    Wr->WriteAudio (Audio);


    delete WWR;
    delete WWW;
    delete Wr;
}
