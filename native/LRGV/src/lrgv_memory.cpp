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

void general_1d_allocate(double *(&data), int length)
{
	data = new double [length];

	for (int i = 0; i < length; i++) data[i] = 0.0;

	return;
}

void general_1d_allocate(char *(&data), int length)
{
	data = new char [length];

	return;
}

void general_1d_allocate(int *(&data), int length)
{
	data = new int [length];

	for (int i = 0; i < length; i++) data[i] = 0;

	return;
}

void general_2d_allocate(double **&data, int rows, int cols)
{
	data = new double *[rows];
	for (int row_it = 0; row_it<rows; row_it++) data[row_it] = new double[cols];

	for (int row_it = 0; row_it < rows; row_it++)
	{
		for (int col_it = 0; col_it < cols; col_it++)
		{
			data[row_it][col_it] = 0.0;
		}
	}
}

void general_2d_allocate(int **(&data), int rows, int cols)
{
	data = new int *[rows];
	for (int row_it = 0; row_it<rows; row_it++) data[row_it] = new int[cols];

	for (int row_it = 0; row_it < rows; row_it++)
	{
		for (int col_it = 0; col_it < cols; col_it++)
		{
			data[row_it][col_it] = 0;
		}
	}
}

void zap(double **(&data), int rows)
{
	if (data != NULL)
	{
		for (int row_it = 0; row_it < rows; row_it++)
		{
			delete [] data[row_it];
		}
		delete [] data;
		data = NULL;
	}
	return;
}

void zap(double *(&data))
{
	if (data != NULL)
	{
		delete [] data;
		data = NULL;
	}

	return;
}

void zap(char *(&data))
{
	delete data;
	return;
}

void zap(int *(&data))
{
	if (data != NULL)
	{
		delete [] data;
		data = NULL;
	}

	return;
}

void zap(int **(&data), int rows)
{
	if (data != NULL)
	{
		for (int row_it = 0; row_it < rows; row_it++)
		{
			delete [] data[row_it];
		}

		delete [] data;
		data = NULL;
	}
	return;
}


void global_trackers_allocation(int &initial_call)
{
	int NumberYears = params.NumberYears;
	int NumberSims = params.NumberSims;

	if (initial_call)
	{
		/* Begin memory allocation to pointers in global structures */

		//annual_tracker yearly lists...
		//these are GLOBAL, YEARLY lists, one entry per year
		//(not stored across different iterations... a new set
		//of values is recorded in annual_tracker at every iteration)

		//annual_tracker.future_Nrt = new double[NumberYears];//create an array of future permanent rights values
		//annual_tracker.No = new double[NumberYears];
		annual_tracker.annual_demand = new double[NumberYears];
		annual_tracker.annual_nlpl = new double[NumberYears];
		annual_tracker.annual_NW = new double[NumberYears];
		annual_tracker.early_NW = new double[NumberYears];
		annual_tracker.end_of_year_water = new double[NumberYears];
		annual_tracker.late_demand = new double[NumberYears];
		annual_tracker.late_NW = new double[NumberYears];
		annual_tracker.option_tracker = new double[NumberYears];
		annual_tracker.Pl5 = new double[NumberYears];
		annual_tracker.portfolio_cost = new double[NumberYears];
		annual_tracker.ToLeasePrice = new double[NumberYears];
		//annual_tracker.Nx_tracker = new double[NumberYears];
		//annual_tracker.Po_list = new double[NumberYears];

		//Allocate futures array: demand, ENr
		//Needed since demand_mean, demand_std_dev, ENr are declared as pointers in the struct
		//added samples stuff in here
		for (int month_it = 0; month_it < 12; month_it++)
		{
			//futures structure
			futures[month_it].demand_mean = new double [NumberYears];
			futures[month_it].demand_std_dev = new double [NumberYears];
			futures[month_it].ENr = new double [NumberYears];
			//samples structure
			//if (!params.sync_flag)
			//{
				//these allocations are done earlier if you are doing 
				//"synchronous" sampling -- see init_LRGV()

				samples[month_it].demand = new double [NumberYears];
				samples[month_it].inf = new double [NumberYears];
				samples[month_it].lease_high_res = new double [NumberYears];
				samples[month_it].lease_low_res = new double [NumberYears];
				samples[month_it].los = new double [NumberYears];
				//samples[month_it].NW = new double [NumberYears];
				samples[month_it].res_var = new double [NumberYears];
			//}
		}

		//allocate sims_years lists, that is, there's a value stored at
		//every year, also for every simulation.  These are GLOBAL lists
		//which cycle over every YEAR and every SIMULATION (but not every month).
		//each allocation here is for a multi-dimensional array of sims x years

		//sims_years_allocate sets each array member to zero.

		general_2d_allocate(sims_years_tracker.early_demand, NumberSims, NumberYears);
		general_2d_allocate(sims_years_tracker.early_NW, NumberSims, NumberYears);
		general_2d_allocate(sims_years_tracker.end_of_yr_water_tracker, NumberSims, NumberYears);
		general_2d_allocate(sims_years_tracker.ex_option_cost_list, NumberSims, NumberYears);
		general_2d_allocate(sims_years_tracker.late_demand, NumberSims, NumberYears);
		general_2d_allocate(sims_years_tracker.late_NW, NumberSims, NumberYears);
		general_2d_allocate(sims_years_tracker.Pl5, NumberSims, NumberYears);
		general_2d_allocate(sims_years_tracker.ToLeasePrice, NumberSims, NumberYears);
		general_2d_allocate(sims_years_tracker.total_Nx_tracker, NumberSims, NumberYears);
		general_2d_allocate(sims_years_tracker.total_Po_list, NumberSims, NumberYears);
		general_2d_allocate(sims_years_tracker.yearly_exercised_options, NumberSims, NumberYears);
		general_2d_allocate(sims_years_tracker.yearly_purchased_leases, NumberSims, NumberYears);
		general_2d_allocate(sims_years_tracker.No, NumberSims, NumberYears);

		//Now allocate sims_years_monthly lists, where there's a value stored
		//for every MONTH, YEAR, and ITERATION...
		for (int month_it=0; month_it<12; month_it++)
		{
			general_2d_allocate(monthly_tracker[month_it].inflows, NumberSims, NumberYears);
			general_2d_allocate(monthly_tracker[month_it].lease_high_res, NumberSims, NumberYears);
			general_2d_allocate(monthly_tracker[month_it].lease_low_res, NumberSims, NumberYears);
			general_2d_allocate(monthly_tracker[month_it].losses, NumberSims, NumberYears);
			general_2d_allocate(monthly_tracker[month_it].random_monthly_demand, NumberSims, NumberYears);
			general_2d_allocate(monthly_tracker[month_it].res_level_tracker, NumberSims, NumberYears);
			general_2d_allocate(monthly_tracker[month_it].res_threshold_tracker, NumberSims, NumberYears);
			general_2d_allocate(monthly_tracker[month_it].res_var, NumberSims, NumberYears);
			general_2d_allocate(monthly_tracker[month_it].total_cfailures, NumberSims, NumberYears);
			general_2d_allocate(monthly_tracker[month_it].total_failures, NumberSims, NumberYears);
			general_2d_allocate(monthly_tracker[month_it].total_monthly_leases, NumberSims, NumberYears);
			general_2d_allocate(monthly_tracker[month_it].yearly_lease_cost, NumberSims, NumberYears);
			general_2d_allocate(monthly_tracker[month_it].dropped_transfer_tracker, NumberSims, NumberYears);
			//NEW tracker for the C++ translation ...
			general_2d_allocate(monthly_tracker[month_it].av_water, NumberSims, NumberYears);
			general_2d_allocate(monthly_tracker[month_it].Nr, NumberSims, NumberYears);
		}

		initial_call = 0;
	} //end if initial_call == 1

	/* End memory allocation for pointers */
	return;
}