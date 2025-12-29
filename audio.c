#define COBJMACROS
#include <initguid.h>
#include <windows.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>

#include "sound_tracks.h"
#include "audio.h"

static pthread_t sound_thread;
static pthread_mutex_t sound_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t sound_cond = PTHREAD_COND_INITIALIZER;

static Sound sound_queue[SOUND_QUEUE_SIZE];
static int queue_head = 0;
static int queue_tail = 0;
static int queue_count = 0;

static bool audio_running = true;

static IAudioClient* audioClient = NULL;
static IAudioRenderClient* renderClient = NULL;
static WAVEFORMATEX* waveFormat = NULL;
static UINT32 bufferFrameCount = 0;
static HANDLE audioEvent = NULL;
static UINT32 frameSize = 0;

static int audio_init(void)
{
    HRESULT hr;
    IMMDeviceEnumerator* enumerator = NULL;
    IMMDevice* device = NULL;
    REFERENCE_TIME defaultPeriod;

    CoCreateInstance(
        &CLSID_MMDeviceEnumerator,
        NULL,
        CLSCTX_ALL,
        &IID_IMMDeviceEnumerator,
        (void**)&enumerator);

    IMMDeviceEnumerator_GetDefaultAudioEndpoint(
        enumerator, eRender, eConsole, &device);

    IMMDevice_Activate(
        device, &IID_IAudioClient,
        CLSCTX_ALL, NULL,
        (void**)&audioClient);

    IAudioClient_GetMixFormat(audioClient, &waveFormat);
    IAudioClient_GetDevicePeriod(audioClient, &defaultPeriod, NULL);

    hr = IAudioClient_Initialize(
        audioClient,
        AUDCLNT_SHAREMODE_SHARED,
        AUDCLNT_STREAMFLAGS_EVENTCALLBACK,
        defaultPeriod * 2,
        0,
        waveFormat,
        NULL);
    if (FAILED(hr)) return -1;

    IAudioClient_GetBufferSize(audioClient, &bufferFrameCount);

    IAudioClient_GetService(
        audioClient,
        &IID_IAudioRenderClient,
        (void**)&renderClient);

    audioEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    IAudioClient_SetEventHandle(audioClient, audioEvent);

    frameSize = waveFormat->nBlockAlign;

    IMMDeviceEnumerator_Release(enumerator);
    IMMDevice_Release(device);

    return 0;
}

static void audio_shutdown(void)
{
    if (audioEvent) {
        CloseHandle(audioEvent);
        audioEvent = NULL;
    }

    if (renderClient) {
        IAudioRenderClient_Release(renderClient);
        renderClient = NULL;
    }

    if (audioClient) {
        IAudioClient_Release(audioClient);
        audioClient = NULL;
    }

    if (waveFormat) {
        CoTaskMemFree(waveFormat);
        waveFormat = NULL;
    }
}

static void play_tone(double frequency, double duration)
{
    BYTE* data;
    UINT32 padding;
    UINT32 framesToWrite;
    double sampleRate = waveFormat->nSamplesPerSec;
    UINT32 totalFrames = (UINT32)(duration * sampleRate);
    UINT32 frameIndex = 0;

    IAudioRenderClient_GetBuffer(renderClient, bufferFrameCount, &data);
    IAudioRenderClient_ReleaseBuffer(renderClient, bufferFrameCount, 0);

    IAudioClient_Start(audioClient);

    while (frameIndex < totalFrames && audio_running)
    {
        WaitForSingleObject(audioEvent, INFINITE);

        IAudioClient_GetCurrentPadding(audioClient, &padding);
        framesToWrite = bufferFrameCount - padding;
        if (framesToWrite == 0)
            continue;

        if (framesToWrite > totalFrames - frameIndex)
            framesToWrite = totalFrames - frameIndex;

        IAudioRenderClient_GetBuffer(
            renderClient,
            framesToWrite,
            &data);

        float* samples = (float*)data;
        for (UINT32 i = 0; i < framesToWrite; i++)
        {
            double t = (frameIndex + i) / sampleRate;
            float s = (float)sin(2.0 * 3.141592653589 * frequency * t);

            for (UINT32 ch = 0; ch < waveFormat->nChannels; ch++)
                *samples++ = s;
        }

        IAudioRenderClient_ReleaseBuffer(
            renderClient,
            framesToWrite,
            0);

        frameIndex += framesToWrite;
    }
    
    IAudioClient_Stop(audioClient);
}

static void* RunAudio(void* arg)
{
    (void)arg;

    CoInitialize(NULL);
    audio_init();

    while (audio_running)
{
    pthread_mutex_lock(&sound_mutex);

    while (queue_count == 0 && audio_running)
        pthread_cond_wait(&sound_cond, &sound_mutex);

    if (!audio_running)
    {
        pthread_mutex_unlock(&sound_mutex);
        break;
    }

    Sound s = sound_queue[queue_head];
    //printf("Freq: %f Dur: %f \n", sound_queue[queue_head].freq, sound_queue[queue_head].dur);
    queue_head = (queue_head + 1) % SOUND_QUEUE_SIZE;
    queue_count--;
    pthread_mutex_unlock(&sound_mutex);

    play_tone(s.freq, s.dur);
}


    audio_shutdown();
    CoUninitialize();
    return NULL;
}

void InitSound(void)
{
    pthread_create(&sound_thread, NULL, RunAudio, NULL);
}

void PlayBeep(TrackType type)
{
    pthread_mutex_lock(&sound_mutex);
    memset(&sound_queue, 0, sizeof(Sound));

    if (queue_count < SOUND_QUEUE_SIZE)
    {
        switch(type)
        {
            case PLAYER_GOAL:
            {
                for (int i = 0; i < LONG_TRACK_L && queue_count < SOUND_QUEUE_SIZE; ++i)
                {
                    sound_queue[queue_tail].freq = player_goal[0][i];
                    sound_queue[queue_tail].dur  = player_goal[1][i];
                    queue_tail = (queue_tail + 1) % SOUND_QUEUE_SIZE;
                    queue_count++;
                }
            } break;
            case ENEMY_GOAL:
            {
                for (int i = 0; i < LONG_TRACK_L && queue_count < SOUND_QUEUE_SIZE; ++i)
                {
                    sound_queue[queue_tail].freq = enemy_goal[0][i];
                    sound_queue[queue_tail].dur  = enemy_goal[1][i];
                    queue_tail = (queue_tail + 1) % SOUND_QUEUE_SIZE;
                    queue_count++;
                }
            } break;
            case BEEP:
            {
                sound_queue[queue_tail].freq = contact[0][0];
                sound_queue[queue_tail].dur  = contact[1][0];
                queue_tail = (queue_tail + 1) % SOUND_QUEUE_SIZE;
                queue_count++;
            } break;
        }

        pthread_cond_signal(&sound_cond);
    }
    pthread_mutex_unlock(&sound_mutex);
}



void TerminateSound(void)
{
    pthread_mutex_lock(&sound_mutex);
    audio_running = false;
    pthread_cond_signal(&sound_cond);
    pthread_mutex_unlock(&sound_mutex);

    pthread_join(sound_thread, NULL);
}
