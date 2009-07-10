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

/* File: particle.h === Interface for the particle system and the star background */

#ifndef __PARTICLE_H__
#define __PARTICLE_H__

/* Particle system function prototypes */
void DrawParticles();
void UpdateParticles();
void CreateParticleExplosion(int x, int y, int r, int g, int b, int energy, int density);
int  NoParticlesLeft();

/* Star background function prototypes */
void InitStars();
void MoveStars();
void DrawStars();

#endif
