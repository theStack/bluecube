/*
 * BlueCube - just another tetris clone 
 * Copyright (C) 2003  Sebastian Falbesoner
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

/* File: sound.c === Loading and playing sounds (wave files) */

#include "SDL.h"
#include "sound.h"
#include <stdlib.h>
#include <string.h>

/* Structure for the sound actually plaing */
typedef struct
{
    int active;      /* Is the sound active? (Should be played?) */
    pSound sound;    /* Pointer to sound data */
    Uint32 position; /* Stream position */
} SoundPlaying, *pSoundPlaying;

SoundPlaying sounds[MAX_PLAYING_SOUNDS]; /* Array with all the sounds */

Sound sndLine;       /* Will be played when a line is cleared */
Sound sndNextlevel;  /* Will be played when the next level is reached */

int bSoundActivated; /* Sound system started? */

static void AudioCallback(void *user_data, Uint8 *audio, int length);

/*=========================================================================
// Name: InitSound()
// Desc: Initializes the sound system (must be called after SDL_Init())
//=======================================================================*/
void InitSound()
{
    SDL_AudioSpec wanted;
    
    /* Open audio device */
    wanted.freq     = 11025;
    wanted.format   = AUDIO_U8;
    wanted.channels = 1;
    wanted.samples  = 1024;
    wanted.callback = AudioCallback;
    wanted.userdata = NULL;

    if (SDL_OpenAudio(&wanted, NULL) < 0) {
        fprintf(stderr, "Couldn't open audio -> %s\n", SDL_GetError());
        exit(1);
    }
}

/*=========================================================================
// Name: LoadSound()
// Desc: Loads a wave file into a Sound structure
//=======================================================================*/
void LoadSound(char *soundfile, pSound sound)
{
    SDL_AudioSpec wav_spec;

    if (!SDL_LoadWAV(soundfile, &wav_spec, &sound->samples, &sound->length)) {
        fprintf(stderr, 
            "Couldn't load wav file [%s] -> %s\n", soundfile, SDL_GetError());
        exit(1);
    }
}

/*=========================================================================
// Name: ClearPlayingSounds()
// Desc: Clears every playing sound
//=======================================================================*/
void ClearPlayingSounds()
{
    int i;

    for (i=0; i<MAX_PLAYING_SOUNDS; i++)
        sounds[i].active = 0;
}

/*=========================================================================
// Name: PutSound()
// Desc: Plays the given sound ;)
//=======================================================================*/
void PutSound(pSound sound)
{
    int i;

    if (!bSoundActivated)
        return;

    /* Find first free slot in array */
    for (i=0; i<MAX_PLAYING_SOUNDS; i++) {
        if (sounds[i].active == 0)
            break;
    }

    if (i == MAX_PLAYING_SOUNDS)
        return;

    SDL_LockAudio();
    sounds[i].active   = 1;
    sounds[i].sound    = sound;
    sounds[i].position = 0;
    SDL_UnlockAudio();
}

/*=========================================================================
// Name: AudioCallback()
// Desc: Audio callback function (very important!)
//=======================================================================*/
static void AudioCallback(void *user_data, Uint8 *audio, int length)
{
    int i;

    /* Clear audiobuffer */
    memset(audio, 0, length);   
        
    /* Mix all sounds in the array together! */
    for (i=0; i<MAX_PLAYING_SOUNDS; i++) {
        if (sounds[i].active) { /* Only if sound is active... */
            Uint8 *sound_buf;
            Uint32 sound_len;

            sound_buf = sounds[i].sound->samples;
            sound_buf += sounds[i].position;

            if ((sounds[i].position + length) > sounds[i].sound->length)
                sound_len = sounds[i].sound->length - sounds[i].position;
            else
                sound_len = length;

            /* Mix sound into stream */
            SDL_MixAudio(audio, sound_buf, sound_len, VOLUME_PER_SOUND);

            /* Update sound buffer position */
            sounds[i].position += length;

            /* Sound has reached end? */
            if (sounds[i].position >= sounds[i].sound->length)
                sounds[i].active = 0;
        }
    }
}
