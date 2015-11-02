#include <iostream>
#include <string>
#include <portaudio.h>
#include <fstream>
#include <libopenmpt_stream_callbacks_file.h>
#include <cstdint>
#include <thread>
#include <chrono>
#include <sstream>
#include <libopenmpt.h>
using namespace std;

void playsong(char *filename[], int argcount)
{
    int interpol = 1;
    int ss = 100;
    int repeattimes = -1;
    bool forky = false;
    const int BUFFERSIZE = 480;
    const int SAMPLERATE = 48000;
    static int16_t left[BUFFERSIZE];
    static int16_t right[BUFFERSIZE];
    static int16_t * const buffers[2] = { left, right };
    FILE * file = 0;
    stringstream arg;
    string bob;
    if(filename[1] != NULL)
    {

    arg << filename[1];


    arg >> bob;

    }
    else
    {
       cout << "USAGE: module-decoder <filename> <args>" << endl;
    }
    for(int i=2;i < argcount;i++)
    {
        arg.str("");
        arg.clear();
        arg << filename[i];
        string s = "";
        arg >> s;

        if(s == "i")
        {
            arg.str("");
            arg.clear();
            arg << filename[i + 1];
            s = "";
            arg >> s;

            interpol = atoi(s.c_str());
            i++;
        }
        if(s == "ss")
        {
            arg.str("");
            arg.clear();
            arg << filename[i + 1];
            s = "";
            arg >> s;

            ss = atoi(s.c_str());
            i++;
        }
        if(s == "r")
        {
            arg.str("");
            arg.clear();
            arg << filename[i + 1];
            s = "";
            arg >> s;

            repeattimes = atoi(s.c_str());
            i++;
        }
        if(s == "fork")
        {
           pid_t pid;
           pid = fork();
           if(pid <0)
           {
               exit(EXIT_FAILURE);
           }
           if (pid > 0)
           {
               exit(EXIT_SUCCESS);
           }
           //pid_t sid;
          //sid = getsid();
           fclose(stdin);
           fclose(stdout);
           fclose(stderr);
           forky = true;
        }

    }
    openmpt_module * mod = 0;
    size_t count = 0;
    PaStream * stream = 0;
    if(!bob.empty())
    {
     fclose(stdout);
     fclose(stderr);
    Pa_Initialize();
    if(forky == false)
    {
    freopen("/dev/tty", "w", stdout);
    freopen("/dev/tty", "w", stderr);
    }
    Pa_OpenDefaultStream(&stream, 0, 2, paInt16 | paNonInterleaved, SAMPLERATE, paFramesPerBufferUnspecified, NULL, NULL);
    Pa_StartStream(stream);

    file = fopen(bob.c_str(), "rb");
    if(file != NULL)
    {
        if(openmpt_could_open_propability(openmpt_stream_get_file_callbacks(),file,1,NULL,NULL) >= 0.4)
        {
    mod = openmpt_module_create(openmpt_stream_get_file_callbacks(), file, NULL, NULL, NULL);
        openmpt_module_set_render_param(mod, OPENMPT_MODULE_RENDER_INTERPOLATIONFILTER_LENGTH, interpol);
        openmpt_module_set_render_param(mod, OPENMPT_MODULE_RENDER_STEREOSEPARATION_PERCENT, ss);
        openmpt_module_set_repeat_count(mod,repeattimes);
        int numtimes = 0;
        fclose(file);
        while (1) {
            system("clear");

            int numsecs = openmpt_module_get_position_seconds(mod) - int(openmpt_module_get_position_seconds(mod) / 60) - (numtimes * 59);
            if(numsecs != 0 && numsecs % 59 == 0)
            {
                ++numtimes;
            }
            cout << int(openmpt_module_get_position_seconds(mod) / 60) << ":" << numsecs << endl;
            cout << "SONG: " << bob << endl;
            count = openmpt_module_read_stereo(mod, SAMPLERATE, BUFFERSIZE, left, right);
            if (count == 0) {
                //goto recursive;
                if(openmpt_module_get_repeat_count(mod) == 0)
                 {
                    break;
                }
                else
                count = openmpt_module_read_stereo(mod, SAMPLERATE, BUFFERSIZE, left, right);
                //break;
            }
            Pa_WriteStream(stream, buffers, (unsigned long)count);
        }

        Pa_StopStream(stream);
        Pa_CloseStream(stream);
        Pa_Terminate();
        openmpt_module_destroy(mod);
    }
    else
    {
       cout << "Not a valid module." << endl;
    }
    }
    }

}




int main(int argc, char *argv[])
{

    playsong(argv, argc);

    return 0;
}

