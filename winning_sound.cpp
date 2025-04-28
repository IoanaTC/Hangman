#include <ao/ao.h>
#include <sndfile.h>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

extern "C" void play_winning_sound()
{
    int stderr_copy = dup(STDERR_FILENO);
    int devnull = open("/dev/null", O_WRONLY);
    if (devnull != -1) {
        dup2(devnull, STDERR_FILENO);
        close(devnull);
    }
    const char * filename = "./win_sound.wav";
    const int BUFFER_FRAMES = 4096;
    int driver = 0, frames = 0;
    ao_sample_format format;
    ao_device * device = nullptr;

    SF_INFO sfinfo;
    memset(&sfinfo, 0, sizeof(SF_INFO));
    
    SNDFILE * sndfile = sf_open(filename, SFM_READ, &sfinfo);
    short buffer[BUFFER_FRAMES * sfinfo.channels];

    if(!sndfile) {
        goto restore_stderr;
    }
    ao_initialize();
    driver = ao_default_driver_id();

    memset(&format, 0, sizeof(format));
    format.bits     = 16;
    format.channels = sfinfo.channels;
    format.rate     = sfinfo.samplerate;
    format.byte_format = AO_FMT_NATIVE;

    device = ao_open_live(driver, &format, NULL);
    if (!device) {
        sf_close(sndfile);
        ao_shutdown();
        goto restore_stderr;
    }

    while ((frames = sf_readf_short(sndfile, buffer, BUFFER_FRAMES)) > 0) {
        ao_play(device, (char*)buffer, frames * sfinfo.channels * sizeof(short));
    }

    ao_close(device);
    sf_close(sndfile);
    ao_shutdown();

restore_stderr:
    dup2(stderr_copy, STDERR_FILENO);
    close(stderr_copy);
    return;
}
