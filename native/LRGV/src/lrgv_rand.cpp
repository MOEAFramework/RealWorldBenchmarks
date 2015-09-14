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
// Primary Author: Dave Goldberg (from SGA)

//added c++ style include statements for
//randsample functions at the bottom...

#include <global.h>

using namespace std;

/////
/////
//Program-specific random sampling routines
//written by JRK
/////
/////

void randsample (const hydro_structure& set, sampled_data& sample, int NumberYears)
{
	/* Usage Notes:
	
	- hydro structure contains, NW, los, inf, res_var

	- this function takes one structure (not an array of structures)
	and samples for every data type within that structure. See function call
	from outside to see how months/years/sims are handled.
	
	-The randsample function is overloaded for each different type of
	historical data.
	
	-The random sampling is done as follows: rnd returns an integer value
	from low to high, where the usage is rnd(low,high). That integer is then used
	to index in to a set of data. sample is the actual sampled value used (the output)
	set is the set of historical values from which we sample (the input)
	
	-Roulette Weighted Sampling artifically "lengthens" the input dataset so that
	some samples become more likely than others. The rationale is the same, though.*/
	
	if (params.roulette_flag)
	{
		for (int year_it = 0; year_it < NumberYears; year_it ++)
		{
			sample.inf[year_it] = 		roulette_draw (set.inf, set.inf_cdf);
			sample.los[year_it] =		roulette_draw (set.los, set.los_cdf);
			sample.res_var[year_it] =	roulette_draw (set.res_var, set.los_cdf);
		}
	}
	else
	{
		for (int year_it = 0; year_it < NumberYears; year_it++)
		{
			int random_draw;
			random_draw = rnd(0,32);
			sample.inf[year_it] = set.inf[random_draw];

			random_draw = rnd(0,32);
			sample.los[year_it] = set.los[random_draw];
		
			//NW sampling is not needed, since NW is only used
			//to calculate means...
			//random_draw = rnd(0,32);
			//sample.NW[year_it] = set.NW[random_draw];

			random_draw = rnd(0,30);
			sample.res_var[year_it] = set.res_var[random_draw];
		}
	}
	return;
}

void randsample (const lease_structure& set, sampled_data& sample, int NumberYears)
{
	// lease structure contains lease prices for high and low reservoir levels
	if (params.roulette_flag)
	{
		for (int year_it = 0; year_it<NumberYears; year_it++)
		{
			sample.lease_high_res[year_it] =	roulette_draw (set.high_res, set.high_res_cdf);
			sample.lease_low_res[year_it] =		roulette_draw (set.low_res, set.low_res_cdf);
		}
	}
	else
	{
		for (int year_it = 0; year_it<NumberYears; year_it++)
		{
			int random_draw;
			random_draw = rnd(0, (int) set.high_res.size()-1);
			sample.lease_high_res[year_it] = set.high_res[random_draw];

			random_draw = rnd(0, (int) set.low_res.size()-1);
			sample.lease_low_res[year_it] = set.low_res[random_draw];
		}
	}

	return;

}

void randsample (const demand_structure& set, sampled_data& sample, int NumberYears, double DemGrowthFactor)
{
	//NOTE: Demand is automatically scaled to demand growth here in the random sample.
	if (params.roulette_flag)
	{
		for (int year_it = 0; year_it<NumberYears; year_it++)
		{
			sample.demand[year_it] = 
				roulette_draw (set.demand, set.demand_cdf)*pow((1+DemGrowthFactor), (double) year_it);
		}
	}
	else
	{
		for (int year_it = 0; year_it<NumberYears; year_it++)
		{
			int random_draw;
			random_draw = rnd(0, (int) set.demand.size()-1);
			sample.demand[year_it] = set.demand[random_draw]*pow((1+DemGrowthFactor), (double) year_it);
		}
	}
	return;

}