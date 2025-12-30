#ifndef AUDIO_H
#define AUDIO_H

#include <stdbool.h>
#include <pthread.h>
#include "sound_tracks.h"

#define SOUND_QUEUE_SIZE 5

typedef struct {
    float freq;
    float dur;
} Sound;

void InitSound(void);
void PlayBeep(TrackType type);
void TerminateSound(void);

#endif
