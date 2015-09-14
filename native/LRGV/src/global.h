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


#ifndef _global_h
#define _global_h

#define LRGV

//This is a stripped-down version of the header file which is used for our
//testing program.

#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <float.h>
#include <ctype.h>
#include <time.h>
#include <iostream>
#include <string>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <assert.h>

//new to this version:
#include <unistd.h>
#include "moeaframework.h"

using namespace std;

struct Individual {
  double  *xreal;
  double  *obj;
  double  *constr;
};

#define START_TIME clock()
#define GET_TIME(start) ((clock()-start)/(double)CLOCKS_PER_SEC)

//void param_read(char *input_filename, char *output_filename, double *xreal, int model_case);
//void lrgv_sampler(string input_filename, string ranges_filename, int num_sol, char *function_calcparam);
//void usage(int argc, char **argv);

#include "lrgv.h"
#include "lrgv_rand.h"
#include "Simulation.h"
extern const vector<string> obj_avail;
extern const vector<string> constr_avail;
//void calc_LRGV(Simulation &sim, Individual *ind, char *calc_param);
#endif
