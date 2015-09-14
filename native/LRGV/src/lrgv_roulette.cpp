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

#include <global.h>

using namespace std;

double roulette_draw(vector <double> dataset, int *cdf)
{
	int success_flag = 0;
	int random_draw = rnd(1, cdf[dataset.size()-1]);

	for (int j = 0; j < dataset.size()-1; j++)
	{
		if ( (random_draw >= cdf[j]) && (random_draw < cdf[j+1]) )
		{
			success_flag = 1;
			return dataset[j];
			break;
		}
	}
	if (random_draw == cdf[dataset.size()-1])
	{
		success_flag = 1;
		return dataset[dataset.size()-1];
	}

	if (success_flag == 0)
	{
		winexit("Interpolation failed to find a value in the CDF.");
	}
}

int init_roulette_calcs(string mode, vector <double> &input_data, int *(&output_cdf), int highclassix, int weight)
{
	int inputlength = input_data.size();
	//first sort data
	if (mode == "ascending")
	{
		sort(input_data.begin(), input_data.end());
	}
	else if (mode == "descending")
	{
		sort(input_data.begin(), input_data.end(), std::greater<double>());
	}
	else
	{
		winexit("Invalid mode passed in to init_roulette_calcs!");
	}


	//get ready to store cdf
	output_cdf = new int [inputlength];

	int k = 1;
	for (int i = 0; i < inputlength; i++)
	{
		output_cdf[i] =						k;
		if (i+1 < highclassix)
			k++;
		else
			k += weight;
	}
	return output_cdf[inputlength-1];
}

void init_roulette()
{
	int inputlength, highclassix;
	double gamma = 0.25; //hard-coded for now
	int weight;

	//in the following choose:
	//ascending: if you want to focus on the HIGH part of the distribution
	//descending: if you want to focus on the LOW part of the distribution

	//inf
	inputlength = hydro_sets[0].inf.size();
	weight = params.inf_weight;
	highclassix = inputlength - (int) floor( gamma*(inputlength) );

	for (int month_it = 0; month_it < 12; month_it++)
	{
		params.inf_cdf_max = 
			init_roulette_calcs("descending", hydro_sets[month_it].inf, hydro_sets[month_it].inf_cdf, highclassix, weight);
	}

	//los
	inputlength = hydro_sets[0].los.size();
	weight = params.los_weight;
	highclassix = inputlength - (int) floor( gamma*(inputlength) );

	for (int month_it = 0; month_it < 12; month_it++)
	{
		params.los_cdf_max =
			init_roulette_calcs("ascending", hydro_sets[month_it].los, hydro_sets[month_it].los_cdf, highclassix, weight);
	}

	//res_var
	inputlength = hydro_sets[0].res_var.size();
	weight = params.res_var_weight;
	highclassix = inputlength - (int) floor( gamma*(inputlength) );

	for (int month_it = 0; month_it < 12; month_it++)
	{
		params.res_var_cdf_max =
			init_roulette_calcs("descending", hydro_sets[month_it].res_var, hydro_sets[month_it].res_var_cdf, highclassix, weight);
	}

	//lease low/high
	weight = params.lease_weight;
	//inputlength and weight are month-specific since we have variable-length lists

	for (int month_it = 0; month_it < 12; month_it++)
	{
		//high
		inputlength = lease_sets[month_it].high_res.size();
		highclassix = inputlength - (int) floor( gamma*(inputlength) );
		params.lease_high_cdf_max =
			init_roulette_calcs("ascending", lease_sets[month_it].high_res, lease_sets[month_it].high_res_cdf, highclassix, weight);

		//low
		inputlength = lease_sets[month_it].low_res.size();
		highclassix = inputlength - (int) floor( gamma*(inputlength) );
		params.lease_low_cdf_max =
			init_roulette_calcs("ascending", lease_sets[month_it].low_res, lease_sets[month_it].low_res_cdf, highclassix, weight);
	}

	//demand
	//note: demand growth happens later
	//also note that inputlength and weight are insite the loop since we have variable-length lists
	weight = params.demand_weight; //fixed 8.5.2011
	for (int month_it = 0; month_it < 12; month_it++)
	{
		inputlength = demand_sets[month_it].demand.size();
		highclassix = inputlength - (int) floor( gamma*(inputlength) );
		params.demand_cdf_max =
			init_roulette_calcs("ascending", demand_sets[month_it].demand, demand_sets[month_it].demand_cdf, highclassix, weight);
	}
	
	return;
}

void finalize_roulette()
{
	for (int month_it = 0; month_it < 12; month_it++)
	{
		zap(hydro_sets[month_it].inf_cdf);
		zap(hydro_sets[month_it].los_cdf);
		zap(hydro_sets[month_it].res_var_cdf);
		zap(lease_sets[month_it].high_res_cdf);
		zap(lease_sets[month_it].low_res_cdf);
		zap(demand_sets[month_it].demand_cdf);
	}
}