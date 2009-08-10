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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "global.h"
#include "grafix.h"
#include "sound.h"
#include "particle.h"
#include "font.h"
#include "box.h"
#include "mainmenu.h"

/*#define DEBUG_CHANGE_LEVEL_WITH_KEYS*/

static void Mainloop();
Uint32 TimeLeft();
static void Game_Loop();
static void DrawScene();
void StartGameOverAnimation();
static void GameOverAnimation();

SDL_Event event;

int lines, score;       /* Line counter and score */
int nextPiece;          /* Next cluster  */
int level;              /* Current level */

int gamestate;    /* Current state */
int bDone=0;     /* Want to Exit? */
int bPause;      /* Pause? */
int bCrazy;      /* Yahooooooooooo ;) CRAZY MODE ^.^ */
int bGameOver;   /* GameOver Animation? */
int bExplode;    /* Explosion is active? */
int x,y;         /* Current explosion coordinates */

/*=========================================================================
// Name: Mainloop()
// Desc: the one and only mainloop ;-)
//=======================================================================*/
void Mainloop()
{
    while (!bDone) { /* Mainloop */
        switch (gamestate) {
        case STATE_MENU:
            MainMenu_Loop();
            SDL_Delay(TimeLeft());
            break;

        case STATE_PLAY:
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                case SDL_QUIT: bDone = 1; break;
                case SDL_KEYDOWN:
                    /* Escape can be pressed everytime to get back to the menu */
                    if (event.key.keysym.sym == SDLK_ESCAPE) 
                        gamestate = STATE_MENU;

                    if (!bPause && !bGameOver) /* Is the game active? */
                    switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        MoveCluster(1); /* "drop" cluster...      */
                        NewCluster();   /* ... and create new one */
                        break;
                    case SDLK_DOWN:  if (MoveCluster(0)) NewCluster(); break;
                    case SDLK_LEFT:  MoveClusterLeft();  break;
                    case SDLK_RIGHT: MoveClusterRight(); break;
                    case SDLK_UP:    TurnClusterRight(); break;
                    case SDLK_p:     bPause = 1; break; /* Activate pause mode */
                    
                    case SDLK_c: /* Crazy mode... */
                        if (!bCrazy)
                            bCrazy = 1;
                        else {
                            BoxDrawInit();
                            bCrazy = 0;
                        }
                        break;
                        
#ifdef DEBUG_CHANGE_LEVEL_WITH_KEYS
                    case SDLK_w:
                        if (level < 10) {
                            level++;
                            PutSound(&sndNextlevel);
                        }
                        break;
                    case SDLK_q:
                        if (level > 0)
                            level--;
                        break;
#endif
                    default: break;
                    }
                    else if (event.key.keysym.sym == SDLK_p) /* deactivate pause mode again */
                        bPause = 0;

                    break;
                }
            }
            Game_Loop();

            SDL_Delay(TimeLeft());
            break;

        case STATE_GAMEOVER:
            break;

        case STATE_CREDITS:
            /* Credits_Loop(false); */
            SDL_Delay(TimeLeft());
            break;

        case STATE_EXIT:
            bDone = 1;
            break;
        }
    }
}

/*=========================================================================
// Name: TimeLeft()
// Desc: Returns the number of ms to wait that the framerate is constant
//=======================================================================*/
Uint32 TimeLeft()
{
    static Uint32 next_time=0;
    Uint32 now;

    now = SDL_GetTicks();
    if (next_time <= now) {
        next_time = now + TICK_INTERVAL;
        return 0;
    }

    return (next_time - now);
}


/*=========================================================================
// Name: NewGame()
// Desc: Starts a new game
//=======================================================================*/
void NewGame()
{
    InitBox(); /* Clear Box */
    BoxDrawInit(); /* Init boxdraw values */
    DestroyParticles(); /* Destroy any particles that might be left */
    
    lines = 0; /* Reset lines counter */
    score = 0; /* Reset score */
    level = 0; /* Reset level */
    nextPiece = rand()%7; /* Create random nextPiece */
    NewCluster();

    bPause = 0; /* No pause */
    bCrazy = 0; /* No crazymode :) */
    bGameOver = 0;

    gamestate = STATE_PLAY; /* Set playstate */
}

/*=========================================================================
// Name: Game_Loop()
// Desc: The main loop for the game
//=======================================================================*/
static void Game_Loop()
{
    if (!bPause) {
        if (!bGameOver) {
            cluster.dropCount--;  /* Decrease time until the next fall */
            if (cluster.dropCount == 0) {
                if (MoveCluster(0)) /* If cluster "collides"... */
                    NewCluster();   /* then create a new one ;) */
            }

            /* Increase Level */
            if (((level == 0) && (lines >=  10)) ||
                ((level == 1) && (lines >=  20)) ||
                ((level == 2) && (lines >=  40)) ||
                ((level == 3) && (lines >=  80)) ||
                ((level == 4) && (lines >= 100)) ||
                ((level == 5) && (lines >= 120)) ||
                ((level == 6) && (lines >= 140)) ||
                ((level == 7) && (lines >= 160)) ||
                ((level == 8) && (lines >= 180)) ||
                ((level == 9) && (lines >= 200))) {
                level++;
                PutSound(&sndNextlevel);
            }
        }
        else {
            GameOverAnimation();
        }

        if (bCrazy)        /* If crazy mode is actived... */
            BoxDrawMove(); /* ... change box settings!    */
        MoveStars();       /* Move stars */
        UpdateParticles(); /* Move particles */
    }

    DrawScene();
}

/*=========================================================================
// Name: DrawScene()
// Desc: Draws the whole scene!
//=======================================================================*/
static void DrawScene()
{
    char chScore[30],
         chLines[30],
         chLevel[30];

    /* Black background */
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0,0,0));
    DrawStars(); /* Starfield */

    if (bCrazy)
        WriteText(font, 250, 30+(boxdraw.box_x), "Cr4zY m0d3!"); /* woo hoo! */

    if (!bGameOver) {
        /* Draw border of the box */
        PutRect(boxdraw.box_x-5, boxdraw.box_y-5,
                boxdraw.box_width + 2*5, boxdraw.box_height + 2*5, 150,150,150);
        PutRect(boxdraw.box_x, boxdraw.box_y,
                boxdraw.box_width, boxdraw.box_height, 90,90,90);

        DrawCluster(); /* Draw cluster */
        sprintf(chScore, "%d", score);
        sprintf(chLines, "%d", lines);
        sprintf(chLevel, "%d", level);
        WriteText(font, 490, 80, "Score");  /*  Show SCORE */
        WriteText(font, 490, 105, chScore); 
        WriteText(font, 495, 180, "Next");  /*  Show NEXT piece */
        DrawNextPiece(490, 220);
        WriteText(font, 490, 350, "Lines"); /*  Show number of killed LINES */
        WriteText(font, 490, 375, chLines);
        WriteText(font, 95, 350, "Level");  /*  Show current LEVEL */
        WriteText(font, 95, 375, chLevel);
    }

    DrawBox();       /* Draw box bricks */
    DrawParticles();

    if (bPause)
        WriteText(font, 265, 20, "- PAUSE -");
    if (bGameOver && !bExplode) {
        WriteTextCenter(font, 120, "GAME OVER");
        sprintf(chScore, "Your Score: %d", score); 
        WriteTextCenter(font, 250, chScore);
    }

    SDL_Flip(screen); /* Let's flip! */
}

/*=========================================================================
// Name: StartGameOverAnimation()
// Desc: Starts the gameover animation
//=======================================================================*/
void StartGameOverAnimation()
{
    x = 0;
    y = 0;
    bGameOver = 1;
    bExplode = 1;
}

/*=========================================================================
// Name: GameOverAnimation()
// Desc: Gameover Animation!
//=======================================================================*/
static void GameOverAnimation()
{
    static int counter = 1; /* Sound counter */

    if (bExplode) {
        while (!IsBrickSet(x,y)) { /* Search for the next brick */
            x++;
            if (x == BOX_BRICKS_X) {
                x = 0;
                y++;
                if (y == BOX_BRICKS_Y) { /* End reached? */ 
                    bExplode = 0;
                    return;
                }
            }
        } /* Booooom! ;) */

        BrickExplosion(x, y, 2, 20);
        SetBrick(x, y, 0);
        counter--;
        if (counter == 0) { /* Sound will be played every six bricks */
            PutSound(&sndLine);
            counter = 6;
        }
    }
    else if (NoParticlesLeft())
        gamestate = STATE_MENU;
}

/*=========================================================================
// Name: main()
// Desc: Entrypoint
//=======================================================================*/
int main(int argc, char *argv[])
{
    printf("-----------------------------------------------------\n");
    printf(" BlueCube (improved version v0.9 hosted on github)   \n");
    printf("    just another tetris clone,                       \n");
    printf("    written by Sebastian Falbesoner <theStack>       \n");
    printf("-----------------------------------------------------\n");
    
    /* First, check whether sound should be activated */
    bSoundActivated = 1;
    if ((argc == 2) && (strcmp(argv[1], "--nosound") == 0)) {
        printf("No sound is used!\n");
        bSoundActivated = 0;
    }

    /* Init randomizer */
    printf("Initializing randomizer... ");
    srand(time(NULL));
    printf("done.\n");
    
    /* Let's init the graphics and sound system */
    printf("Starting up SDL... ");
    InitSDLex();
    printf("done.\n");
    if (bSoundActivated) {
        printf("Initializing sound subsystem... ");
        InitSound();
        printf("done.\n");
    }
    
    /* Load font */
    printf("Loading font files... ");
    font = LoadFontfile("font/font.dat", "font/widths.dat");
    printf("done.\n");

    /* Load soundfiles */
    if (bSoundActivated) {
        printf("Loading sound files... ");
        LoadSound("sound/killline.snd", &sndLine);
        LoadSound("sound/nextlev.snd",  &sndNextlevel);
        printf("done.\n");
    }
    printf("=== Let the fun begin! ===\n");

    /* Init star background */
    InitStars();

    if (bSoundActivated) {
        ClearPlayingSounds();
        SDL_PauseAudio(0);
    }

    /* Start menu first, please... */
    gamestate = STATE_MENU;

    /* Call main loop */
    Mainloop();

    /* Free sounds again */
    if (bSoundActivated) {
        printf("Releasing memory for sound samples... ");
        SDL_FreeWAV(sndLine.samples); 
        SDL_FreeWAV(sndNextlevel.samples);
        printf("done.\n");
    }

    /* Free font again */
    printf("Releasing memory for fonts... ");
    FreeFont(font);
    printf("done.\n");

    return 0;
}
