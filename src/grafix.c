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

/* File: grafix.c === gfx Implementation */

#include <stdio.h>
#include <stdlib.h>
#include "grafix.h"
#include "sound.h"

SDL_Surface *screen; /* Primary Surface */

/*=========================================================================
// Name: InitSDLex()
// Desc: Starts the graphics system (SDL)
//=======================================================================*/
void InitSDLex()
{
    int init_flags = SDL_INIT_VIDEO;
    if (bSoundActivated)
        init_flags |= SDL_INIT_AUDIO;

    if (SDL_Init(init_flags) < 0) {
        fprintf(stderr, "Couldn't init SDL -> %s\n", SDL_GetError());
        exit(1);
    }
    atexit(SDL_Quit);
    if (bSoundActivated)
        atexit(SDL_CloseAudio); 

    /* Set video mode */
    screen = SDL_SetVideoMode(SCREEN_X, SCREEN_Y, SCREEN_BPP, 
        SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
    if (screen == NULL) {
        fprintf(stderr, "Couldn't set video mode -> %s\n", SDL_GetError());
        exit(1);
    }

    SDL_WM_SetCaption("BlueCube", "BlueCube");
    SDL_ShowCursor(SDL_DISABLE);
}

/*=========================================================================
// Name: PutPixel()
// Desc: Simplay plots a pixel onto the screen (but he must be locked!)
//=======================================================================*/
void PutPixel(int x, int y, int r, int g, int b)
{
    Uint8 *p = (Uint8*)screen->pixels + y * screen->pitch + x*2;
    
    *(Uint16 *)p = SDL_MapRGB(screen->format, r,g,b);
}

/*=========================================================================
// Name: PutRect()
// Desc: Draws a filled rectangle onto the screen
//=======================================================================*/
void PutRect(int x, int y, int w, int h, int r, int g, int b)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, r, g, b));
}


/*=========================================================================
// Name: BlitIMG()
// Desc: Blits a part of another surface onto the screen
//=======================================================================*/
void BlitIMG(SDL_Surface *img, int x, int y, int x2, int y2, int w, int h)
{
    SDL_Rect dst, src;

    dst.x = x;
    dst.y = y;
    src.x = x2;
    src.y = y2;
    src.w = w;
    src.h = h;

    SDL_BlitSurface(img, &src, screen, &dst);
}
