/*
Copyright (C) 2008-2013 Joseph Kasprzyk, Patrick Reed, Brian Kirsch, Greg Characklis and others.

LRGV is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

LRGV is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the LRGV.  If not, see <http://www.gnu.org/licenses/>.
*/


/* Declaration for random number related variables and routines */
// Primary Author: Dave Goldberg (from SGA)

/* Variable declarations for the random number generator */
extern double seed[52];
extern double oldrand[55];
extern int jrand;

/* Function declarations for the random number generator */
void randomize(double seed);
void warmup_random (double seed);
void advance_random ();
double randomperc();
int rnd (int low, int high);
double rndreal (double low, double high);
void init_seeds();

