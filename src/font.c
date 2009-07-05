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

/* File: font.c === Loading and blitting bitmap fonts */

#include <string.h>
#include <stdlib.h>
#include "grafix.h"
#include "font.h"

SDLFont *font; /* Font used for everything */

static int GetCenterX(SDLFont *font, const char *text)
{
	int i;
	int textWidth = 0;
	int textLength = strlen(text);

	for (i=0; i < textLength; i++)
		textWidth += font->widths[(int)text[i]];

	return (SCREEN_X/2 - textWidth/2);
}

/*=========================================================================
// Name: LoadFontfile()
// Desc: Tries to load a bitmap font (the width-file is also needed) and 
//       returns a pointer to (hopefully) the new created SDLFont
//=======================================================================*/
SDLFont *LoadFontfile(const char *fontfile, const char *widthfile)
{
	int i;
	FILE *fp;
	SDLFont     *tempFont;    /* The font structure */
	SDL_Surface *tempSurface; /* Temporary font surface */

	
	/* Create new font structure (allocate memory) */
	tempFont = (SDLFont*)malloc(sizeof(SDLFont));

	/* Load font bitmap into the temporary surface, convert it to display format 
	   and assign it to the font structure (then free the temporary surface) */
	tempSurface = SDL_LoadBMP(fontfile);
	if (tempSurface == NULL)
	{
		fprintf(stderr, "Couldn't load font file (%s)\n", fontfile);
		exit(1);
	}
	SDL_SetColorKey(tempSurface, SDL_SRCCOLORKEY, SDL_MapRGB(tempSurface->format, 0,0,0));
	tempFont->fontBitmap = SDL_DisplayFormat(tempSurface);
	SDL_FreeSurface(tempSurface);

	/* Allocate array for the char widths and load width-file */
	tempFont->widths = (int*)malloc(256*sizeof(int));
	fp = fopen(widthfile, "rb");
	if (fp != NULL)
	{
		for (i=0; i<256; i++)
			tempFont->widths[i] = fgetc(fp);
	}
	else
	{
		fprintf(stderr, "Couldn't load font widths file (%s)\n", widthfile);
		exit(1);
	}

	tempFont->charWidth = tempFont->fontBitmap->w / 16;

	/* Return new created font structure */
	return tempFont;
}

/*=========================================================================
// Name: FreeFont()
// Desc: Deletes a font correctly from memory
//=======================================================================*/
void FreeFont(SDLFont *font)
{
	free(font->widths);
	SDL_FreeSurface(font->fontBitmap);
	free(font);
}

/*=========================================================================
// Name: WriteText()
// Desc: Draws text onto the screen ;)
//=======================================================================*/
void WriteText(SDLFont *font, int x, int y, const char *text)
{
	int i;
	int nextX=0;
	int text_length = strlen(text);
	int ascii;
	
	for (i=0; i<text_length; i++)
	{
		ascii = (int)text[i];
		
		BlitIMG(
				font->fontBitmap, x+nextX, y,
				(ascii%16 * font->charWidth) +
				((font->charWidth/2) - (font->widths[ascii])/2),
				((ascii/16)-2) * font->charWidth,
				font->widths[ascii],
				font->charWidth);

		nextX += font->widths[ascii];
	}
}

/*=========================================================================
// Name: WriteTextCenter()
// Desc: Same as WriteText(), but the text will be centered, so you
//       don't have to give the x coordinate as argument
//=======================================================================*/
void WriteTextCenter(SDLFont *font, int y, const char *text)
{
	int x;

	/* Calculate X-Position */
	x = GetCenterX(font, text);

	/* Draw text with the calculated position */
	WriteText(font, x, y, text);
}
