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


/* Definition of random number generation routines */
#include <global.h>

/* Get seed number for random and start it up */
void randomize(double seed)
{
      int j1;
      for(j1=0; j1<=54; j1++)
      {
            oldrand[j1] = 0.0;
      }
      jrand=0;
      warmup_random (seed);
      return;
}

/* Get randomize off and running */
void warmup_random (double seed)
{
      int j1, ii;
      double new_random, prev_random;
      oldrand[54] = seed;
      new_random = 0.000000001;
      prev_random = seed;
      for(j1=1; j1<=54; j1++)
      {
            ii = (21*j1)%54;
            oldrand[ii] = new_random;
            new_random = prev_random-new_random;
            if(new_random<0.0)
            {
                  new_random += 1.0;
            }
            prev_random = oldrand[ii];
      }
      advance_random ();
      advance_random ();
      advance_random ();
      jrand = 0;
      return;
}

/* Create next batch of 55 random numbers */
void advance_random ()
{
      int j1;
      double new_random;
      for(j1=0; j1<24; j1++)
      {
            new_random = oldrand[j1]-oldrand[j1+31];
            if(new_random<0.0)
            {
                  new_random = new_random+1.0;
            }
            oldrand[j1] = new_random;
      }
      for(j1=24; j1<55; j1++)
      {
            new_random = oldrand[j1]-oldrand[j1-24];
            if(new_random<0.0)
            {
                  new_random = new_random+1.0;
            }
            oldrand[j1] = new_random;
      }
}

/* Fetch a single random number between 0.0 and 1.0 */
double randomperc()
{
      jrand++;
      if(jrand>=55)
      {
            jrand = 1;
            advance_random();
      }
      return((double)oldrand[jrand]);
}

/* Fetch a single random integer between low and high including the bounds */
int rnd (int low, int high)
{
    int res;
    if (low >= high)
    {
        res = low;
    }
    else
    {
		res = low + (randomperc()*(high-low+1));
        if (res > high)
        {
            res = high;
        }
    }
    return (res);
}

/* Fetch a single random real number between low and high including the bounds */
double rndreal (double low, double high)
{
    return (low + (high-low)*randomperc());
}

void init_seeds()
{
	//50 Random seeds to select from
	seed[1] = 0.842;
	seed[2] = 0.194;
	seed[3] = 0.016;
	seed[4] = 0.936;
	seed[5] = 0.904;
	seed[6] = 0.830;
	seed[7] = 0.439;
	seed[8] = 0.319;
	seed[9] = 0.635;
	seed[10] = 0.959;
	seed[11]= 0.652;
	seed[12]= 0.291;
	seed[13]= 0.668;
	seed[14]= 0.716;
	seed[15]= 0.601;
	seed[16]= 0.424;
	seed[17]= 0.808;
	seed[18]= 0.232;
	seed[19]= 0.275;
	seed[20]= 0.057;
	seed[21]= 0.612;
	seed[22]= 0.305;
	seed[23]= 0.095;
	seed[24]= 0.101;
	seed[25]= 0.251;
	seed[26]= 0.780;
	seed[27]= 0.198;
	seed[28]= 0.522;
	seed[29]= 0.282;
	seed[30]= 0.752;
	seed[31]= 0.376;
	seed[32]= 0.963;
	seed[33]= 0.416;
	seed[34]= 0.048;
	seed[35]= 0.318;
	seed[36]= 0.580;
	seed[37]= 0.963;
	seed[38]= 0.035;
	seed[39]= 0.646;
	seed[40]= 0.543;
	seed[41]= 0.901;
	seed[42]= 0.504;	
	seed[43]= 0.306;
	seed[44]= 0.960;
	seed[45]= 0.607;
	seed[46]= 0.657;
	seed[47]= 0.144;
	seed[48]= 0.021;
	seed[49]= 0.319;
	seed[50]= 0.263;
	seed[51]= 0.213;
}
