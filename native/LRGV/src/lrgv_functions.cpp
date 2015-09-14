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

void transfer_tracker(double *(&TransferTracker), double AvWater, double Demand)
{
	double NeededTransfers = 0.0;

	/* Begin Transfer Tracker section*/

	//"Transfer Tracking Section, roll the vector"
	for (int nu = 11; nu >= 0; nu--)
	{
		//start at index 12, make the index 12 member
		//equal to the index 11 member...
		TransferTracker[nu+1] = TransferTracker[nu];
	}

	//and when you get to the zero-th member, you zero it out.
	//since you've moved all the other members up
	//the last transfer in your bunch has expired.
	TransferTracker[0] = 0;
	
	//utilize transfers...

	if (AvWater <= Demand) 
	{
		//either: you met demand EXACTLY, or you had a failure.  Either way,
		//you know that at the last month, you used all your transfers...so the
		//tracker should be set to empty.
		zeroes(TransferTracker, 13);
	}

	if (AvWater > Demand && (AvWater - sum(TransferTracker,13)) < Demand)
	{
		//if we had enough water, and also:
		//the difference between the amount of water we had and the
		//amount in our transfer account ... was less than the demand
		//(that is, you only needed a portion of your transfers) Now, the
		//transfer tracker sees where that water came from, by attempting to
		//pull the water out of the tracker in the order which you purchased
		//your transfers (oldest first)
		
		//NeededTransfers is how much you needed to take from your
		//leases to fulfill water last time.
		NeededTransfers = Demand - (AvWater - sum(TransferTracker,13));

		//declare a local counter here, and pop it into a while loop
		int nu = 12; //the highest index is 12 (in Matlab it is 13)
		
		//while you still need to pull water out and you're not at the end of the list...
		while (NeededTransfers > 0 && nu >=0) 
		{
			//if the nu-th value is not zero, we can try to pull some water
			//out of that list-member
			if (TransferTracker[nu] != 0)
			{
				//if you can get all you need (and more) from one list member...
				if (TransferTracker[nu] >= NeededTransfers)
				{
					//subtract out your needed transfers from the given member,
					TransferTracker[nu] = TransferTracker[nu] - NeededTransfers;
					//then zero-out your needed transfers (because you got all you need!)
					NeededTransfers = 0;
				}
				//otherwise, you're going to pull out all you can from the
				//given member, and keep looking for some more water
				else
				{
					NeededTransfers = NeededTransfers - TransferTracker[nu];
					TransferTracker[nu] = 0;
				}
				
			}//if(TransferTracker[nu] !=0)

			nu = nu - 1; //we can try and move to the next list member.

			//and this loop is designed such that you can only get into it
			//if you know that the model needed water last month but didn't
			//need your entire amount of transfers.  So, there's a guaranteed
			//amount of water left over in the Tracker when you're done.  The
			//tracking is done after the usage calculation, so you're just
			//"cleaning up" and bookkeeping at this point.

		}//while(NeededTransfers> 0 && nu >=0)

	}//if(AvWater>Demand&& ... )

	return;
}


double resilience_calc()
{
	double current_value = 0.0; //fixed 3/23/2010 -- this value is pulling members from the failures matrix,
								//and apparently this matrix was comprised of doubles (even though it should just
								//be zeros and ones
	double previous_value = 0;
	int number_of_events = 0;
	double raw_resilience = 0.0;
	double temp_sum = 0.0;

	double *calc_vector;
	//general_1d_allocate(calc_vector, params.NumberSims*params.NumberYears); //wrong (see next line)
	general_1d_allocate(calc_vector, 12*params.NumberYears); //JRK fix 2009-06-15_1 version

	int k = 0;

	for (int sims_it = 0; sims_it < params.NumberSims; sims_it++)
	{
		k = 0;
		
		for (int year_it = 0; year_it < params.NumberYears; year_it++)
		{
			for (int month_it = 0; month_it < 12; month_it++)
			{
				calc_vector[k] = monthly_tracker[month_it].total_failures[sims_it][year_it];
				k++;
			}
		}
	
		for (k = 0; k < 12*params.NumberYears; k++) //fixed on 2009-06-17
		{
			current_value = calc_vector[k];
			if (current_value > 0)
			{
				if (previous_value == 0) number_of_events++; //a new event
				raw_resilience++; //time of event
			}
			previous_value = current_value;
		}

		if (number_of_events == 0 || raw_resilience == 0)
		{
			temp_sum = temp_sum + 1.0;
		}
		else
		{
			temp_sum = temp_sum + ((double) number_of_events / (double) raw_resilience);
		}
	}
	zap(calc_vector);

	return ((1/(double) params.NumberSims)*temp_sum);
}

double failvol_calc()
{
	double temp_sum = 0.0;
	double placeholder = 0.0;

	for (int sims_it = 0; sims_it < params.NumberSims; sims_it++)
	{
		for (int year_it = 0; year_it < params.NumberYears; year_it++)
		{
			for (int month_it = 0; month_it < 12; month_it++)
			{
				if (monthly_tracker[month_it].av_water[sims_it][year_it] < monthly_tracker[month_it].random_monthly_demand[sims_it][year_it])
					placeholder = monthly_tracker[month_it].random_monthly_demand[sims_it][year_it]-monthly_tracker[month_it].av_water[sims_it][year_it];
				else placeholder = 0.0;
				temp_sum = temp_sum + placeholder;
			}
		}
	}
	return ((1/(double) params.NumberSims)*temp_sum);
}
void vulnerability_calc(int local_sims, int local_years)
{
	double *calc_vector;
	int k, period_counter, temp_currentperiodmax, temp_outermax_periods, total_periods;
	double temp_sum, temp_currentmax, temp_outersum;
	general_1d_allocate(calc_vector, 12*local_years);
	
	temp_outersum = 0.0; //for doing an expectation across monte carlo simulations
	temp_outermax_periods = 0;
	total_periods = 0;
	for (int sims_it = 0; sims_it < local_sims; sims_it++)
	{
		//we are within the (sims_it)th time series.  We pull out all 12*(NumberYears) values for
		//failure volume regardless of month or year.
		k = 0;
		for (int year_it = 0; year_it < local_years; year_it++)
		{
			for (int month_it = 0; month_it < 12; month_it++)
			{
				if (monthly_tracker[month_it].av_water[sims_it][year_it] < monthly_tracker[month_it].random_monthly_demand[sims_it][year_it])
					calc_vector[k] = monthly_tracker[month_it].random_monthly_demand[sims_it][year_it]-monthly_tracker[month_it].av_water[sims_it][year_it];
				else calc_vector[k] = 0.0;
				k++;
			}
		}
		//now, we take this time series and perform some calculations.  We are looking for the
		//sojourn into failure that has the highest volume.
		k = 0;
		temp_sum = 0.0;
		temp_currentmax = 0.0;
		temp_currentperiodmax = 0;
		
		//cout << "Inside the vulnerability calc, local_years is " << local_years << endl;
		
		while (k < 12*local_years)
		{
			temp_sum = 0.0;
			//count the total failure volume in this sojourn.  The do-while control structure
			//will count a failure volume even if there is only one period in this sojourn.
			period_counter = 0;
			do
			{
				if (calc_vector[k] > 0.0)
				{
					temp_sum += calc_vector[k];
					period_counter++;
					total_periods++;
					k++;
				}
			}while(calc_vector[k] > 0.0 && k < 12*local_years); //added k < 12*local_years, 3/29/2010
			//since the summation is over for this sojourn, we check to see if ours was the highest
			if (temp_sum > temp_currentmax) temp_currentmax = temp_sum;
			if (period_counter > temp_currentperiodmax) temp_currentperiodmax = period_counter;
			//once we are past this code, we know we have to look for the next sojourn, so k is iterated
			//so that we can continue stepping through the timeseries
			k++;			
		}
		temp_outersum += temp_currentmax;
		if (temp_currentperiodmax > temp_outermax_periods) temp_outermax_periods = temp_currentperiodmax;
	}
	
	zap(calc_vector);
	//For vulnerability, the final returned value is the expected value of the maximum total failure volumes.
	g.vulnerability = temp_outersum / local_sims;
	//For failure periods, this value is the expected value of maximum failure period length.
	g.failure_periods = temp_outermax_periods;
	g.total_periods = total_periods; //only used in drought for now, strict sum
	//cout << "vulnerability calc over" << endl;
}
void single_sync_sampling()
{
	int NumberYears = params.NumberYears;
	int NumberSims = params.NumberSims;
	super.y = new year_structure[NumberSims];
	for (int sims_it = 0; sims_it < NumberSims; sims_it++)
	{
		for (int month_it = 0; month_it < 12; month_it++)
		{
			//have to allocate memory first for each sample and month...
			super.y[sims_it].samples[month_it].demand = new double [NumberYears];
			super.y[sims_it].samples[month_it].inf = new double [NumberYears];
			super.y[sims_it].samples[month_it].lease_high_res = new double [NumberYears];
			super.y[sims_it].samples[month_it].lease_low_res = new double [NumberYears];
			super.y[sims_it].samples[month_it].los = new double [NumberYears];
			//super.y[sims_it].samples[month_it].NW = new double [NumberYears];
			super.y[sims_it].samples[month_it].res_var = new double [NumberYears];
		}
	}

	if (params.sync_flag)
	{
		for (int sims_it = 0; sims_it < NumberSims; sims_it++)
		{
			for (int month_it = 0; month_it < 12; month_it++)
			{
				randsample(hydro_sets[month_it], super.y[sims_it].samples[month_it], NumberYears);
				randsample(lease_sets[month_it], super.y[sims_it].samples[month_it], NumberYears);
				randsample(demand_sets[month_it], super.y[sims_it].samples[month_it], NumberYears, params.DemGrowthFactor);
				//We have: 12 samples structures, one for each month
				//with enough randomly-sampled values to run
				//one n-iteration (monte carlo iteration).
			}
		}
	}
	else
	{
		for (int month_it = 0; month_it < 12; month_it++)
		{
			super.y[0].samples[month_it].inf[0] = hydro_sets[month_it].inf[params.single_year];
			super.y[0].samples[month_it].los[0] = hydro_sets[month_it].los[params.single_year];
			super.y[0].samples[month_it].res_var[0] = hydro_sets[month_it].res_var[params.single_year];

			super.y[0].samples[month_it].lease_high_res[0] = max_array(lease_sets[month_it].high_res, (int)lease_sets[month_it].high_res.size());
			super.y[0].samples[month_it].lease_low_res[0] = max_array(lease_sets[month_it].low_res, (int)lease_sets[month_it].low_res.size());

			super.y[0].samples[month_it].demand[0] = max_array(demand_sets[month_it].demand,(int)demand_sets[month_it].demand.size())*pow((1+params.DemGrowthFactor), 9);	
		}
	}
}
void init_LRGV(char** argv, string init_param)
{
	//You always call the ten-year option first, then you use the
	//information from that call within the drought calculations.
	if (init_param == "ten-year")
	{
		filenames_structure filenames;

		filenames.hydro = "hydro.txt";
		filenames.lease = "lease.txt";
		filenames.demand = "demand.txt";

		//these filenames don't depend on things in the control file:
		//REMOVED 9/21/2012
//#ifndef LRGV_SAMPLER
//		params.filename_base = new char[255];
//		strcpy(params.filename_base, argv[2]);
//#endif
		//END REMOVED
		filenames.control = params.filename_base + "_control.txt";
		filenames.timing = params.filename_base + "_timing.txt";
		filenames.rng = params.filename_base + "_random_ensemble.txt";
		filenames.out = params.filename_base + "_results-full.txt";

		hist_read(filenames); //read control file (*_control.txt) and historical data
		
		//these filenames do depend on things in the control file:

		//results filename is based on delimiter.
		if (params.delim_flag == 0)
		{
			filenames.results = params.filename_base + "_results-yearly.csv";
			filenames.monthly = params.filename_base + "_results-monthly.csv";
		}
		else
		{
			filenames.results = params.filename_base + "_results-yearly.txt";
			filenames.monthly = params.filename_base + "_results-monthly.txt";
		}

		//open output files
		if (params.timing_flag) time_stream.open(filenames.timing.c_str());
		if (params.results_flag)
		{
			results_stream.open(filenames.results.c_str());
			if (params.results_header_flag) write_results_header(filenames, init_param);
		}
		if (params.monthly_flag)
		{
			monthly_stream.open(filenames.monthly.c_str());
			if (params.monthly_header_flag) write_monthly_header(filenames);
		}
		if (params.rng_flag) rng_stream.open(filenames.rng.c_str());
		if (params.out_flag) out_stream.open(filenames.out.c_str());
		
		if (params.timing_flag)
		{
			endtime = clock()-start;
			time_stream << CLOCKS_PER_SEC << "   " << endtime << "   " << endl;
		}
		
		sampler_count = 0;
		if (params.single_flag || params.sync_flag) single_sync_sampling();
	}
	if (init_param == "drought-full")
	{
		filenames_structure filenames;
		
		//Added these back in, 2012-10-01. Could've corrupted memory

		filenames.hydro = "hydro.txt";
		filenames.lease = "lease.txt";
		filenames.demand = "demand.txt";

		//these filenames don't depend on things in the control file:
		//REMOVED 9/21/2012.  The filename base should already be taken care of in main.
//#ifndef LRGV_SAMPLER
//		params.filename_base = new char[255];
//		strcpy(params.filename_base, argv[2]);
//#endif

		filenames.control = params.filename_base + "_control.txt";
		filenames.timing = params.filename_base + "_timing.txt";
		filenames.rng = params.filename_base + "_random_ensemble.txt";
		filenames.out = params.filename_base + "_results-full.txt";

		hist_read(filenames); //read control file (*_control.txt) and historical data
		
		//TEST: We'll try rewriting some of the params variables, in order to
		//run the drought with the same control file as the ten-year.

		drought.NumberSims = 1;
		drought.NumberYears = 1;
		drought.single_year = 19;
		drought.single_flag = 1;

		params.NumberSims = drought.NumberSims;
		params.NumberYears = drought.NumberYears;
		params.single_year = drought.single_year;
		params.single_flag = drought.single_flag;

		//these filenames do depend on things in the control file:

		//results filename is based on delimiter.
		if (params.delim_flag == 0)
		{
			filenames.results = params.filename_base + "_results-yearly.csv";
			filenames.monthly = params.filename_base + "_results-monthly.csv";
		}
		else
		{
			filenames.results = params.filename_base + "_results-yearly.txt";
			filenames.monthly = params.filename_base + "_results-monthly.txt";
		}

		//open output files
		if (params.timing_flag) time_stream.open(filenames.timing.c_str());
		if (params.results_flag)
		{
			results_stream.open(filenames.results.c_str());
			if (params.results_header_flag) write_results_header(filenames, init_param);
		}
		if (params.monthly_flag)
		{
			monthly_stream.open(filenames.monthly.c_str());
			if (params.monthly_header_flag) write_monthly_header(filenames);
		}
		if (params.rng_flag) rng_stream.open(filenames.rng.c_str());
		if (params.out_flag) out_stream.open(filenames.out.c_str());
		
		if (params.timing_flag)
		{
			endtime = clock()-start;
			time_stream << CLOCKS_PER_SEC << "   " << endtime << "   " << endl;
		}
		
		sampler_count = 0;
		//if (params.single_flag || params.sync_flag) single_sync_sampling();	
		
		int NumberSims = drought.NumberSims;
		int NumberYears = drought.NumberYears;
		super.y = new year_structure[NumberSims];
		for (int sims_it = 0; sims_it < NumberSims; sims_it++)
		{
			for (int month_it = 0; month_it < 12; month_it++)
			{
				//have to allocate memory first for each sample and month...
				super.y[sims_it].samples[month_it].demand = new double [NumberYears];
				super.y[sims_it].samples[month_it].inf = new double [NumberYears];
				super.y[sims_it].samples[month_it].lease_high_res = new double [NumberYears];
				super.y[sims_it].samples[month_it].lease_low_res = new double [NumberYears];
				super.y[sims_it].samples[month_it].los = new double [NumberYears];
				//super.y[sims_it].samples[month_it].NW = new double [NumberYears];
				super.y[sims_it].samples[month_it].res_var = new double [NumberYears];
			}
		}
		//this assumes the multi-year scenario is not using synchronous sampling
		//(it is overwriting the super structure)
		for (int month_it = 0; month_it < 12; month_it++)
		{
			super.y[0].samples[month_it].inf[0] = hydro_sets[month_it].inf[drought.single_year];
			super.y[0].samples[month_it].los[0] = hydro_sets[month_it].los[drought.single_year];
			super.y[0].samples[month_it].res_var[0] = hydro_sets[month_it].res_var[drought.single_year];

			super.y[0].samples[month_it].lease_high_res[0] = max_array(lease_sets[month_it].high_res, (int)lease_sets[month_it].high_res.size());
			super.y[0].samples[month_it].lease_low_res[0] = max_array(lease_sets[month_it].low_res, (int)lease_sets[month_it].low_res.size());

			super.y[0].samples[month_it].demand[0] = max_array(demand_sets[month_it].demand,(int)demand_sets[month_it].demand.size())*pow((1+params.DemGrowthFactor), 9);	
		}
	}
	else if(init_param == "drought_noinit")
	{
		//we are going to store things that are different than the ten-year
		//in the drought structure.
		drought.NumberSims = 1;
		drought.NumberYears = 1;
		drought.single_year = 19;
		drought.single_flag = 1;
		int NumberSims = drought.NumberSims;
		int NumberYears = drought.NumberYears;
		super.y = new year_structure[NumberSims];
		for (int sims_it = 0; sims_it < NumberSims; sims_it++)
		{
			for (int month_it = 0; month_it < 12; month_it++)
			{
				//have to allocate memory first for each sample and month...
				super.y[sims_it].samples[month_it].demand = new double [NumberYears];
				super.y[sims_it].samples[month_it].inf = new double [NumberYears];
				super.y[sims_it].samples[month_it].lease_high_res = new double [NumberYears];
				super.y[sims_it].samples[month_it].lease_low_res = new double [NumberYears];
				super.y[sims_it].samples[month_it].los = new double [NumberYears];
				//super.y[sims_it].samples[month_it].NW = new double [NumberYears];
				super.y[sims_it].samples[month_it].res_var = new double [NumberYears];
			}
		}
		//this assumes the multi-year scenario is not using synchronous sampling
		//(it is overwriting the super structure)
		for (int month_it = 0; month_it < 12; month_it++)
		{
			super.y[0].samples[month_it].inf[0] = hydro_sets[month_it].inf[drought.single_year];
			super.y[0].samples[month_it].los[0] = hydro_sets[month_it].los[drought.single_year];
			super.y[0].samples[month_it].res_var[0] = hydro_sets[month_it].res_var[drought.single_year];

			super.y[0].samples[month_it].lease_high_res[0] = max_array(lease_sets[month_it].high_res, (int)lease_sets[month_it].high_res.size());
			super.y[0].samples[month_it].lease_low_res[0] = max_array(lease_sets[month_it].low_res, (int)lease_sets[month_it].low_res.size());

			super.y[0].samples[month_it].demand[0] = max_array(demand_sets[month_it].demand,(int)demand_sets[month_it].demand.size())*pow((1+params.DemGrowthFactor), 9);	
		}

	}

	//simulator.setInitialCall(1);
}


void transform_LRGV(double* vars)
{
	//DEBUG 10-01
	//cout << "Calling transform function..." << endl;
	//END DEBUG
	
	//if (params.timing_flag) start = clock();
	//check if this should be called in the sampler, I don't think it should be.
	double temp;
	
	if (params.model_case == 2 || params.model_case == 3)
	{
		//delta_o_high discretization
		if (vars[2] < 0.05) vars[2] = 0;
		else if (vars[2] < 0.15) vars[2] = 0.1;
		else if (vars[2] < 0.25) vars[2] = 0.2;
		else if (vars[2] < 0.35) vars[2] = 0.3;
		else if (vars[2] < 0.45) vars[2] = 0.4;
		else if (vars[2] < 0.55) vars[2] = 0.5;
		else if (vars[2] < 0.65) vars[2] = 0.6;
		else if (vars[2] < 0.75) vars[2] = 0.7;
		else if (vars[2] < 0.85) vars[2] = 0.8;
		else if (vars[2] < 0.95) vars[2] = 0.9;
		else vars[2] = 1.0;

		//xi discretization
		if (vars[3] < 0.125) vars[3] = 0.10;
		else if (vars[3] < 0.175) vars[3] = 0.15;
		else if (vars[3] < 0.225) vars[3] = 0.20;
		else if (vars[3] < 0.275) vars[3] = 0.25;
		else if (vars[3] < 0.325) vars[3] = 0.30;
		else if (vars[3] < 0.375) vars[3] = 0.35;
		else vars[3] = 0.40;

		//alpha/beta discretization
		temp = vars[4];
		vars[4] = rounding(temp, 2.0);
		temp = vars[5];
		vars[5] = rounding(temp, 2.0);

		//alpha2 / beta2
		if (vars[4] + vars[5] > 3.0) vars[5] = 3.0 - vars[4]; //transform
		
		//alpha / beta
		if (params.model_case == 3)
		{
			temp = vars[6];
			vars[6] = rounding(temp, 2.0);
			temp = vars[7];
			vars[7] = rounding(temp, 2.0);
			if (vars[6] + vars[7] > 3.0) vars[7] = 3.0 - vars[6];
		}
	}
	else if (params.model_case == 4 || params.model_case == 5)
	{
		temp = vars[2];
		vars[2] = rounding(temp, 2.0);
		if (params.model_case == 5)
		{
			temp = vars[3];
			vars[3] = rounding(temp,2.0);
		}
	}
	else if (params.model_case == 6)
	{
		temp = vars[2];
		vars[2] = rounding(temp, 2.0);
		temp = vars[3];
		vars[3] = rounding(temp, 2.0);
		temp = vars[4];
		vars[4] = rounding(temp, 2.0);
		temp = vars[5];
		vars[5] = rounding(temp, 2.0);
		
		//alpha2 / beta2
		if (vars[2] + vars[3] > 3.0) vars[3] = 3.0 - vars[2];

		if (vars[4] + vars[5] > 3.0) vars[5] = 3.0 - vars[4];
	}
	//if (params.timing_flag)
	//{
	//	end = clock() - start;
	//	time_stream << end << endl;
	//}

	return;
}

void calc_LRGV(double* vars, double* objs, double* consts, string calc_param)
{
	//new wrapper for object-oriented call.
	//can later be integrated into problemdef.cpp...
	simulator.calc_LRGV(vars, objs, consts, calc_param);
}

//void calc_LRGV(Simulation &sim, Individual *ind, char *calc_param)
//{
//	//new wrapper for object-oriented call.
//	//can later be integrated into problemdef.cpp...
//	sim.calc_LRGV(ind, calc_param);
//	//simulator.calc_LRGV(ind, calc_param);
//}

void finalize_LRGV()
{
	/* Deallocate memory */
	//note futures, monthly_tracker, and samples were statically
	//allocated to 12 members each.
	zap(annual_tracker.annual_demand);
	zap(annual_tracker.annual_nlpl);
	zap(annual_tracker.annual_NW);
	zap(annual_tracker.early_NW);
	zap(annual_tracker.end_of_year_water);
	zap(annual_tracker.late_demand);
	zap(annual_tracker.late_NW);
	zap(annual_tracker.option_tracker);
	zap(annual_tracker.Pl5);
	zap(annual_tracker.portfolio_cost);
	zap(annual_tracker.ToLeasePrice);

	for (int month_it = 0; month_it < 12; month_it++)
	{

		zap(futures[month_it].demand_mean);
		zap(futures[month_it].demand_std_dev);
		zap(futures[month_it].ENr);
		zap(samples[month_it].demand);
		zap(samples[month_it].inf);
		zap(samples[month_it].lease_high_res);
		zap(samples[month_it].lease_low_res);
		zap(samples[month_it].los);
		zap(samples[month_it].res_var);
	}
	
	int NumberSims = params.NumberSims;
	zap(sims_years_tracker.early_demand, NumberSims);
	zap(sims_years_tracker.early_NW, NumberSims);
	zap(sims_years_tracker.end_of_yr_water_tracker, NumberSims);
	zap(sims_years_tracker.ex_option_cost_list, NumberSims);
	zap(sims_years_tracker.late_demand, NumberSims);
	zap(sims_years_tracker.late_NW, NumberSims);
	zap(sims_years_tracker.Pl5, NumberSims);
	zap(sims_years_tracker.ToLeasePrice, NumberSims);
	zap(sims_years_tracker.total_Nx_tracker, NumberSims);
	zap(sims_years_tracker.total_Po_list, NumberSims);
	zap(sims_years_tracker.yearly_exercised_options, NumberSims);
	zap(sims_years_tracker.yearly_purchased_leases, NumberSims);
	zap(sims_years_tracker.No, NumberSims);

	for (int month_it=0; month_it<12; month_it++)
	{
		zap(monthly_tracker[month_it].inflows, NumberSims);
		zap(monthly_tracker[month_it].lease_high_res, NumberSims);
		zap(monthly_tracker[month_it].lease_low_res, NumberSims);
		zap(monthly_tracker[month_it].losses, NumberSims);
		zap(monthly_tracker[month_it].random_monthly_demand, NumberSims);
		zap(monthly_tracker[month_it].res_level_tracker, NumberSims);
		zap(monthly_tracker[month_it].res_threshold_tracker, NumberSims);
		zap(monthly_tracker[month_it].res_var, NumberSims);
		zap(monthly_tracker[month_it].total_cfailures, NumberSims);
		zap(monthly_tracker[month_it].total_failures, NumberSims);
		zap(monthly_tracker[month_it].total_monthly_leases, NumberSims);
		zap(monthly_tracker[month_it].yearly_lease_cost, NumberSims);
		zap(monthly_tracker[month_it].dropped_transfer_tracker, NumberSims);
		zap(monthly_tracker[month_it].av_water, NumberSims);
		zap(monthly_tracker[month_it].Nr, NumberSims);
	}

	zap(params.initrights_vec);

	if(params.timing_flag) time_stream.close();
	if(params.results_flag) results_stream.close();
	if(params.rng_flag) rng_stream.close();
	if(params.out_flag) out_stream.close();
	if(params.monthly_flag) monthly_stream.close();

	return;
}
