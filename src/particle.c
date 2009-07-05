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

/* File: particle.c === Particle system and star background */

#include "global.h"
#include "particle.h"
#include "grafix.h"
#include <math.h>
#include <stdlib.h>

extern int gamestate; /* Evil coding style... */

Particle particles[MAX_PARTICLES];
Star stars[MAX_STARS];

int active_particles = 0;

/* Some static functions to add and delete particles */
static void AddParticle(pParticle particle)
{
    if (active_particles >= MAX_PARTICLES)
        return;

    particles[active_particles] = *particle;
    active_particles++;
}

static void DeleteParticle(int index)
{
    particles[index] = particles[active_particles - 1];
    active_particles--;
}

/*=========================================================================
// Name: DrawParticles()
// Desc: Draws all active particles onto the screen
//=======================================================================*/
void DrawParticles()
{
    int i;

    for (i=0; i<active_particles; i++)
    {
        int x, y;

        x = (int)particles[i].x;
        y = (int)particles[i].y;

        PutRect( x, y, 2, 2,
            particles[i].r, particles[i].g, particles[i].b);
    }
}

/*=========================================================================
// Name: UpdateParticles()
// Desc: Moves all the particles and deletes those who are "dead"
//=======================================================================*/
void UpdateParticles()
{
    int i;

    for (i=0; i<active_particles; i++)
    {
        particles[i].x += particles[i].energy * cos(particles[i].angle * PI / 180.0);
        particles[i].y += particles[i].dy;
        /*
           for STATE_CREDITS should be used:
           particles[i].y += particles[i].energy * -sin(particles[i].angle * PI / 180.0);
        */
        particles[i].r-=2;
        particles[i].g-=2;
        particles[i].b-=2;

        if (particles[i].r < 0)
            particles[i].r = 0;
        if (particles[i].g < 0)
            particles[i].g = 0;
        if (particles[i].b < 0)
            particles[i].b = 0;

        if ((particles[i].r + particles[i].g + particles[i].b) == 0) {
            DeleteParticle(i);
            i--;
        }
    }
}

/*=========================================================================
// Name: CreateParticleExplosion()
// Desc: This function will be called to start an explosion ;)
//=======================================================================*/
void CreateParticleExplosion(int x, int y, int r, int g, int b, 
                             int energy, int density)
{
    int i;
    Particle particle; /* New particle */

    for (i=0; i<density; i++)
    {
        particle.x = x;
        particle.y = y;
        particle.dy = ((rand()%10)+1)/10.0;
        particle.angle = rand()%360;
        particle.energy = (double)(rand()%(energy * 1000)) / 1000.0;

        particle.r = r;
        particle.g = g;
        particle.b = b;

        AddParticle(&particle);
    }
}

/*=========================================================================
// Name: NoParticlesLeft()
// Desc: Returns a true value if there are no particles left
//=======================================================================*/
int NoParticlesLeft()
{
    if (active_particles == 0)
        return 1;
    else
        return 0;
}

/*=========================================================================
// Name: InitStars()
// Desc: Inits the star array with random values
//=======================================================================*/
void InitStars()
{
    int i;

    for (i=0; i<MAX_STARS; i++)
    {
        stars[i].pX = (float)(rand() % SCREEN_X);
        stars[i].pY = rand() % SCREEN_Y;
        stars[i].vX = (float)(((rand()%100)/10)+1);
        stars[i].clr = 255 - (100-(int)(stars[i].vX*10));
    }
}

/*=========================================================================
// Name: MoveStars()
// Desc: Moves all the stars
//=======================================================================*/
void MoveStars()
{
    int i;

    for (i=0; i<MAX_STARS; i++)
    {
        stars[i].pX += stars[i].vX;
        if (stars[i].pX >= SCREEN_X)
            stars[i].pX -= (float)SCREEN_X;
        
        if (gamestate == STATE_CREDITS)
        {
            stars[i].pY -= stars[i].vX;
            if (stars[i].pY < 0)
                stars[i].pY += SCREEN_Y;
        }
    }

}

/*=========================================================================
// Name: DrawStars()
// Desc: Draws all the stars onto the screen
//=======================================================================*/
void DrawStars()
{
    int i;

    SDL_LockSurface(screen);
    for (i=0; i<MAX_STARS; i++)
        PutPixel((int)stars[i].pX, stars[i].pY, stars[i].clr, stars[i].clr, 255);
    SDL_UnlockSurface(screen);
}
