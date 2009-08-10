/*
 * BlueCube - just another tetris clone 
 * Copyright (C) 2009  Sebastian Falbesoner
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

#include "global.h"
#include "grafix.h"
#include "credits.h"
#include "font.h"
#include "particle.h"

void NewGame();
extern int bDone;
extern int gamestate;

/*=========================================================================
// Name: MainMenu_Loop()
// Desc: The main menu loop (nasty function...)
//=======================================================================*/
void MainMenu_Loop()
{
    SDL_Event event;

    int i;
    static int iAuswahl = 1;  /* Current selection in the menu */
    static int MoveValue = 0; /* X-Position of the text */
    static int bMoveDirection = 1;  /* 0=left, 1=right */
    const int yPositions[3] = {180, 250, 320};
    const int xPositions[3] = {251, 271, 291};
    const char *labels[3] = {"    .go!", "  .about", "    .quit"};
    const char *descriptions[3] = {
        "Let the fun begin!", "Credits and other stuff...", "Please don't leave me alone!" };

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            bDone = 1;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_UP:
                iAuswahl--;      
                if (iAuswahl < 1)
                    iAuswahl = 3;
                break;
            case SDLK_DOWN:
                iAuswahl++;
                if (iAuswahl > 3)
                    iAuswahl = 1;
                break;
            case SDLK_RETURN:
            case SDLK_SPACE:
                switch (iAuswahl) {
                case 1:
                    NewGame();
                    break;
                case 2:
                    gamestate = STATE_CREDITS;
                    ShowCredits();
                    gamestate = STATE_MENU;
                    break;
                case 3:
                    gamestate = STATE_EXIT;
                    break;
                }
                break;
            default: break;
            }
            break;
        }
    }

    /* Move activated text */
    if (bMoveDirection) {
        MoveValue+=2;
        if (MoveValue == 6)
            bMoveDirection = 0;
    }
    else {
        MoveValue-=2;
        if (MoveValue == -6)
            bMoveDirection = 1;
    }

    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0,0,0));
    PutRect(220, 150, 210, 230, 16,16,16);
    MoveStars();   /* Move stars */
    DrawStars();   /* Draw stars */
    WriteTextCenter(font, 80, HEADLINE);

    /* Draw "text marker" */
    PutRect(xPositions[iAuswahl-1]-10, yPositions[iAuswahl-1]+MoveValue, 140, 32, 
        0,(iAuswahl-1)*32,(iAuswahl+1)*50);
    
    for (i=0; i<3; i++)
        if (i == iAuswahl-1)
            WriteText(font, xPositions[i]+MoveValue, yPositions[i]+(MoveValue/2), labels[i]);
        else
            WriteText(font, xPositions[i], yPositions[i], labels[i]);
        
    PutRect(0,0,640,40, 32,32,32);
    PutRect(0,40,640,3,128,128,128);
    WriteTextCenter(font, 3, "just another tetris clone");
    
    PutRect(0,437,640,3, 128,128,128);
    PutRect(0,440,640,40, 32,32,32);
    WriteTextCenter(font, 445, descriptions[iAuswahl-1]);

    SDL_Flip(screen);
}
