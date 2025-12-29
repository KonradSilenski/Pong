#ifndef AUDIO_H
#define AUDIO_H

#include <stdbool.h>
#include <pthread.h>
#include "sound_tracks.h"

/* Sound request shared with audio thread */
#define SOUND_QUEUE_SIZE 32

typedef struct {
    float freq;
    float dur;
} Sound;


/* Public API */
void InitSound(void);
void PlayBeep(TrackType type);
void TerminateSound(void);

#endif
