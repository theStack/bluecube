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

/* File: sound.h === Interface for loading and playing sounds */

#ifndef __SOUND_H__
#define __SOUND_H__

/* Maximum number of sounds playing at the same time */
#define MAX_PLAYING_SOUNDS    10

/* Sound volume */
#define VOLUME_PER_SOUND      (SDL_MIX_MAXVOLUME / 2)

/* Structure for loaded sound files */
typedef struct
{
	Uint8 *samples; /* Raw PCM data */
	Uint32 length;  /* Soundsize (in bytes) */
} Sound, *pSound;


/* Structure for the sound actually plaing */
typedef struct
{
	int active;      /* Is the sound active? (Should be played?) */
	pSound sound;    /* Pointer to sound data */
	Uint32 position; /* Stream position */
} SoundPlaying, *pSoundPlaying;

void InitSound(void);
void LoadSound(char *soundfile, pSound sound);
void ClearPlayingSounds(void);
void PutSound(pSound sound);


/* Make my sounds available in every file including this header */
extern Sound sndLine;
extern Sound sndNextlevel;

#endif
