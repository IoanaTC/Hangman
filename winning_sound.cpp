#include <ao/ao.h>
#include <sndfile.h>
#include <cstring>

using namespace std;

extern "C" void play_winning_sound()
{
    const char * filename = "./win_sound.wav";
    
    SF_INFO sfinfo;
    memset(&sfinfo, 0, sizeof(SF_INFO));
    
    SNDFILE * sndfile = sf_open(filename, SFM_READ, &sfinfo);
    if(!sndfile) {
        return;
    }
    ao_initialize();
    int driver = ao_default_driver_id();

    ao_sample_format format;
    memset(&format, 0, sizeof(format));
    format.bits     = 16;
    format.channels = sfinfo.channels;
    format.rate     = sfinfo.samplerate;
    format.byte_format = AO_FMT_NATIVE;

    ao_device* device = ao_open_live(driver, &format, NULL);
    if (!device) {
        sf_close(sndfile);
        ao_shutdown();
        return;
    }

    const int BUFFER_FRAMES = 4096;
    short buffer[BUFFER_FRAMES * sfinfo.channels];
    int frames;
    while ((frames = sf_readf_short(sndfile, buffer, BUFFER_FRAMES)) > 0) {
        ao_play(device, (char*)buffer, frames * sfinfo.channels * sizeof(short));
    }

    ao_close(device);
    sf_close(sndfile);
    ao_shutdown();
}
