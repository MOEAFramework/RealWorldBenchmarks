/* LakeProblem_5Obj_1Const_Stoch.cpp
   
  Riddhi Singh, May, 2014
  The Pennsylvania State University
  rus197@psu.edu

  Adapted by Tori Ward, July 2014 
  Cornell University
  vlw27@cornell.edu

  Adapted by Jonathan Herman and David Hadka, Sept-Dec 2014
  Cornell University and The Pennsylvania State University

  A multi-objective represention of the lake model from Carpenter et al., 1999
  This simulation is designed for optimization with either Borg or the MOEAFramework.

  Stochasticity is introduced by natural phosphorous inflows. 
  These follow a lognormal distribution with specified mu and sigma.

  Decision Vector 
    vars : anthropogenic pollution flow at previous time step - Size 100, Bounds (0.0,0.1)

  Objectives
  1. minimize the maximum Phosphorous averaged over all lognormal draws in a given time period
  2. maximize expected benefit from pollution
  3. maximize the probability of meeting an inertia constraint
  4. maximize Reliability

*/

#include <stdio.h>
#include <unistd.h>
#include <sstream>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/math/tools/roots.hpp>
#include "moeaframework.h"
#include "boostutil.h"
#include "generate-scenarios.h"

#define nDays 100
#define nSamples 100
#define alpha 0.4 // utility from pollution
#define beta 0.08 // eutrophic cost
#define reliability_threshold 0.85
#define inertia_threshold (-0.02)

double b, q, mu, sigma, delta, pcrit;

int nvars   = nDays;
int nobjs   = 4;
int nconsts = 1;

namespace ublas = boost::numeric::ublas;
namespace tools = boost::math::tools;
using namespace std;

ublas::vector<double> average_daily_P(nDays);
ublas::vector<double> discounted_benefit(nSamples);
ublas::vector<double> days_inertia_met(nSamples);
ublas::vector<double> days_pcrit_met(nSamples);

void lake_problem(double* vars, double* objs, double* consts) 
{
  zero(average_daily_P);
  zero(discounted_benefit);
  zero(days_inertia_met);
  zero(days_pcrit_met);

  for (int s = 0; s < nSamples; s++)
  {   
    // randomly generated natural phosphorous inflows
    ublas::vector<double> P_inflow = generateSOW(nDays, mu, sigma);

    double X = 0.0; // lake state 

    //implement the lake model from Carpenter et al. 1999
    for (int i = 0; i < nDays; i++)
    {
      // new state: previous state - decay + recycling + pollution
      X = X*(1-b) + pow(X,q)/(1+pow(X,q)) + vars[i] + P_inflow(i);

      average_daily_P(i) += X/nSamples;

      discounted_benefit(s) += alpha*vars[i]*pow(delta,i);

      if(i > 0 && vars[i]-vars[i-1] > inertia_threshold)
        days_inertia_met(s) += 1;

      if(X < pcrit)
        days_pcrit_met(s) += 1;

    }
  }

  // Calculate minimization objectives (defined in comments at beginning of file)
  objs[0] = vmax(average_daily_P);
  objs[1] = -1*vsum(discounted_benefit)/nSamples;
  objs[2] = -1*vsum(days_inertia_met)/((nDays-1)*nSamples);
  objs[3] = -1*vsum(days_pcrit_met)/(nDays*nSamples);

  consts[0] = max(0.0, reliability_threshold - (-1*objs[3]));

  average_daily_P.clear();
  discounted_benefit.clear();
  days_inertia_met.clear();
  days_pcrit_met.clear();
}

double root_function(double x) {
  return pow(x,q)/(1+pow(x,q)) - b*x;
}

bool root_termination(double min, double max) {
  return abs(max - min) <= 0.000001;
}

int main(int argc, char* argv[]) 
{  
  double vars[nvars];
  double objs[nobjs];
  double consts[nconsts];

  // initialize defaults
  b = 0.42;
  q = 2;
  mu = 0.02;
  sigma = 0.0017783;
  delta = 0.98;
  pcrit = -1;

  //read the command line arguments
  int opt;

  while ((opt = getopt(argc, argv, "b:q:m:s:d:p:")) != -1) {
    switch (opt) {
      case 'b':
        b = atof(optarg);
        break;
      case 'q':
        q = atof(optarg);
        break;
      case 'm':
        mu = atof(optarg);
        break;
      case 's':
        sigma = atof(optarg);
        break;
      case 'd':
        delta = atof(optarg);
        break;
      case 'p':
        pcrit = atof(optarg);
        break;
      case '?':
      default:
        fprintf(stderr, "Unrecognized option\n");
        exit(EXIT_FAILURE);
    }
  }

  // compute the critical threshold if not supplied on command line
  if (pcrit < 0) {
    std::pair<double, double> result = tools::bisect(root_function, 0.01, 1.0, root_termination);
    pcrit = (result.first + result.second) / 2;
  }

  MOEA_Init(nobjs, nconsts);

  while (MOEA_Next_solution() == MOEA_SUCCESS) {
    MOEA_Read_doubles(nvars, vars);
    lake_problem(vars, objs, consts);
    MOEA_Write(objs, consts);
  }

  MOEA_Terminate();
  return EXIT_SUCCESS;
}
