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

/* File: credits.c === The Credits ;) */

#include <stdlib.h>
#include "grafix.h"
#include "particle.h"
#include "font.h"
#include "credits.h"

extern SDL_Surface *screen;

int times[MAX_TIMES] = {3000, 6000, 9000, 12000, 15000, 18000, 21000, 24000,
						27000, 30000};
						
int current;
int running;

char *scroller[MAX_TIMES][6] = {
	{
		"BlueCube",
		"",
		"a tiny project",
		"written in C and",
		"SDL to get into",
		"game progamming"
	},
	{
		"",
		"programming",
		"",
		"SEBASTIAN FALBESONER",
		"",
		""
	},
	{
		"",
		"okay",
		"",
		"nothing more",
		"to say i think",
		""
	},
	{
		"",
		"GREETINGS",
		"",
		"go to all the",
		"sick people",
		"in my class"
	},
	{
		"",
		"and of course",
		"to all",
		"the others",
		"who i know",
		""
	},
	{
		"",
		"",
		"=)",
		"=)",
		"",
		""
	},
	{
		"press escape!",
		"",
		"nothing",
		"to see here",
		"any more",
		""
	},
	{
		"",
		"",
		"",
		"",
		"",
		""
	},
	{
		"",
		"",
		"",
		"",
		"",
		""
	},
	{
		"",
		"press C",
		"during the",
		"game to",
		"activate",
		"\'crazy mode\' ;)"
	}
};

/* Function prototypes from other source files */
Uint32 TimeLeft();

/*=========================================================================
// Name: ShowCredits()
// Desc: Shows us the credits ;)
//=======================================================================*/
void ShowCredits()
{
	/*int explosion_created = 0;*/
	int i, j = 0;
	current = 0;
	running = 1;
	Uint8* keystate;

	int now = SDL_GetTicks();
	
	while (running)
	{
		if ((SDL_GetTicks() - now) >= times[current])
		{
			/*explosion_created = 0;*/
			j = 0;
			current++;
			if (current == MAX_TIMES)
			{
				current--; /* To not cause a segfault the last frame... */
				running = 0;
			}
		}

		SDL_PumpEvents();

		keystate = SDL_GetKeyState(NULL);
		if (keystate[SDLK_ESCAPE])
		{
			running = 0;
		}

		MoveStars();

		
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0,0,0));

		
		/*
			if (!explosion_created) {
				j++;
				CreateParticleExplosion(rand()%640, rand()%480, 
				rand()%256, rand()%256, rand()%256, 1, 50);
				if (j == 10)
					explosion_created = 1;
			}
		*/

		DrawStars();
		PutRect(160,120,320,240, 16,16,16);
		for (i=0; i<6; i++)
			WriteTextCenter(font, 120 + i*40, scroller[current][i]);

		
		
		SDL_Flip(screen);
		SDL_Delay(TimeLeft());
	}
}
