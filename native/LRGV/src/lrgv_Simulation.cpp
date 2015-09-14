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

Simulation::Simulation()
{
	initial_call = 1;
	TransferTracker = NULL;
}

Simulation::~Simulation()
{
	//cout << "Hi there from the destructor." << endl;
	zap(TransferTracker);
	zap(LeaseCost, NumberYears);
	zap(NumberPurchasedLeases, NumberYears);
	zap(NxTracker);
	zap(PoList);
	zap(Failures, NumberYears);
	zap(CFailures, NumberYears);
	zap(FailureRecord, NumberYears);
	zap(CFailureRecord, NumberYears);
	zap(Reliability);
	zap(CReliability);
	zap(AverageMonthlyLeaseCost, NumberYears);
	zap(AverageYearlyLeaseCost);
	zap(TotalYearlyLeaseCost, NumberSims);
	zap(TotalAnnualCosts, NumberSims);
	zap(PermRightCosts, NumberSims);
	zap(AverageAnnualCosts);
	zap(StdDevInput);
	zap(StdDevCost);
	zap(cost_sorting_array);
	zap(SortedTotalAnnualCosts, NumberSims);
	zap(VAR);
	zap(CVAR);
	zap(VARmean);
	zap(YearlyDroppedTransfers,NumberSims);
	zap(DroppedTransfersVector);
	zap(YearlyNumberTransfers, NumberSims);
	zap(NumberTransfersVector);
	//cout << "Destructor out!" << endl;
}

//Define as std::vector to allow using find
string arr1[] = {"cost", "surplus", "critrel", "drop", "rel", "cvar", "numleases", "drtranscost", "drvuln","aggcost","aggrel"};
extern const vector<string> obj_avail (arr1, arr1 + sizeof(arr1) / sizeof(arr1[0]) );
string arr2[] = {"rel", "critrel", "cvar", "drvuln"};
extern const vector<string> constr_avail (arr2, arr2 + sizeof(arr2) / sizeof(arr2[0]) );

void Simulation::calc_LRGV(double* vars, double* objs, double* consts, string calc_param)
{
	//increment the global sampler counter and output headers to output streams
	sampler_count = sampler_count + 1;
	//note: none of the output routines work for the associated drought structure yet
	if (params.rng_flag)
	{
		if (params.sync_flag)
		{
			if (sampler_count == 1) rng_stream << "RESULTS_FOR_SAMPLE_" << sampler_count << endl;
		}
		else
		{
			if (params.single_flag) //this shouldn't cause problems for now
			{
				if (sampler_count == 1) rng_stream << "RESULTS_FOR_SAMPLE_" << sampler_count << endl;
			}
			else rng_stream << "RESULTS_FOR_SAMPLE_" << sampler_count << endl;
		}
			
	}
	if (params.out_flag) out_stream << "RESULTS_FOR_SAMPLE_" << sampler_count << endl;
	if (params.timing_flag)
	{
		start = clock(); //total time
		timers.before_loop_start = clock(); //initial stuff
	}

 	//obj = ind->obj;
	//constr  = ind->constr;
	//xreal = ind->xreal;



	/* End objective variables declarations */

	/* Assign decision variables */

	//assumptions:
	//sampler will always do the [0, 1] variables
	//constant parameter file has read in the real value if dectransform_flag is zero and nothing needs to be done
	//if dectransform_flag is one, we have to transform the hard-coded value once at the beginning for each constant parameter

	if(params.mode == "resample") //legacy -- processing_flag 0
	{
		strategy.Nrt = rounding(0.5*(vars[0]+1.0)*params.Nrt_max);
		Nrt = strategy.Nrt;
		//case 1 specific stuff
		if (params.model_case == 1)
		{
			//CASE 1: Permanent Rights Only
			lease_flag = 0;
			options_flag = 0;
			alpha = 0;
			beta = 0;
			alpha2 = 0;
			beta2 = 0;
			No = 0;
		}
		//cases 2 and 3, WRR formulations for Cases B - D
		else if (params.model_case == 2 || params.model_case == 3)
		{
			if (params.model_case == 2) lease_flag = 0;
			else lease_flag = 1;

			options_flag = 1;

			strategy.No_low = rounding(vars[1]*params.No_max);
			strategy.No_high = rounding((1.0 + vars[2])*strategy.No_low);//10-1 fix
			strategy.xi = vars[3];
							
			strategy.alpha2 = vars[4];
			strategy.beta2 = vars[4]+ vars[5];
			if (params.model_case == 3)
			{
				strategy.alpha = vars[6];
				strategy.beta = vars[6] + vars[7];
			}
		}
		//case 4: de novo "simple strategy"
		else if (params.model_case == 4)
		{
			lease_flag = 1;
			options_flag = 1;
			strategy.No_low = rounding(vars[1]*params.No_max);
			strategy.No_high = strategy.No_low;
			strategy.xi = 0.4;
			strategy.alpha2 = vars[2];
			strategy.beta2 = strategy.alpha2;
			strategy.alpha = strategy.alpha2;
			strategy.beta = strategy.alpha2; //bug fixed 11/4/2009
		}
		//case 5: de novo "two alphas"
		else if (params.model_case == 5)
		{
			lease_flag = 1;
			options_flag = 1;
			strategy.No_low = rounding(vars[1]*params.No_max);
			strategy.No_high = strategy.No_low;
			strategy.xi = 0.4;
			strategy.alpha2 = vars[2];
			strategy.beta2 = strategy.alpha2;
			strategy.alpha = vars[3];
			strategy.beta = strategy.alpha; //bug fixed 11/4/2009
		}
		//case 6: de novo "two alphas with betas (no adaptive options)"
		else if (params.model_case == 6)
		{
			lease_flag = 1;
			options_flag = 1;
			strategy.No_low = rounding(vars[1]*params.No_max);
			strategy.No_high = strategy.No_low;
			strategy.xi = 0.4;
			strategy.alpha2 = vars[2];
			strategy.beta2 = vars[2]+ vars[3];
			strategy.alpha = vars[4];
			strategy.beta = vars[4] + vars[5];
		}
		//the last de novo case fully corresponds with the WRR Case D.

		//no matter what the case, assign the local alpha and betas here
		alpha = strategy.alpha;
		beta = strategy.beta;
		alpha2 = strategy.alpha2;
		beta2 = strategy.beta2;
		
		//moved assignment of high/low options to after this, when ifri has been assigned
	} //end code for resampling
	else if (params.mode == "sobol") //legacy: processing_flag == 2
	{
		//do the calculation for rights no matter what the case
		if (params.dectransform_flag == 1 && strategy.Nrt_order == params.total_possible_samples && initial_call == 1)
			strategy.Nrt = rounding(0.5*(strategy.Nrt+1.0)*params.Nrt_max); //we read constant value once
		else if (strategy.Nrt_order < params.total_possible_samples) //sampled param has to get set every time
			strategy.Nrt = rounding(0.5*(vars[0]+1.0)*params.Nrt_max);
		//There's an implicit else here -- if Nrt was hard-coded, it stays the same and it 
		Nrt = strategy.Nrt;

		//case 1 specific stuff
		if (params.model_case == 1)
		{
			//CASE 1: Permanent Rights Only
			lease_flag = 0;
			options_flag = 0;
			alpha = 0;
			beta = 0;
			alpha2 = 0;
			beta2 = 0;
			No = 0;
		}
		else if (params.model_case == 2 || params.model_case == 3) //changed to elseif on 11/8/10
		{
			if (params.model_case == 2) lease_flag = 0;
			else lease_flag = 1;

			options_flag = 1;
			
			if (params.dectransform_flag == 1 && strategy.No_low_order == params.total_possible_samples && initial_call == 1)
				strategy.No_low = rounding(strategy.No_low*params.No_max);
			else if (strategy.No_low_order < params.total_possible_samples)
				strategy.No_low = rounding(vars[1]*params.No_max);
			
			if (params.dectransform_flag == 1 && strategy.No_high_order == params.total_possible_samples && initial_call == 1)
				strategy.No_high = rounding((1.0 + strategy.No_high)*strategy.No_low);
			else if (strategy.No_high_order < params.total_possible_samples)
				strategy.No_high = rounding((1.0 + vars[2])*strategy.No_low);//10-1 fix
			
			if (strategy.xi_order < params.total_possible_samples) strategy.xi = vars[3];
			
			if (strategy.alpha2_order < params.total_possible_samples)			
				strategy.alpha2 = vars[4];
			
			// if  you are transforming and "hard-coding" the variables, you need to add the alpha
			// to the beta in order to continue...

			if (params.dectransform_flag == 1 && strategy.beta2_order == params.total_possible_samples && initial_call == 1)
				strategy.beta2 = strategy.alpha2 + strategy.beta2;
			else if (strategy.beta2_order < params.total_possible_samples)
				strategy.beta2 = strategy.alpha2 + vars[5]; //it is being sampled, but not truncated

			//new, 6/5/2009
			if (strategy.beta2 > 3.0) strategy.beta2 = 3.0; //the truncation
			
			if (params.model_case == 3)
			{
				if (strategy.alpha_order < params.total_possible_samples) strategy.alpha = vars[6];
				
				if (params.dectransform_flag == 1 && strategy.beta2_order == params.total_possible_samples && initial_call == 1)
					strategy.beta = strategy.alpha + strategy.beta;
				else if (strategy.beta_order < params.total_possible_samples)
					strategy.beta = strategy.alpha + vars[7]; //sampled not truncated

				if (strategy.beta > 3.0) strategy.beta = 3.0; //the truncation
			}
		//moved the assignment of No to after the ifri is sampled
		} //end if (params.model_case == 2 or 3)
		else if (params.model_case == 6)
		{
			lease_flag = 1;
			options_flag = 1;
			
			if (params.dectransform_flag == 1 && strategy.No_low_order == params.total_possible_samples && initial_call == 1)
				strategy.No_low = rounding(strategy.No_low*params.No_max);
			else if (strategy.No_low_order < params.total_possible_samples)
				strategy.No_low = rounding(vars[1]*params.No_max);
			
			strategy.No_high = strategy.No_low;
			strategy.xi = 0.4;		
			
			if (strategy.alpha2_order < params.total_possible_samples)			
				strategy.alpha2 = vars[4];
			
			// if  you are transforming and "hard-coding" the variables, you need to add the alpha
			// to the beta in order to continue...

			if (params.dectransform_flag == 1 && strategy.beta2_order == params.total_possible_samples && initial_call == 1)
				strategy.beta2 = strategy.alpha2 + strategy.beta2;
			else if (strategy.beta2_order < params.total_possible_samples)
				strategy.beta2 = strategy.alpha2 + vars[5]; //it is being sampled, but not truncated

			//new, 6/5/2009
			if (strategy.beta2 > 3.0) strategy.beta2 = 3.0; //the truncation
			
			if (strategy.alpha_order < params.total_possible_samples) strategy.alpha = vars[6];
				
			if (params.dectransform_flag == 1 && strategy.beta2_order == params.total_possible_samples && initial_call == 1)
				strategy.beta = strategy.alpha + strategy.beta;
			else if (strategy.beta_order < params.total_possible_samples)
				strategy.beta = strategy.alpha + vars[7]; //sampled not truncated

			if (strategy.beta > 3.0) strategy.beta = 3.0; //the truncation
			//moved the assignment of No to after the ifri is sampled

		}
		//no matter what the case
		alpha = strategy.alpha;
		beta = strategy.beta;
		alpha2 = strategy.alpha2;
		beta2 = strategy.beta2;
	} //end processing for sobol
	else if (params.mode == "std-io")
	{
		strategy.Nrt = rounding(0.5*(vars[0]+1.0)*params.Nrt_max);
		Nrt = strategy.Nrt;
		
		//DEBUG 10-01
		//cout << "Nrt = " << Nrt << "." << endl;
		//END DEBUG
		
		//case 1 specific stuff
		if (params.model_case == 1)
		{
			//CASE 1: Permanent Rights Only
			lease_flag = 0;
			options_flag = 0;
			alpha = 0;
			beta = 0;
			alpha2 = 0;
			beta2 = 0;
			No = 0;
		}
		//cases 2 and 3, WRR formulations for Cases B - D
		else if (params.model_case == 2 || params.model_case == 3)
		{
			if (params.model_case == 2) lease_flag = 0;
			else lease_flag = 1;

			options_flag = 1;

			strategy.No_low = rounding(vars[1]*params.No_max);
			strategy.No_high = rounding((1.0 + vars[2])*strategy.No_low);//10-1 fix
			strategy.xi = vars[3];
						
			strategy.alpha2 = vars[4];
			strategy.beta2 = vars[4]+ vars[5];
			if (params.model_case == 3)
			{
				strategy.alpha = vars[6];
				strategy.beta = vars[6] + vars[7];
			}
		}
	//case 4: de novo "simple strategy"
		else if (params.model_case == 4)
		{
			lease_flag = 1;
			options_flag = 1;
			strategy.No_low = rounding(vars[1]*params.No_max);
			strategy.No_high = strategy.No_low;
			strategy.xi = 0.4;
			strategy.alpha2 = vars[2];
			strategy.beta2 = strategy.alpha2;
			strategy.alpha = strategy.alpha2;
			strategy.beta = strategy.alpha2; //bug fixed 11/4/2009
		}
		//case 5: de novo "two alphas"
		else if (params.model_case == 5)
		{
			lease_flag = 1;
			options_flag = 1;
			strategy.No_low = rounding(vars[1]*params.No_max);
			strategy.No_high = strategy.No_low;
			strategy.xi = 0.4;
			strategy.alpha2 = vars[2];
			strategy.beta2 = strategy.alpha2;
			strategy.alpha = vars[3];
			strategy.beta = strategy.alpha; //bug fixed 11/4/2009
		}
		//case 6: de novo "two alphas with betas (no adaptive options)"
		else if (params.model_case == 6)
		{
			lease_flag = 1;
			options_flag = 1;
			strategy.No_low = rounding(vars[1]*params.No_max);
			strategy.No_high = strategy.No_low;
			strategy.xi = 0.4;
			strategy.alpha2 = vars[2];
			strategy.beta2 = vars[2]+ vars[3];
			strategy.alpha = vars[4];
			strategy.beta = vars[4] + vars[5];
		}
		//the last de novo case fully corresponds with the WRR Case D.
	
		alpha = strategy.alpha;
		beta = strategy.beta;
		alpha2 = strategy.alpha2;
		beta2 = strategy.beta2;
		//moved assignment of high/low options to after this, when ifri has been assigned	
	}
//DEBUG 10-01
//cout << "alpha = " << alpha << ", beta =" << beta << ", alpha2 = " << alpha2 << ", beta2 = " << beta2 << endl;
//cout << "Nolow = " << strategy.No_low << ", Nohigh = " << strategy.No_high << ", xi = " << strategy.xi << "." << endl;
//END DEBUG
	/* End Assign Decision Variables */

	/* Do Roulette Calculations */
	if (params.roulette_flag)
	{
		//DEBUG 10-01
//		cout << "Calling roulette." << endl;
		//END DEBUG
		init_roulette();
	}
	/* End Roulette Calculations */

	/* Assign local parameters */

	critical_reliability_threshold = params.critical_reliability_threshold;
	DemGrowthFactor = params.DemGrowthFactor;

	//ifri stuff moved inside Monte Carlo loop

	in_loss = params.in_loss;
	iRo = params.iRo;
	OExerciseMonth = params.OExerciseMonth;
	Po = params.Po;
	Pr = params.Pr;
	Px = params.Px;
	reservoir_threshold = params.reservoir_threshold;
	ReservoirCriticalLevel = params.ReservoirCriticalLevel;
	TWR = params.TWR;
	
	if (calc_param == "ten-year")
	{
		NumberSims = params.NumberSims;
		NumberYears = params.NumberYears;
		single_year = -1;
		single_flag = 0;
	}
	else if (calc_param == "drought_full" || calc_param == "drought_noinit")
	{
		NumberSims = drought.NumberSims;
		NumberYears = drought.NumberYears;
		single_year = 19;
		single_flag = 1;
	}

	/* End Assign Local Parameters */

	//the historical files are read in once outside of this function
	//(samples and hydro-demand-lease are global variables)
	
	//the random number generator is also initialized outside of this function
	
	//allocate memory to local lists
	if (initial_call)
	{
		//cout << "Doing some allocation." << endl;
		general_1d_allocate(TransferTracker, 13);
		general_2d_allocate(LeaseCost, NumberYears, 12); //then allocate memory
		general_2d_allocate(NumberPurchasedLeases, NumberYears, 12);
		general_1d_allocate(NxTracker, NumberYears);
		general_1d_allocate(PoList, NumberYears);
		general_2d_allocate(Failures, NumberYears, 12);
		general_2d_allocate(CFailures, NumberYears, 12);
		general_2d_allocate(FailureRecord, NumberYears, 12);
		general_2d_allocate(CFailureRecord, NumberYears, 12);
		general_1d_allocate(Reliability, NumberYears);
		general_1d_allocate(CReliability, NumberYears);
		general_2d_allocate(AverageMonthlyLeaseCost, NumberYears, 12);
		general_1d_allocate(AverageYearlyLeaseCost, NumberYears);
		general_2d_allocate(TotalYearlyLeaseCost, NumberSims, NumberYears);
		general_2d_allocate(TotalAnnualCosts, NumberSims, NumberYears);
		general_2d_allocate(PermRightCosts, NumberSims, NumberYears);
		general_1d_allocate(AverageAnnualCosts, NumberYears);
		general_1d_allocate(StdDevInput, NumberSims);
		general_1d_allocate(StdDevCost, NumberYears);
		general_1d_allocate(cost_sorting_array, NumberSims);
		general_2d_allocate(SortedTotalAnnualCosts, NumberSims, NumberYears);
		general_1d_allocate(VAR, NumberYears);
		general_1d_allocate(CVAR, NumberYears);
		general_1d_allocate(VARmean, NumberYears);
		general_2d_allocate(YearlyDroppedTransfers,NumberSims,NumberYears);
		general_1d_allocate(DroppedTransfersVector, NumberYears);
		general_2d_allocate(YearlyNumberTransfers, NumberSims, NumberYears);
		general_1d_allocate(NumberTransfersVector, NumberYears);

		/* End declarations, final calculations variables */
	}
	global_trackers_allocation(initial_call); //allocate global stuff

	/* Begin growing future values */

	//Grow future rights ...
	if (single_flag) //now a local flag
	{
		//DEBUG 10-01
		//cout << "Single flag was tripped. Growing demand to year ten for the single year." << endl;
		//END DEBUG
		//We only care about Nrt from the strategy, for year ten
		//annual_tracker.future_Nrt[0] = strategy.Nrt[9];
		//likewise, since the single-year is year 10, the city treats this year like year ten...
		for (int month_it = 0; month_it < 12; month_it++)
		{
			futures[month_it].demand_mean[0] = demand_sets[month_it].mean*pow((1+DemGrowthFactor),9.0);
		}
	}
	else
	{
		//for (int year_it = 0; year_it < NumberYears; year_it++)
		//{
			//annual_tracker.future_Nrt[year_it] = strategy.Nrt[year_it];
		//}
		
		//Grow future demand ...
		//DEBUG 10-01
		//cout << "Growing demand over the ten years." << endl;
		//END DEBUG

		for (int year_it = 0; year_it < NumberYears; year_it++) //loop over years
		{
			for (int month_it = 0; month_it < 12; month_it++) //loop over months
			{
				//We're initializing the variable here.
				futures[month_it].demand_mean[year_it] = demand_sets[month_it].mean*pow((1+DemGrowthFactor),(double)(year_it));
				//note: the power in that equation was written as year-1, but since our
				//indices begin at 0, this turns into the index k

				/* later, add a calculation of future standard deviation here.  I think they had
				intended originally to grow the future mean and then sample from a translated normal
				distribution in the future... since you run out of historical data when the demand has
				increased.  this hasn't been implemented in Matlab yet, and we'll see if we have to do it
				at this step */
			}
		}
	}

	//expected monthly allocations
	
	//NOTE this should be okay in the single-year scenario since the year_it will only be calculated
	//"at" index zero

	for (int year_it = 0; year_it < NumberYears; year_it++)
	{
		for (int month_it = 0; month_it < 12; month_it++)
		{
			avg_hist_NW = average_array(hydro_sets[month_it].NW,33);
			//to calculate ENr, we use the average NW value across the
			//entire historical record for a given month

			//this is the only place historical NW is used.
			//in other places of the code, NW = Inflows - Losses

			//the EXPECTED allocation is calculated like this:
			//you create a ratio of how many rights you own (Nrt) to the
			//total number of rights for the whole region.  That's the percentage
			//of the new water that you can use.  You take that ratio, and multiply
			//it with a loss coefficient, and you multiply that factor with the
			//amount of new water that you EXPECT will be coming.
			//(the same way the actual allocation is calculated, but this is used
			//for anticipatory purposes in calculating strategies, etc.)

			futures[month_it].ENr[year_it] = avg_hist_NW*(1-in_loss)*strategy.Nrt/TWR;
		}
	}

	/* End Growing Future Values */

	if (params.timing_flag)
	{
		timers.before_loop_end = clock();
		timers.before_loop_sum = timers.before_loop_end - timers.before_loop_start;
	}

	//cout << "initial calls before main loop = " << end << endl;

	//Then begin main loop...

	for (int n = 0; n < NumberSims; n++) //ITERATION N-LOOP (indexed by n)
	{
		
		//declare the "proper" iterators
		CurrentYear = 0;  //Every n-iteration starts at year 0
		CurrentMonth = 0; //Likewise, every n-iteration starts with month 0
		CurrentSim = n; //assign a more proper iterator
		
		//re-initialize your local lists
		zeroes(LeaseCost, NumberYears, 12);
		zeroes(Failures, NumberYears, 12);
		zeroes(CFailures, NumberYears, 12);
		zeroes(NumberPurchasedLeases, NumberYears, 12);
		zeroes(NxTracker, NumberYears);
		zeroes(PoList, NumberYears);
		zeroes(TransferTracker, 13);
		zeroes(AnnualNewWateri, 12);

		//re-initialize your local placeholders
		DroppedTransfers = 0.0;

		if (CurrentSim == 0)
		{
			timers.monte_carlo_sum = 0.0;
			timers.calcs_sum = 0.0;
		}

		if (params.timing_flag) timers.monte_carlo_start = clock();

		/* Random Sampling Routine */
		if (params.sync_flag || single_flag) //single_flag is local now
		{
			for (int year_it = 0; year_it < NumberYears; year_it++)
			{
				for (int month_it = 0; month_it < 12; month_it++)
				{
					samples[month_it].demand[year_it] = super.y[CurrentSim].samples[month_it].demand[year_it];
					samples[month_it].inf[year_it] = super.y[CurrentSim].samples[month_it].inf[year_it];
					samples[month_it].lease_high_res[year_it] = super.y[CurrentSim].samples[month_it].lease_high_res[year_it];
					samples[month_it].lease_low_res[year_it] = super.y[CurrentSim].samples[month_it].lease_low_res[year_it];
					samples[month_it].los[year_it] = super.y[CurrentSim].samples[month_it].los[year_it];
					//samples[month_it].NW[year_it] = super.y[CurrentSim].samples[month_it].NW[year_it];
					samples[month_it].res_var[year_it] = super.y[CurrentSim].samples[month_it].res_var[year_it];
				}
			}
		}
		else
		{
			//takes _sets structures and populates the global samples structure
			//new sample every time!
			for (int month_it = 0; month_it<12; month_it++)
			{
				randsample(hydro_sets[month_it], samples[month_it], NumberYears);
				randsample(lease_sets[month_it], samples[month_it], NumberYears);
				randsample(demand_sets[month_it], samples[month_it], NumberYears, DemGrowthFactor);
				//We have: 12 samples structures, one for each month
				//with enough randomly-sampled values to run
				//one n-iteration (monte carlo iteration).
			}
		}


		if (params.rng_flag)
		{
			if (params.sync_flag)
			{
				if(sampler_count == 1)
				{
					//only output the first set of draws when sync_flag is turned on
					//(they should all be the same for each solution)
					sets_output_rng_text(rng_stream, CurrentSim);
				}
			}
			else
			{
				if (params.single_flag)
				{
					if (sampler_count == 1 && CurrentSim == 0)
						sets_output_rng_text(rng_stream, CurrentSim);
				}
				else
				{
					//output the random values every time
					sets_output_rng_text(rng_stream, CurrentSim);
		
				}
			}
			
		}

		/* End Random Sampling Routine */
		
		if (params.timing_flag) //start timing on calculations
		{
			timers.monte_carlo_end = clock();
			timers.monte_carlo_sum += timers.monte_carlo_end - timers.monte_carlo_start;
			timers.calcs_start = clock();
		}
		//populate early and late demand
		for (int year_it = 0; year_it<NumberYears; year_it++)
		{
			sims_years_tracker.early_demand[CurrentSim][year_it] = 0.0;
			for (int month_it = 0; month_it<5; month_it++)
			{
				sims_years_tracker.early_demand[CurrentSim][year_it] += samples[month_it].demand[year_it];
				//samples[month].demand is the same as DemandList in matlab code
			}
			sims_years_tracker.late_demand[CurrentSim][year_it] = 0.0;
			for (int month_it = 5; month_it<12; month_it++)
			{
				sims_years_tracker.late_demand[CurrentSim][year_it] += samples[month_it].demand[year_it];
			}
		}

		//Initially set reservoir tracking variables...

		Ro = iRo; //iRo is a user-set parameter .. the only "original" variable in this chain
		//CurrentResLevel and OldReservoirLevel are used for bookkeeping later
		//but they're all equal right now.
		CurrentResLevel = Ro;
		OldReservoirLevel = CurrentResLevel;
		Ro_diff = reservoir_threshold - Ro; //closeness to threshold
		
		//Now, set Nro (the initial water in your account) to the initial condition
		//based on Nrt.  Later, Nro is used for bookkeeping purposes and stores
		//left-over water throughout the years.

		if (calc_param == "drought_noinit")
		{
			//add in the reading of a parameter here

			//assert(strategy.Nrt);
			
			//hardcode this for the time being...
			if (params.ifri_dist == "constant")
			{
				ifri = params.ifri_param1;
			}
			else
			{
				ifri = 0.4118;
			}
			

			//params.ifri = ifri;  //don't think this is needed

			//ifri = (obj[1]*100000.0)/strategy.Nrt;
			//params.ifri = ifri; //recording what the calculated initial rights were for output file
		}
		else
		{
			//Previous code to handle ifri sampling:
			//if (!params.ifri_high)	ifri = params.ifri; //handles situation where ifri is static
			//else
			//{
			//	//otherwise,the ifri is sampled
			//	ifri = rndreal(params.ifri_low, params.ifri_high);
			//	//params.ifri = ifri; //recording what the sampled initial rights were for output file
			//}
			//end Previous Code

			if(params.ifri_dist == "constant")
			{
				ifri = params.ifri_param1;
			}
			else if(params.ifri_dist == "uniform")
			{
				ifri = rndreal(params.ifri_param1, params.ifri_param2);
			}
			else if(params.ifri_dist == "normal")
			{
				int random_draw;
				random_draw = rnd(0,49999);
				ifri = params.initrights_vec[random_draw]; //the input file has a prescribed mean and std
				//DEBUG 10-01
				//cout << "ifri = " << ifri << endl;
				//END DEBUG
			}
			else
			{
				cout << "Initial rights dist. is invalid!" << endl;
				exit(1);
			}
		}
		fri = ifri; //fraction of rights is set to the initial fraction of rights
		Nro = fri*Nrt;
		To = Nro; //the total water, at the beginning
		if (ifri > strategy.xi)
		{
			No = strategy.No_low;
		}
		else No = strategy.No_high;
		//assign water allocation to list...
		NewMonthAllocationList.clear(); //ensure that the vector is empty (at the beginning of the n-loop)
		NewMonthAllocationList.push_back(Nro); //this should be the first entry (later, the first entry will be reassigned)
		
		//record the initial lease price
		PoList[CurrentYear] = Po;
		
		//The following loop asks the question: Do we expect that we will be allocated more water
		//in the first four months of the year (ENr), than we are allowed to take (Nrt - Nro, the amount of water 
		//left from our rights)?  If so, then we expect that we will be allocated the full amount of what
		//we are allowed.  Otherwise, we will only expect to receive what we expect
		//to be allocated for the first three months.
		if ( futures[0].ENr[CurrentYear]+futures[1].ENr[CurrentYear]+futures[2].ENr[CurrentYear]+futures[3].ENr[CurrentYear] > (Nrt-Nro))
		{
			ENrt = Nrt-Nro;
		}
		else
		{
			ENrt = futures[0].ENr[CurrentYear]+futures[1].ENr[CurrentYear]+futures[2].ENr[CurrentYear]+futures[3].ENr[CurrentYear];
		}
	
		Te1 = To + ENrt;

		//set lease price based on reservoir storage
		//The initial lease price is based on a december lease price, since the beginning
		//of the model is from a "previous" December (which is artificial and part of the initial
		//condition to begin our model)
		if (CurrentResLevel < reservoir_threshold) P_lease_o = samples[11].lease_low_res[CurrentYear];
		else P_lease_o = samples[11].lease_high_res[CurrentYear];

		sims_years_tracker.ToLeasePrice[CurrentSim][CurrentYear] = P_lease_o; //record lease price
		
		d1 = 0;
		for (int month_it=0; month_it<5; month_it++) d1 += futures[month_it].demand_mean[CurrentYear]; //d1 must be early demand

		//note that d1 is calculated based on AVERAGE demand and not demand
		//which is sampled from the distributions

		N1o = 0; // "N-one-oh", leases purchased in the beginning of the year
		
		//first reference to alpha
		//alpha and beta are early year strategy variables
		if (lease_flag)
		{
			if (Te1/d1 <= alpha) //check if water is needed
			{

				if (beta*d1 - Te1 > 0) //then, how much?
				{
					N1o = rounding(beta*d1 - Te1); //10-1-08 fix (rounding leases to be integers)
				}
				else
				{
					N1o = 0;
				}
				//in the above line, we've still tripped the alpha "chicken" factor, but beta
				//is set such that we have enough water to get by, so we don't lease.

				//note that here, the beta loop is inside the alpha loop, while later
				//the loops are separate
				
				//Here, CurrentYear == CurrentMonth ==0
				//Lease Cost Assignment 1: before simulation begins (initial acquisition)
				LeaseCost[CurrentYear][0] += N1o*P_lease_o; //record cost
			}
			else
			{
				N1o = 0;
			}
		}
		else N1o = 0; //for the lease_flag == 0

		//Add the leased water into the TransferTracker
		TransferTracker[0] = N1o;
		NumberPurchasedLeases[CurrentYear][CurrentMonth] = N1o; //add leases to list

		T1 = N1o + Nro;
		
		//we are in the iteration n-loop so it's okay to clear this
		AvWaterList.clear(); // make sure the vector is cleared out
		AvWaterList.push_back(T1); //add water at beginning of year to the AvWaterList	

		for (int outer_year_it = 0; outer_year_it < NumberYears; outer_year_it++) //ITERATION YEAR-LOOP (indexed by outer_year_it)
		{
			CurrentYear = outer_year_it; //Use CurrentYear as a more proper iterator
					
			if (CurrentYear == 0)
			{
				if (ifri > strategy.xi) //changed to local ifri instead of from the params structure
				{
					No = strategy.No_low;
				}
				else No = strategy.No_high;
			}
			else if (CurrentYear > 0) //We calculated No for the first year based on ifri (initial condition)
			{
				//We will calculate the fro as the AvWater divided by permanent rights
				if ((AvWaterList[0] / Nrt) > strategy.xi)
				{
					No = strategy.No_low;
				}
				else No = strategy.No_high;
			}

			sims_years_tracker.No[CurrentSim][CurrentYear] = No;

			zeroes(AnnualSupplyi, 12);

			//Calculate allocations / expected demand
			//(we've already done this calculation, so now we
			//just put it in a local list)
			//ExpAlloRestYear is a 10-member list
			//ExpYearDemand is an 11-member list
			
			for (int allo_it = 0; allo_it < 10; allo_it++)
			{
				ExpAlloRestYear[allo_it] = 0; //initialize the given list member

				//then populate it...

				//in January: we sum up the demand for February up to and including November
				//in February: we sum up the demand for March up to and including November
				//it stops in November, at the index 10, where we only take the 10th value

				for (int month_it = allo_it+1; month_it < 11; month_it++) //Calculating over the rest of the year (stops at Nov.)
				{
					ExpAlloRestYear[allo_it] += futures[month_it].ENr[CurrentYear];
				}

			}//allo_it for-loop
		
			for (int demand_it = 0; demand_it < 11; demand_it++)
			{
				ExpYearDemand[demand_it] = 0; //initialize value

				for (int month_it = demand_it+1; month_it < 12; month_it++) //Calculating over the rest of year (stops at Dec.)
				{
					ExpYearDemand[demand_it] += futures[month_it].demand_mean[CurrentYear];
				}
			}//demand_it for-loop

			Nx = 0; //exercised options	

			for (int outer_month_it = 0; outer_month_it < 12; outer_month_it++) //ITERATION MONTH LOOP (indexed by outer_month_it)
			{
				CurrentMonth = outer_month_it;
							
				//set some placeholders to zero ...

				//Show that there is no use if the CurrentMonth is zero...
				if (CurrentMonth == 0) zeroes(MonthlyWaterUsageList, 12);
				N1 = 0; // lease acquisitions
				NextMonthAvWater = 0; //added to this translation (seems more clear)
				//NextMonthAvWater is used when leases and options are calculated
				//in a given month.  It's then assigned to the AvWaterList, so that
				//next month we can put it into our AvWater like this...
				
				//assign placeholders from global structures ...

				AvWater = AvWaterList[CurrentMonth]; //the total water we have
				monthly_tracker[CurrentMonth].av_water[CurrentSim][CurrentYear] = AvWater;
				//assign AvWater to a new global list ...

				
				//Assign the samples to a scalar variable.
				Inflows = samples[CurrentMonth].inf[CurrentYear];
				Demand = samples[CurrentMonth].demand[CurrentYear];
				monthly_tracker[CurrentMonth].random_monthly_demand[CurrentSim][CurrentYear] = Demand;
				Losses = samples[CurrentMonth].los[CurrentYear];
				ResVariation = samples[CurrentMonth].res_var[CurrentYear];
				
				//"If monthly demand exceeds the available water, register a failure for that month"
				if (AvWater >= Demand)
				{
					use = Demand;
				}

				else
				{
					Failures[CurrentYear][CurrentMonth] = 1; //changed this from a += construction
					use = AvWater;
					
					//now check critical reliability...
					if (AvWater/Demand < critical_reliability_threshold) // now moved inside the failures loop
					{
						CFailures[CurrentYear][CurrentMonth] = 1; //changed this from a += construction
					}
				}
				
				transfer_tracker(TransferTracker, AvWater, Demand);

				//If you didn't use the oldest transfer in the list (the one at the 13th position, index 12)
				//you have to get rid of it. (and make a note that it was dropped, since a portfolio that
				//had a lot of dropped transfers would waste money)
				
				temp_transfer_sum = sum(TransferTracker,13);

				DroppedTransfers = TransferTracker[12]; //local list
				monthly_tracker[CurrentMonth].dropped_transfer_tracker[CurrentSim][CurrentYear] = DroppedTransfers; //main list

				/* End Transfer Section */

				MonthlyWaterUsageList[CurrentMonth] = use; //local list
				
				//reservoir variation ...
				if ((OldReservoirLevel+ResVariation)<0) CurrentResLevel = 0;
				else CurrentResLevel = OldReservoirLevel + ResVariation;
		
				ReservoirList[CurrentMonth] = CurrentResLevel; //local list
				monthly_tracker[CurrentMonth].res_level_tracker[CurrentSim][CurrentYear] = CurrentResLevel; //global list
				if (CurrentResLevel < reservoir_threshold)
				{
					monthly_tracker[CurrentMonth].res_threshold_tracker[CurrentSim][CurrentYear] = 1;
				}
				else
				{
					monthly_tracker[CurrentMonth].res_threshold_tracker[CurrentSim][CurrentYear] = 0;
				}
				//calculate new water
				if ((Inflows - Losses)>0) NewWater = Inflows - Losses;
				else NewWater = 0;

				//"Calculate Monthly allocation based on inflow - No
				//allocation if number of permanent rights have already
				//been allocated or if the reservoir level is
				//below its 'lower limit' "

				//allocate water ...

				if (CurrentResLevel >= ReservoirCriticalLevel)
				{

					if ((NewWater*(1-in_loss)*Nrt/TWR)<=(Nrt-sum(NewMonthAllocationList,int(NewMonthAllocationList.size()))))
					{
						Nr = NewWater*(1-in_loss)*Nrt / TWR;
					}
					else
					{
						Nr = Nrt - sum(NewMonthAllocationList,int(NewMonthAllocationList.size()));
					}
					
				}//if(CurrentResLevel>=ReservoirCriticalLevel
				else 
				{
					Nr = 0;
				}

				//Nr was negative with a low value for Nrt.  So we put this in:
				if (Nr < 0) Nr = 0; //Nr should never be negative though.
							
				//"Register monthly allocation"
				//NewMonthAllocationList for the first month is initially set as
				//Nro -- water allocated at the beginning of the year.  Then, after
				//month 0 is over, the water is allocated based on inflow and the vector
				//member is reassigned to Nr. Since NewMonthAllocationList is already
				//a one-member vector for Month 0, we just assign the value.  Otherwise,
				//we'll have to grow the vector as we assign it (therefore we put an
				//if-else statement here.
				if(CurrentMonth == 0) NewMonthAllocationList[CurrentMonth] = Nr;
				else NewMonthAllocationList.push_back(Nr);

				monthly_tracker[CurrentMonth].Nr[CurrentSim][CurrentYear] = Nr;

				AnnualSupplyi[CurrentMonth] = Nr;
				AnnualNewWateri[CurrentMonth] = NewWater;

				/* Market Specific Section */
				/* "If the expected available water is less than
				A% of the demand, then water should be leased and/or
				optionied (enough to meet B% of the unmet demand)
				depending on the month in question, the option exercise
				price and lease prices -- No allocation is considered in
				November" */
				
				//The dropped transfers have to be taken out somewhere.
				//they are taken out of the Nro, a "bookkeeping" variable for
				//the beginning of the year. 
				
				//I'm cautious though about what happens when this number is
				//less than zero.

				Nro = Nro - DroppedTransfers;

				//calculate available water for next month (so far)

				if (CurrentMonth <= OExerciseMonth)
				{
					//the data we want in NumberPurchasedLeases is in the CurrentYear_th row.
					NextMonthAvWater = Nro + sum(NewMonthAllocationList, (int) NewMonthAllocationList.size()) + sum(NumberPurchasedLeases[CurrentYear],12) - sum(MonthlyWaterUsageList,12);
					//NextMonthAvWater was calculated above to subtract out the droppedtransfers in Nro.			
				}
				else
				{
					//after they have the choice of exercising options, these
					//exercised options are included in this calculation
					NextMonthAvWater = Nro + sum(NewMonthAllocationList, (int) NewMonthAllocationList.size()) + sum(NumberPurchasedLeases[CurrentYear],12) - sum(MonthlyWaterUsageList,12) + Nx;
				}

				//note: NextMonthAvWater is assigned before the city decides to buy water to meet their needs next month

				//calculate expected available water for next month (used for acquiring more water)
				
				//if it's before the Exercise Month...
				if (CurrentMonth <= (OExerciseMonth-1)) //IS THIS RIGHT
				{
					//in the months before the exercise month, assume that we'll be exercising
					//all our options.
					NextMonthExpAvWater = NextMonthAvWater + ExpAlloRestYear[CurrentMonth] + No;
				}
				//otherwise...
				else
				{
					//if it's before the index 10 - month
					if (CurrentMonth < 10)
					{
						NextMonthExpAvWater = NextMonthAvWater + ExpAlloRestYear[CurrentMonth];
						//Therefore, the last month to have a "NextMonthExpAvWater" is index 9,
						//which is October.
					}
				}
				
				/////
				//If it's before the exercise month...
				if (CurrentMonth < OExerciseMonth)
				{
					//calculate NextMonthAvWater (actual water we have) and only the amount
					//we are expected to be allocated UNTIL the exercise month...
					RestSpringExpAvWater = NextMonthExpAvWater - ExpAlloRestYear[OExerciseMonth] - No;
					
					if(lease_flag)
					{
						//second reference to alpha
						//if you need water ...
						if ((RestSpringExpAvWater/(ExpYearDemand[CurrentMonth]-ExpYearDemand[OExerciseMonth]))<=alpha)
						{
							//only set your lease price if you're gonna need it.
							//so we've tripped the alpha factor then we...

							//set lease price
							if (CurrentResLevel < reservoir_threshold)
							{
								P_lease = samples[CurrentMonth].lease_low_res[CurrentYear];
							}
							else P_lease = samples[CurrentMonth].lease_high_res[CurrentYear];
							//end first if-else block
							
							//then how much?
							if ( ((beta*(ExpYearDemand[CurrentMonth]-ExpYearDemand[OExerciseMonth]))-RestSpringExpAvWater ) > 0)
							{
								//we place lease acquisitions in the "N-one" variable...
								N1 = rounding(beta*(ExpYearDemand[CurrentMonth]-ExpYearDemand[OExerciseMonth]) - RestSpringExpAvWater); //10-1-08, round leases
								//then add those leases to the water we have.
								NextMonthAvWater = NextMonthAvWater + N1; //this is "N-one"
								TransferTracker[0] = N1;
							}
							else 
							{
								N1 = 0;
							}
							//end second if-else block
						
							//in the following assignment, I'm under the impression that
							//LeaseCost[CurrentYear][CurrentMonth+1] should always be zero at
							//this point.

							//but anyway, the following assignment populates the next value in the
							//leasecost list, based on the price we just assigned (based on reservoir level)
							//and the leases that we just decided to buy (based on the value of beta)
							
							//if we don't buy anything, everything's just zero
							
							//Lease Cost Assignment 2: Before ExerciseMonth (during month loop)
							LeaseCost[CurrentYear][CurrentMonth+1] += N1*P_lease;
							
						} //if(RestSpringExpAvWater /(ExpYearDemand...
						else
						{
							N1 = 0;
						}
					}
					else N1 = 0; //for the lease_flag == 0
				} //if (CurrentMonth<OExerciseMonth)

				//the following loop implies that your month is both at the exercise month
				//and less than december (so november, index 10, is the last month that is
				//eligible).
				
				/////
				//or, if it's after the exercise month, but before the end of the year...
				if (CurrentMonth >= OExerciseMonth && CurrentMonth < 11)
				{
					//assign lease price...
					if (CurrentResLevel < reservoir_threshold)	P_lease = samples[CurrentMonth].lease_low_res[CurrentYear];
					else P_lease = samples[CurrentMonth].lease_high_res[CurrentYear];
		
					if (CurrentMonth == OExerciseMonth)	sims_years_tracker.Pl5[CurrentSim][CurrentYear] = P_lease;

					if ((NextMonthExpAvWater/ExpYearDemand[CurrentMonth])<= alpha2)
					{
						if (CurrentMonth == OExerciseMonth)
						{
							if (lease_flag && options_flag && Px < P_lease) //you are allowed to buy leases, and you are exercising
							{
								if ((beta2*ExpYearDemand[CurrentMonth] - NextMonthExpAvWater)<= No)
								{
									if ((beta2*ExpYearDemand[CurrentMonth] - NextMonthExpAvWater)>0)
									{
										Nx = rounding(beta2*ExpYearDemand[CurrentMonth] - NextMonthExpAvWater); //10-1-08 fix, rounding exercised options
										NextMonthAvWater += Nx;
									}
									else Nx = 0;
									N1 = 0; //added
								}
								else
								{
									Nx = No;
									NextMonthAvWater += Nx;
									//"Next few lines (3) are being tried out"
									NextMonthExpAvWater = NextMonthAvWater + ExpAlloRestYear[CurrentMonth];
									if ((beta2*ExpYearDemand[CurrentMonth] - NextMonthExpAvWater)>0) //changed to beta2, 9-15-2011
									{
										N1 = rounding(beta2*ExpYearDemand[CurrentMonth] - NextMonthExpAvWater);//changed to beta2, 9-15-2011
										//two commented-out lines in Matlab are omitted here
										NextMonthAvWater += N1;
									}
									else N1 = 0;
									//Lease Cost Assignment 3: During Exercise Month for "filling out"
									//your options with extra leases
									LeaseCost[CurrentYear][CurrentMonth+1] += N1*P_lease; //changed from a += construction							
								}
								TransferTracker[0] = N1 + Nx;//new!
							}//end if (Px<P_lease)
							else if (lease_flag && options_flag) //you are allowed to buy leases, and you will only be leasing
							{
								if ((beta2*ExpYearDemand[CurrentMonth] - NextMonthExpAvWater) > 0)
								{
									N1 = beta2*ExpYearDemand[CurrentMonth] - NextMonthExpAvWater;
									NextMonthAvWater += N1;
								}
								else
								{
									N1 = 0;
								}
								//Lease Cost Assignment 4: for an unfavorable strike price,
								//you buy all leases in the exercise month
								LeaseCost[CurrentYear][CurrentMonth+1] += N1*P_lease;
								TransferTracker[0] = N1; //new!
							}
							else if (options_flag && !lease_flag) //you are NOT allowed to buy leases and you will just be exercising if you need to
							{
								//if your options are enough (or more than enough) to meet your beta-need,
								if ((beta2*ExpYearDemand[CurrentMonth] - NextMonthExpAvWater)<= No)
								{
									//if beta says you need water,
									if ((beta2*ExpYearDemand[CurrentMonth] - NextMonthExpAvWater)>0)
									{
										//exercise what you need
										Nx = rounding(beta2*ExpYearDemand[CurrentMonth] - NextMonthExpAvWater); //10-1-08 rounding exercised options
										//and add it to your NextMonthAvWater
										NextMonthAvWater += Nx;
									}
									//otherwise, beta says you don't need water and you don't exercise anything
									else Nx = 0;
									//end if-else block
								}
								//otherwise, you don't have enough options to meet your need.
								else
								{
									//but, you'll exercise all you can...
									Nx = No;
									NextMonthAvWater += Nx;				
								}

								//record your exercised options in the tracker
								TransferTracker[0] = Nx;

							} //end, you are NOT allowed to buy leases and you'll just be exercising.
							
							NxTracker[CurrentYear] = Nx; //local list for exercised options
							
							//else if (lease_flag && !options_flag)
							//{
							//	//if beta says we need water,
							//	if ((beta2*ExpYearDemand[CurrentMonth] - NextMonthExpAvWater) > 0)
							//	{
							//		N1 = beta2*ExpYearDemand[CurrentMonth] - NextMonthExpAvWater;
							//		NextMonthAvWater += N1;
							//	}
							//	else
							//	{
							//		N1 = 0;
							//	}								
							//	//Lease Cost Assignment 7: no options allowed, in exercise month
							//	LeaseCost[CurrentYear][CurrentMonth+1] += N1*P_lease; //local list
							//	TransferTracker[0] = N1; //tracker
							//}
							//The above if-else loop contained four conditions: if, if else, if else, else
						}//end if(CurrentMonth == OExerciseMonth)
						else if (lease_flag) //you can buy leases
						{
							//if beta says we need water,
							if ((beta2*ExpYearDemand[CurrentMonth] - NextMonthExpAvWater) > 0)
							{
								N1 = rounding(beta2*ExpYearDemand[CurrentMonth] - NextMonthExpAvWater);//10-1-08 fix - rounding leases
								NextMonthAvWater += N1;
							}
							else
							{
								N1 = 0;
							}
							//end if-else block (beta says we need water)
							
							//Lease Cost Assignment 5: It is after the exercise month
							//and we leased some water
							LeaseCost[CurrentYear][CurrentMonth+1] += N1*P_lease; //local list
							TransferTracker[0] = N1; //tracker
						} //end -- It's not the exercise month, and you are allowed to buy leases
						else //you are NOT allowed to buy leases
						{
							N1 = 0;
						}
						//end if-else block (whether it is the exercise month or not)

					} //end if(NextMonthExpAvWater/ExpYearDemand[CurrentMonth]<=alpha)

				}//end if(CurrentMonth>=OExerciseMonth && CurrentMonth < 11)

				//now we're done with leases and it's time to bookkeep before next month.
				if (CurrentMonth < 11)
				{
					//Add NextMonthAvWater to AvWaterList. (the scalar
					//will be assigned at the beginning of the next month.)
					AvWaterList.push_back(NextMonthAvWater);
					NumberPurchasedLeases[CurrentYear][CurrentMonth+1] = N1;

					//that's all we need to do before the end of the month,
					//so go back up to the beginning of the month loop.
				}
				
				/////
				//it is now the end of the year and it's time to bookkeep before next year.
				if (CurrentMonth == 11)
				{
					//ADDED: Calculate this even for the last year!
					//track how much water is left at the end of the year
					sims_years_tracker.end_of_yr_water_tracker[CurrentSim][CurrentYear] = AvWater - DroppedTransfers;


					//if it's any year but the very last year...
					if (CurrentYear != NumberYears-1)
					{
						//Do some initializiation for next year. Before the first
						//year, these calculations are done at the start of the
						//iteration n-loop. After that, they are done at "the end,"
						//which is here.

						//set the reservoir difference.
						Ro_diff = reservoir_threshold - CurrentResLevel;
						
						//Note: some lines implementing a more complicated pricing scheme
						//are in the Matlab code here.
						//the idea is that there are about 10 or so options prices
						//which are more precisely based on the reservoir height,
						//as calculated above.
						
						//assign the options price.
						PoList[CurrentYear+1] = Po;
						
						//take care of initial allocations for next year.  The city
						//starts off with whatever it had at the end of the previous
						//year.
						To = NextMonthAvWater;
						Nro = NextMonthAvWater;
						
						//clear out the allocations list to get ready for
						//next year.
						NewMonthAllocationList.clear();
						NewMonthAllocationList.push_back(Nro);
					
						//get ready to expect some water for next year.

						double ENr_temp=0;

						//again, see how much water is expected for the total
						//allocation. (be careful with the Nro being too high)
						
						for (int month_it = 0; month_it<4; month_it++)
						{
							//ENr are values based on historical
							//NW lists.
							ENr_temp += futures[month_it].ENr[CurrentYear+1];
						}

						if (ENr_temp > (Nrt-Nro))
						{
							ENrt = Nrt - Nro;
						}
						else ENrt = ENr_temp;
						//end if-else block

						if (ENrt < 0) ENrt = 0; //this non-negativity restriction was added in the C++ translation

						//acquire some leases for next year...

						//expected water includes water we have right now
						//and the expected allocation for next year.
						Te1 = To + ENrt;

						//set the lease price
						if (CurrentResLevel <= reservoir_threshold)
						{
							P_lease_o = samples[11].lease_low_res[CurrentYear+1];
						}
						else P_lease_o = samples[11].lease_high_res[CurrentYear+1];
						//end if-else block
						
						sims_years_tracker.ToLeasePrice[CurrentSim][CurrentYear+1] = P_lease_o;

						d1 = 0; //formulation as before
						for (int month_it=0; month_it<5; month_it++) d1 += futures[month_it].demand_mean[CurrentYear+1];
						
						if (lease_flag)
						{
							//fourth reference to alpha
							if (Te1/d1 <= alpha)
							{
								if ((beta*d1 - Te1) > 0)
								{
									N1o = rounding(beta*d1 - Te1);//10-1-08 round leases
								}
								else N1o = 0;
								//end if-else block
								
								//Lease Cost Assignment 6: It's the end of the
								//year and we're getting ready for a new simulation
								//year
								LeaseCost[CurrentYear+1][0] += N1o*P_lease_o;
							}
							else N1o = 0;
						}
						else N1o = 0; //for lease_flag == 0
						//end if-else block
						
						//register your initial transfer in the tracker.
						//the tracker should keep the water throughout one
						//full year, no matter which calendar year you're in.
						TransferTracker[0] = N1o;

						T1 = N1o + Nro;

						//Because AvWaterList is indexed by month and should
						//therefore only be valid within a certain calendar year,
						//we will clear it.  This is the only way to be safe from
						//using a C++ vector.
						AvWaterList.clear();
						AvWaterList.push_back(T1);
					
						NumberPurchasedLeases[CurrentYear+1][0] = N1o;

					}//end if(CurrentYear!=NumberYears)

					OldReservoirLevel = CurrentResLevel; //is this in the right scope?

				}//end if(CurrentMonth==11)

			} //ITERATION MONTH LOOP (indexed by outer_month_it) ends

			sims_years_tracker.early_NW[CurrentSim][CurrentYear] = 0;
			for (int month_it = 0; month_it<5; month_it++)
			{
				sims_years_tracker.early_NW[CurrentSim][CurrentYear] += AnnualSupplyi[month_it];
			}
			
			sims_years_tracker.late_NW[CurrentSim][CurrentYear] = 0;
			for (int month_it = 5; month_it < 12; month_it++)
			{
				sims_years_tracker.late_NW[CurrentSim][CurrentYear] += AnnualSupplyi[month_it];
			}

		} //ITERATION YEAR LOOP (indexed by outer_year_it) ends



		//begin page 22 of matlab code
	
		//populate output lists within n-loop
		//that is, these outputs are stored at every iteration

		//monthly lists (stores at every month, year, and iteration)
		//also includes yearly lists (stores at every year and iteration)

		for (int year_it = 0; year_it<NumberYears; year_it++)
		{
			sims_years_tracker.total_Nx_tracker[CurrentSim][year_it] = NxTracker[year_it];
			sims_years_tracker.total_Po_list[CurrentSim][year_it] = PoList[year_it];

			for (int month_it = 0; month_it<12; month_it++)
			{
				monthly_tracker[month_it].yearly_lease_cost[CurrentSim][year_it] = LeaseCost[year_it][month_it];
				monthly_tracker[month_it].total_monthly_leases[CurrentSim][year_it] = NumberPurchasedLeases[year_it][month_it];
				monthly_tracker[month_it].total_failures[CurrentSim][year_it] = Failures[year_it][month_it];
				monthly_tracker[month_it].total_cfailures[CurrentSim][year_it] = CFailures[year_it][month_it];
				
			}//close month_it loop

		}//close year_it loop
		if (params.timing_flag)
		{		
			timers.calcs_end = clock();
			timers.calcs_sum += timers.calcs_end - timers.calcs_start;
		}


	}//ITERATION N-LOOP (indexed by n) ends

	//
	//
	//BEGIN METRIC CALCULATIONS
	//
	//
	
	if(calc_param == "ten-year" || calc_param == "drought_full")
	{
		if (params.timing_flag) timers.obj_start = clock();
		
		//
		//calculate failures
		//

		for (int year_it = 0; year_it<NumberYears; year_it++)
		{
			for (int month_it = 0; month_it<12; month_it++)
			{
				temp_sum_f = 0.0; //for failures
				temp_sum_cf = 0.0; //for critical failures
				for (int sims_it = 0; sims_it < NumberSims; sims_it++)
				{
					temp_sum_f += monthly_tracker[month_it].total_failures[sims_it][year_it];
					temp_sum_cf += monthly_tracker[month_it].total_cfailures[sims_it][year_it];
				}
				temp_mean_f = temp_sum_f / NumberSims;
				temp_mean_cf = temp_sum_cf / NumberSims;
				//expectation over the MC draws for years and months
				FailureRecord[year_it][month_it] = temp_mean_f; 
				CFailureRecord[year_it][month_it] = temp_mean_cf;
			}
		}
		
		if(params.out_flag)
		{
			out_stream << "failure_record(years_by_months)" << endl;
			general_debug_output(out_stream, FailureRecord, NumberYears, 12);
		}
		
		for (int year_it = 0; year_it < NumberYears; year_it++)
		{
			temp_sum_f = 0.0;
			temp_sum_cf = 0.0;
			//now take the months/years expectation and sum it for a yearly reliability
			for (int month_it = 0; month_it < 12; month_it++)
			{
				temp_sum_f += FailureRecord[year_it][month_it];
				temp_sum_cf += CFailureRecord[year_it][month_it];
			}		
			Reliability[year_it] = 1 - temp_sum_f/12;
			CReliability[year_it] = 1 - temp_sum_cf/12;
		}

		mean_Reliability = average_array(Reliability, int(NumberYears));
		min_Reliability = min_array(Reliability, int(NumberYears));
		sum_CReliability = sum(CReliability, int(NumberYears));
		min_CReliability = min_array(CReliability, int(NumberYears));

		//
		// dropped transfers
		//
		
		for (int sims_it = 0; sims_it < NumberSims; sims_it++)
		{
			for (int year_it = 0; year_it < NumberYears; year_it++)
			{
				dropped_sum = 0.0;
				for (int month_it = 0; month_it < 12; month_it++)
				{
					dropped_sum += monthly_tracker[month_it].dropped_transfer_tracker[sims_it][year_it];
				}
				YearlyDroppedTransfers[sims_it][year_it] = dropped_sum;
			}
		}

		if (params.out_flag)
		{
			out_stream << "YearlyDroppedTransfers(sims_by_years)" << endl;
			general_debug_output(out_stream, YearlyDroppedTransfers, NumberSims, NumberYears);
		}

		for (int year_it = 0; year_it < NumberYears; year_it++)
		{
			DroppedTransfersVector[year_it] = average_array_colwise(YearlyDroppedTransfers, NumberSims, NumberYears, year_it);
		}

		TotalDroppedTransfers = sum(DroppedTransfersVector, NumberYears);

		//
		// number of transfers
		//

		number_transfers_sum = 0.0;
		yearly_lease_sum = 0.0;

		for (int sims_it = 0; sims_it < NumberSims; sims_it++)
		{
			for (int year_it = 0; year_it < NumberYears; year_it++)
			{
				number_transfers_sum = 0.0;
				yearly_lease_sum = 0.0;				
				for (int month_it = 0; month_it < 12; month_it++)
				{
					if (monthly_tracker[month_it].total_monthly_leases[sims_it][year_it] > 0)
					{
						//increment the counter if there was a lease regardless of volume...
						number_transfers_sum = number_transfers_sum + 1.0;			
					}
					//and sum the volume
					yearly_lease_sum = yearly_lease_sum + monthly_tracker[month_it].total_monthly_leases[sims_it][year_it];
				}
				sims_years_tracker.yearly_purchased_leases[sims_it][year_it] = yearly_lease_sum;
				YearlyNumberTransfers[sims_it][year_it] = number_transfers_sum;
			}
		}

		if (params.out_flag)
		{
			out_stream << "YearlyPurchasedLeases(sims_by_years)" << endl;
			general_debug_output(out_stream, sims_years_tracker.yearly_purchased_leases, NumberSims, NumberYears);
		}
		
		for (int year_it = 0; year_it < NumberYears; year_it++)
		{
			NumberTransfersVector[year_it] = average_array_colwise(YearlyNumberTransfers, NumberSims, NumberYears, year_it);
		}

		TotalNumberTransfers = sum(NumberTransfersVector, NumberYears);
		
		//
		// costs
		//

		for (int year_it = 0; year_it < NumberYears; year_it++)
		{
			for (int month_it = 0; month_it < 12; month_it++)
			{
				lease_cost_sum = 0.0;
				for (int sims_it = 0; sims_it < NumberSims; sims_it++)
				{
					//Here we're adding all the simulations up per year and month
					lease_cost_sum += monthly_tracker[month_it].yearly_lease_cost[sims_it][year_it];
				}
				AverageMonthlyLeaseCost[year_it][month_it] = lease_cost_sum / NumberSims;				
			}
			AverageYearlyLeaseCost[year_it] = sum(AverageMonthlyLeaseCost[year_it], 12);
		}

		for (int sims_it = 0; sims_it < NumberSims; sims_it++)
		{
			for (int year_it = 0; year_it < NumberYears; year_it++)
			{
				lease_cost_sum = 0.0;
				for (int month_it = 0; month_it < 12; month_it++)
				{
					//Here, we are taking a sum of all the months, per sims and year
					lease_cost_sum += monthly_tracker[month_it].yearly_lease_cost[sims_it][year_it];
				}
				TotalYearlyLeaseCost[sims_it][year_it] = lease_cost_sum;
			}
		}

		for (int sims_it = 0; sims_it < NumberSims; sims_it++)
		{
			for (int year_it = 0; year_it < NumberYears; year_it++)
			{
				//This is mainly calculated for some future work where the rights price would change.
				//Currently, this should be a sims by years matrix of the same value.
				PermRightCosts[sims_it][year_it] = Nrt*Pr; //added *Pr

				//Total Annual Costs = Permanent Rights costs + Options (Upfront) Costs +
				//Exercised Options Cost + LeaseCosts
				TotalAnnualCosts[sims_it][year_it] = 
					PermRightCosts[sims_it][year_it] + 
					sims_years_tracker.No[sims_it][year_it]*sims_years_tracker.total_Po_list[sims_it][year_it] + 
					sims_years_tracker.total_Nx_tracker[sims_it][year_it]*Px + 
					TotalYearlyLeaseCost[sims_it][year_it];
			}
		}

		for (int year_it = 0; year_it < NumberYears; year_it++)
		{
			AverageAnnualCosts[year_it] = average_array_colwise(TotalAnnualCosts, NumberSims, NumberYears, year_it);
		}
		
		UltimateTotalAvgCost = sum(AverageAnnualCosts, NumberYears);
		ObjectiveCost = max_array(AverageAnnualCosts, NumberYears);

		if (params.out_flag)
		{
			for (int month_it = 0; month_it < 12; month_it++)
			{
				out_stream << "AvWater(sims_by_years)-Month_" << month_it << endl;
				general_debug_output(out_stream, monthly_tracker[month_it].av_water, NumberSims, NumberYears);
			}
			for (int month_it = 0; month_it < 12; month_it++)
			{
				out_stream << "Nr(sims_by_years)-Month_" << month_it << endl;
				general_debug_output(out_stream, monthly_tracker[month_it].Nr, NumberSims, NumberYears);
			}
			for (int month_it = 0; month_it < 12; month_it++)
			{
				out_stream << "ResLevel(sims_by_years)-Month_" << month_it << endl;
				general_debug_output(out_stream, monthly_tracker[month_it].res_level_tracker, NumberSims, NumberYears);
			}
			if (params.model_case > 1)
			{
				out_stream << "No(sims_by_years)" << endl;
				general_debug_output(out_stream, sims_years_tracker.No, NumberSims, NumberYears);
				out_stream << "Nx(sims_by_years)" << endl;
				general_debug_output(out_stream, sims_years_tracker.total_Nx_tracker, NumberSims, NumberYears);
				for (int month_it = 0; month_it < 12; month_it++)
				{
					out_stream << "DroppedTransfers(sims_by_years)-Month_" << month_it << endl;
					general_debug_output(out_stream, monthly_tracker[month_it].dropped_transfer_tracker, NumberSims, NumberYears);
				}
				if (params.model_case > 2)
				{
					for (int month_it = 0; month_it < 12; month_it++)
					{
						out_stream << "Nl(sims_by_years)-Month_" << month_it << endl;
						general_debug_output(out_stream, monthly_tracker[month_it].total_monthly_leases, NumberSims, NumberYears);
					}
					for (int month_it = 0; month_it < 12; month_it++)
					{
						out_stream << "DroppedTransfers(sims_by_years)-Month_" << month_it << endl;
						general_debug_output(out_stream, monthly_tracker[month_it].dropped_transfer_tracker, NumberSims, NumberYears);
					}
					
				}
			}

			out_stream << "EndOfYearWater(sims_by_years)" << endl;
			general_debug_output(out_stream, sims_years_tracker.end_of_yr_water_tracker, NumberSims, NumberYears);
			out_stream << "TotalAnnualCosts(sims_by_years)" << endl;
			general_debug_output(out_stream, TotalAnnualCosts, NumberSims, NumberYears);
		
		}//end if(params.out_flag)

		//
		// surplus water
		//
		
		EndofYearWaterObjective = 0.0;
		for (int year_it = 0; year_it < NumberYears; year_it++)
		{
			EndofYearWaterObjective = EndofYearWaterObjective + (1/(double)NumberYears)*(average_array_colwise(sims_years_tracker.end_of_yr_water_tracker, NumberSims, NumberYears, year_it));
		}

		//
		// cost variation
		//
			
		for (int year_it = 0; year_it < NumberYears; year_it++)
		{
			for (int sims_it = 0; sims_it < NumberSims; sims_it++)
			{
				StdDevInput[sims_it] = TotalAnnualCosts[sims_it][year_it]; //create an input vector (pull from column)
			}
			StdDevCost[year_it] = std_dev(StdDevInput, NumberSims);
		}

		//to sort the totalannualcosts, we are going to pull out the columns again
		//and place them in a 2-d array.
		
		for (int year_it = 0; year_it < NumberYears; year_it++)
		{
			for (int sims_it = 0; sims_it < NumberSims; sims_it++)
			{
				cost_sorting_array[sims_it] = TotalAnnualCosts[sims_it][year_it];
			}
			sort(cost_sorting_array, cost_sorting_array+NumberSims);
			for (int sims_it = 0; sims_it < NumberSims; sims_it++)
			{
				SortedTotalAnnualCosts[sims_it][year_it] = cost_sorting_array[sims_it];
			}
		}
		
		if (!single_flag) //single_flag is local now
		{
			//calculate how many members will be in these percentile lists
			nthpercentile = ceil(NumberSims*0.95);
			uqpercentile = ceil(NumberSims*0.75);
			lqpercentile = ceil(NumberSims*0.25);

			//now we can allocate...
			general_2d_allocate(varvector, (int)(NumberSims-nthpercentile), NumberYears);

			//How to populate var_vector:
			//for example, if you had 100 sims where the nth percentile was 95, for each year:
			//varvector[4] = sortedcost[99]  (the fifth value in your varvector is the same as the
			//last sorted cost, or the most expensive cost in your list) .. and varvector[0] = sortedcost[95].

			for (int year_it = 0; year_it < NumberYears; year_it++)
			{
				//the outer loop will go and put a value in for every year following this
				//same process (yet another column trick to compensate for matlab)

				for (int subtraction_value = 1; subtraction_value <= (NumberSims - nthpercentile); subtraction_value++)
				{
					//there is probably a more straight-forward way of doing this,
					//but this was the matlab code convention. You assign backwards,
					//starting at the highest value and ending at the lowest value
					//subtraction_value is just a counter to facilitate this process
					varvector[(int)(NumberSims-nthpercentile-subtraction_value)][year_it] = SortedTotalAnnualCosts[NumberSims-subtraction_value][year_it];
				}
			}

			//the varvector contains costs at and above the 95th percentile.
			//some different high-cost variables are here but we don't use all of them
			
			for (int year_it = 0; year_it < NumberYears; year_it++)
			{
				//VAR = the cost at the 95th percentiile
				VAR[year_it] = varvector[0][year_it];
				double dummy_sum = 0.0;			
				for (int sims_it = 0; sims_it < (NumberSims-nthpercentile); sims_it++)
				{
					dummy_sum += varvector[sims_it][year_it];
				}
				//CVAR (per year) = the average of the costs falling above the 95th percnetile
				CVAR[year_it] = dummy_sum / (NumberSims - nthpercentile);
				//VARmean (per year) = the above value scaled by that year's cost
				VARmean[year_it] = CVAR[year_it] / AverageAnnualCosts[year_it];
				//In the WRR study, we could have used the max of the above vector,
				//but the calculation we make later is similar
			}
			meanVARmean = average_array(VARmean, NumberYears);
			ObjectiveVAR = average_array(VAR, NumberYears);
			//the variable naming in this code for the most part follows the UNC conventions,
			//so objectives that they may have used would be different than the WRR 2009 variables.
			//all the variables in the code can be used for analysis though; they are all mostly correct.
		} //end if(!params.single_flag)

		//
		//
		// Optimization Objectives
		//
		//

		//NOTE: The wrapper for the epsilon-NSGAII algorithm takes the vars variable passed in through the acb
		//structure and populates pointers to the the obj and constr vectors for output.  Each variable is calculated
		//as defined below.
		if (params.obj_flag)
		{
			//save the untransformed values for use in the aggregation calculation below

			//cout << "Storing full-period metrics for later: " << endl;
			
			g.fullperiod_cost = UltimateTotalAvgCost;		//cost
			g.fullperiod_rel = min_Reliability;				//rel
			g.fullperiod_crel = min_CReliability;				//crel
			g.fullperiod_cvar = (max_array(CVAR, NumberYears) / AverageAnnualCosts[max_index_array(CVAR, NumberYears)]);	//cvar
			
			//cout << "fullperiodcost = " << g.fullperiod_cost << endl;
			//cout << "fullperiodrel = " << g.fullperiod_rel << endl;
			//cout << "fullperiodcrel = " << g.fullperiod_crel << endl;
			//cout << "fullperiodcvar = " << g.fullperiod_cvar << endl;
			
			for (int obj_it = 0; obj_it < (int) params.obj_names.size(); obj_it++)
			{
				// Possible objectives are defined in obj_avail at top of this file and checked when reading control file
				if (params.obj_names[obj_it] == "cost")
				{
					objs[obj_it] = UltimateTotalAvgCost / params.obj_scalingfactors[obj_it];
				}
				else if (params.obj_names[obj_it] == "surplus")
				{
					objs[obj_it] = EndofYearWaterObjective / params.obj_scalingfactors[obj_it];
				}
				else if (params.obj_names[obj_it] == "critrel")
				{
					//9/26/2012: add -1.0 for minimization
					objs[obj_it] = (-1.0)*min_CReliability / params.obj_scalingfactors[obj_it];
				}
				else if (params.obj_names[obj_it] == "drop")
				{
					objs[obj_it] = TotalDroppedTransfers / params.obj_scalingfactors[obj_it];
				}
				else if (params.obj_names[obj_it] == "rel")
				{
					//9/26/2012: add -1.0 for minimization
					objs[obj_it] = (-1.0)*min_Reliability / params.obj_scalingfactors[obj_it];
				}
				else if (params.obj_names[obj_it] == "cvar")
				{
					objs[obj_it] = (max_array(CVAR, NumberYears) / AverageAnnualCosts[max_index_array(CVAR, NumberYears)]) / params.obj_scalingfactors[obj_it];
				}
				else if (params.obj_names[obj_it] == "numleases")
				{
					objs[obj_it] = TotalNumberTransfers / params.obj_scalingfactors[obj_it];
				}
			}
		}
		
		if (params.constr_flag)
		{
			int constr_marker = 0;
			for (int constr_it = 0; constr_it < (int) params.constr_names.size(); constr_it++)
			{
				//Possible constraints are defined in constr_avail at top of this file and checked when reading control file
				// first we read which of the constraints we are doing
				double constr_junk;
				if (params.constr_names[constr_it] == "rel")
				{
					constr_junk = min_Reliability;
					constr_marker = 1;
				}
				else if (params.constr_names[constr_it] == "critrel")
				{
					constr_junk = min_CReliability;
					constr_marker = 1;
				}
				else if (params.constr_names[constr_it] == "cvar")
				{
					constr_junk = (max_array(CVAR, NumberYears) / AverageAnnualCosts[max_index_array(CVAR, NumberYears)]);
					constr_marker = 1;
				}
				else constr_marker = 0;
				
				//next we read whether it is a greater than, less than, or equal constraint and calculate the
				//value accordingly

				//note that the algorithm knows that the constraint has been activated if and only if the value is less than zero;
				//thus everything is calculated as a sort of percent difference
				if (constr_marker)
				{
					//Fixed the constraint problem, 10-03-2012
					if (params.constr_comparators[constr_it] == ">=")
					{
						if (constr_junk >= params.constr_values[constr_it]) consts[constr_it] = 0.0;
						else
							consts[constr_it] = constr_junk / params.constr_values[constr_it] - 1.0;
					}
					else if (params.constr_comparators[constr_it] == "<=")
					{
						if (constr_junk <= params.constr_values[constr_it]) consts[constr_it] = 0.0;
						else consts[constr_it] = 1.0 - constr_junk / params.constr_values[constr_it];
					}
					else if (params.constr_comparators[constr_it] == "==")
					{
						if (constr_junk == params.constr_values[constr_it]) consts[constr_it] = 0.0;
						else
						{
							//9/26/12 fix a divide by zero...
							if (params.constr_values[constr_it] == 0)
								consts[constr_it] = -1.0*abs(constr_junk - params.constr_values[constr_it]);
							else
								consts[constr_it] = -1.0*abs((constr_junk - params.constr_values[constr_it])/params.constr_values[constr_it]);
						}
					}
				}

			}
		}

		//old objectives
		//Objective O0:  min_Reliability is defined above.  (Note previous versions had
		//a -1.0 multiplier (for maximization) but this is now handled explicitly in the
		//optimization wrapper).  No scaling factor.
		//obj[0] = min_Reliability;
		//Objective 3:  A year is chosen with the maximum CVAR, and the objective is its CVAR value scaled
		//by its cost.  A scaling factor is also applied. Only valid for params.model_case > 1 
		//obj[1] = (max_array(CVAR, NumberYears) / AverageAnnualCosts[max_index_array(CVAR, NumberYears)]) / 10;
		
		//Objective 0:  UltimateTotalAvgCost defined above, with a scaling factor.
		//obj[0] = UltimateTotalAvgCost /1.0e8;

		//Objective 1:  EndofYearWaterObjective defined above, with a scaling factor.
		//obj[1] = EndofYearWaterObjective /100000.0;
		//
		////Objective 2:  Critical Reliability ... maximized!
		//obj[2] = min_CReliability;

		////MODEL CASES above 1 (with options and leases)
		//if (params.model_case > 1)
		//{

		//	//Objective 3:  TotalDroppedTransfers defined above, with a scaling factor.
		//	obj[3] = TotalDroppedTransfers / 1.0e6;

		//	//MODEL CASES above 2 (with leases)
		//	if (params.model_case > 2)
		//	{
		//		//Objective 4:  TotalNumberTransfers defined above, with a scaling factor.
		//		obj[4] = TotalNumberTransfers / 100;
		//	}

		//}
		if (params.timing_flag)
		{
			timers.obj_end = clock();

			timers.obj_sum = timers.obj_end - timers.obj_start;
		}

		//constr[0] = min_Reliability / 0.98 - 1.0; //reliability above 98%
		////the following critical reliability constraint was incorrectly defined for
		////"trial08" of the WRR study (thus it was never activated).  In "trial09", it was
		////fixed and therefore it worked correctly.  It forces CReliability to be exactly 100%
		////in each year.
		////constr[1] = sum_CReliability / ((double)NumberYears) - 1.0; //100% creliability constraint
		//constr[1] = min_CReliability / 0.99 - 1.0; //max-min CReliability .. greater than 99%
		//if (params.model_case > 1)
		//{
		//	constr[2] = 1.0 - (max_array(CVAR, NumberYears) / AverageAnnualCosts[max_index_array(CVAR, NumberYears)]) / 1.1; //cost variability below 1.1
		//}
		
		if (!single_flag) //local now
		{
			//the only variable we are really concerned with deallocating is the
			//varvector, because it is allocated at each iteration as a function of
			//the number of simulations called at runtime.  All the other variables are
			//statically allocated and freed at the end of the set of simulations.
			zap(varvector, (int)(NumberSims-nthpercentile));
		}

		if (params.timing_flag) //total time
		{
			time_stream << timers.before_loop_sum << "   " << timers.monte_carlo_sum << "   ";
			time_stream << timers.calcs_sum << "   " << timers.obj_sum << "   ";
			endtime = clock() - start;
			time_stream << endtime << endl;
		}

		//Begin Results Reporting (*.csv file for objectives, and yearly data)
		if (params.results_flag)
		{
			results_stream.setf(ios::fixed);
			
			//see function "d" for the delimiter

			if (params.mode == "resample") //legacy: processing_flag != 2
			{
				results_stream << g.alg;
				results_stream << getDelim << g.config;
				results_stream << getDelim << g.RS;
				results_stream << getDelim << g.index << getDelim;
			}
			
			results_stream << setprecision(2) << UltimateTotalAvgCost;
			if (calc_param == "drought_full")
			{
				//Calculate Drought Transfers Cost
				double drtranscost = 0.0;
				for (int month_it = 0; month_it < 12; month_it++)
				{
					drtranscost += monthly_tracker[month_it].yearly_lease_cost[0][0];
				}
				drtranscost += sims_years_tracker.total_Nx_tracker[0][0]*Px;
				results_stream << getDelim << setprecision(2) << drtranscost;
			}
			results_stream << getDelim << setprecision(6) << min_Reliability;
			results_stream << getDelim << setprecision(6) << min_CReliability;
			results_stream << getDelim << setprecision(1) << EndofYearWaterObjective;

			if (params.model_case > 1)
			{
				results_stream << getDelim;
				results_stream << setprecision(6);
				results_stream << (max_array(CVAR, NumberYears) / AverageAnnualCosts[max_index_array(CVAR, NumberYears)]);
				results_stream << getDelim << scientific << setprecision(6) << TotalDroppedTransfers;
				if (params.model_case > 2)
				{
					results_stream << getDelim << fixed << setprecision(6) << TotalNumberTransfers;
				}
			}
			results_stream << setprecision(6);
			double temp = resilience_calc();

			results_stream << getDelim << resilience_calc();
			results_stream << getDelim << failvol_calc();
			vulnerability_calc(params.NumberSims, params.NumberYears);
			results_stream << getDelim << g.vulnerability << getDelim << g.failure_periods;

			//03-16-2015: These variables are not valid anymore, took them out of reporting
			//results_stream << getDelim << constr[0] << getDelim << constr[1];
		//	if (params.model_case > 1) results_stream << getDelim << constr[2] ;

			//results_stream << getDelim << params.ifri;

			results_stream << getDelim << setprecision(0) << strategy.Nrt;
			if (params.model_case > 1)
			{
				results_stream << getDelim << setprecision(3) << strategy.xi;
				results_stream << getDelim << setprecision(0) << strategy.No_low;
				results_stream << getDelim << setprecision(0) << strategy.No_high;
				results_stream << getDelim << setprecision(6) << strategy.alpha2;
				results_stream << getDelim << setprecision(6) << strategy.beta2;
				if (params.model_case > 2)
				{
					results_stream << getDelim << strategy.alpha << getDelim << strategy.beta;
				}
			}

			results_stream << getDelim << ifri;

			results_stream << setprecision(6); //just to make sure the order doesn't change later and this gets ignored

			for (int year_it = 0; year_it < NumberYears; year_it++) results_stream << getDelim << Reliability[year_it];
			results_stream << getDelim << average_array(Reliability, params.NumberYears);

			for (int year_it = 0; year_it < NumberYears; year_it++) results_stream << getDelim << CReliability[year_it];
			results_stream << getDelim << average_array(CReliability, params.NumberYears);

			if (params.model_case > 1){
				results_stream << setprecision(1);
				for (int year_it = 0; year_it < NumberYears; year_it++) results_stream << getDelim << DroppedTransfersVector[year_it];
				results_stream << getDelim << average_array(DroppedTransfersVector, params.NumberYears);
			}

			results_stream << setprecision(2);
			for (int year_it = 0; year_it < NumberYears; year_it++) results_stream << getDelim << AverageAnnualCosts[year_it];
			results_stream << getDelim << average_array(AverageAnnualCosts, params.NumberYears);

			results_stream << setprecision(2);
			for (int year_it = 0; year_it < NumberYears; year_it++) results_stream << getDelim << AverageYearlyLeaseCost[year_it];
			results_stream << getDelim << average_array(AverageYearlyLeaseCost, params.NumberYears);

			results_stream << setprecision(4);
			results_stream << getDelim << PermRightCosts[0][0]/average_array(AverageAnnualCosts, params.NumberYears);
			if (params.model_case > 1)
			{
				temp_double = 0.0;
				for (int sims_it = 0; sims_it < NumberSims; sims_it++)
				{
					for (int year_it = 0; year_it < NumberYears; year_it++)
					{
						temp_double += sims_years_tracker.No[sims_it][year_it]*sims_years_tracker.total_Po_list[sims_it][year_it]+
							sims_years_tracker.total_Nx_tracker[sims_it][year_it]*Px;
					}
				}
				results_stream << getDelim << temp_double / (NumberSims*NumberYears*average_array(AverageAnnualCosts, params.NumberYears));
				if (params.model_case > 2)
				{
					results_stream << getDelim << average_array(AverageYearlyLeaseCost, params.NumberYears)/average_array(AverageAnnualCosts, params.NumberYears);
				}
			}		

			results_sum = 0.0;
			if (params.model_case > 1)
			{
				for (int year_it = 0; year_it < NumberYears; year_it++)
				{
					high_count = 0;
					low_count = 0;
					highlow_error = 0;
					for (int sims_it = 0; sims_it < NumberSims; sims_it++)
					{
						if (sims_years_tracker.No[sims_it][year_it] == strategy.No_high) high_count++;
						else if (sims_years_tracker.No[sims_it][year_it] == strategy.No_low) low_count++;
						else highlow_error++;
					}

					if ((high_count + low_count) != NumberSims) results_stream << ",counting_error";
					else if (highlow_error) results_stream << ",highlow_error";
					else results_stream << getDelim << high_count;
					results_sum = results_sum + (double) high_count;
					
				} // end up year loop for high/low options count
				results_stream << getDelim << (1/((double) params.NumberYears))*results_sum;

				results_sum = 0.0;

				//Nx count
				for (int year_it = 0; year_it < NumberYears; year_it++)
				{
					high_count = 0;
					low_count = 0;
					highlow_error = 0;
					for (int sims_it = 0; sims_it < NumberSims; sims_it++)
					{
						if (sims_years_tracker.total_Nx_tracker[sims_it][year_it] > 0) high_count++;
						else if (sims_years_tracker.total_Nx_tracker[sims_it][year_it] == 0) low_count++;
						else highlow_error++;
					}

					if ((high_count + low_count) != NumberSims) results_stream << ",counting_error";
					else if (highlow_error) results_stream << ",highlow_error";
					else results_stream << getDelim << high_count;
					
					results_sum = results_sum + (double) high_count;

				}// end year loop for Nx count
				results_stream << getDelim << (1/((double) params.NumberYears))*results_sum;
				
				results_sum = 0.0;

				for (int year_it = 0; year_it < NumberYears; year_it++)
				{
					temp_double = average_array_colwise(sims_years_tracker.total_Nx_tracker, NumberSims, NumberYears, year_it);
					results_stream << getDelim << temp_double;
					results_sum = results_sum + temp_double;
				}
				results_stream << getDelim << (1/((double) params.NumberYears))*results_sum;

				results_sum = 0.0;

				if (params.model_case > 2)
				{
					for (int year_it = 0; year_it < NumberYears; year_it++)
					{
						temp_double = average_array_colwise(sims_years_tracker.yearly_purchased_leases, NumberSims, NumberYears, year_it);
						results_stream << getDelim << temp_double;
						results_sum = results_sum + temp_double;
					}
					results_stream << getDelim << (1/((double) params.NumberYears))*results_sum;

					results_sum = 0.0;
				}
			} //end if params.model_case > 1

			results_sum = 0.0;
			for (int year_it = 0; year_it < NumberYears; year_it++)
			{
				temp_double = average_array_colwise(sims_years_tracker.end_of_yr_water_tracker, NumberSims, NumberYears, year_it);
				results_stream << getDelim << temp_double;
				results_sum = results_sum + temp_double;
			}
			results_stream << getDelim << (1/((double) params.NumberYears))*results_sum;
			results_sum = 0.0;

			//This is also a fix to ensure that the Sobol results print correctly
			//In the ten-year Sobol, we aren't running the concurrent drought simulation,
			//so we need a new line here...
			//9/21/2012 Removed preprocessor definition here.
			if (params.mode == "sobol" || params.sync_flag) results_stream << endl; //legacy: first part was processing_flag == 2
			
			//On 3/23/2015 the BAND-AID fixes continue.  Almost all applications of the
			//LRGV use the 'combined' calculation mode, and for this, the drought will
			//put out its own metrics.  Therefore we will comment this out, and allow the drought
			//to continue putting out stuff.
			//if (params.mode == "std-io") results_stream << endl;
		} // end if params.results_flag
		//End Results Reporting
		if (params.monthly_flag) write_monthly_output();
	} //end, if the calc_param is ten year or drought full.
	
	//
	//
	// BEGIN DROUGHT METRIC CALCULATIONS
	//
	//

	else if (calc_param == "drought_noinit")
	{
			//cout << "Checking to make sure fullperiod metrics are still here: " << endl;
			
			//cout << g.fullperiod_cost << endl;
			//cout << g.fullperiod_rel << endl;
			//cout << g.fullperiod_crel << endl;
			//cout << g.fullperiod_cvar << endl;

			//cout << "also check failure and total periods: " << endl;
			//cout << g.failure_periods << endl;
			//cout << g.total_periods << endl;

		//Calculate Drought Transfers Cost
		double drtranscost = 0.0;
		for (int month_it = 0; month_it < 12; month_it++)
		{
			drtranscost += monthly_tracker[month_it].yearly_lease_cost[0][0];
		}
		drtranscost += sims_years_tracker.total_Nx_tracker[0][0]*Px;

		//Calculate Vulnerability
		vulnerability_calc(1, 1);

		//Assign Objectives and Constraints
		if (params.obj_flag)
		{
		        //Possible objectives are defined in obj_avail at top of this file and checked when reading control file
			for (int obj_it = 0; obj_it < (int) params.obj_names.size(); obj_it++)
			{
				if (params.obj_names[obj_it] == "drtranscost")
					objs[obj_it] = drtranscost / params.obj_scalingfactors[obj_it];
				else if (params.obj_names[obj_it] == "drvuln")
					objs[obj_it] = g.vulnerability / params.obj_scalingfactors[obj_it];
				else if (params.obj_names[obj_it] == "aggcost")
				{
					//cout << "cost, cvar, and drcost are: " << endl;
					//cout << g.fullperiod_cost << "," << g.fullperiod_cvar << "," << drtranscost << "." << endl;
					objs[obj_it] = 
						g.fullperiod_cost/100000000.0 +
						g.fullperiod_cvar/10.0 +
						drtranscost/10000000.0;
				}
				else if (params.obj_names[obj_it] == "aggrel")
				{
					//cout << "rel, crel, and drrel are: " << endl;
					//cout << g.fullperiod_rel << "," << g.fullperiod_crel << "," << (12.0 - double(g.total_periods))/12.0 << "." << endl;									
					//9/26/2012: add -1.0 for minimization
					objs[obj_it] =
						(-1.0)*(g.fullperiod_rel + g.fullperiod_crel + (12.0 - double(g.total_periods))/12.0)/3.0;
				}
			}
		}

		if (params.constr_flag)
		{
                        //Possible constraints are defined in constr_avail at top of this file and checked when reading control file
			int constr_marker = 0;
			for (int constr_it = 0; constr_it < (int) params.constr_names.size(); constr_it++)
			{
				double constr_junk;
				if (params.constr_names[constr_it] == "drvuln")
				{
					constr_junk = g.vulnerability;
					constr_marker = 1;
				}
				else
				{
					constr_marker = 0;
				}
				
				//next we read whether it is a greater than, less than, or equal constraint and calculate the
				//value accordingly

				//note that the algorithm knows that the constraint has been activated if and only if the value is less than zero;
				//thus everything is calculated as a sort of percent difference
				if (constr_marker)
				{
					//fixed constraints, 10-03-12
					if (params.constr_comparators[constr_it] == ">=")
					{
						if (constr_junk >= params.constr_values[constr_it]) consts[constr_it] = 0.0;
						else
							consts[constr_it] = constr_junk / params.constr_values[constr_it] - 1.0;
					}
					else if (params.constr_comparators[constr_it] == "<=")
					{
						if (constr_junk <= params.constr_values[constr_it]) consts[constr_it] = 0.0;
						else consts[constr_it] = 1.0 - constr_junk / params.constr_values[constr_it];
					}
					else if (params.constr_comparators[constr_it] == "==")
					{
						if (constr_junk == params.constr_values[constr_it]) consts[constr_it] = 0.0;
						else
						{
							//9/26/12 fix a divide by zero...
							if (params.constr_values[constr_it] == 0)
							{
								if (params.constr_names[constr_it] == "drvuln")
									consts[constr_it] = -1.0*abs(constr_junk - params.constr_values[constr_it])/10000.0;
								else
									consts[constr_it] = -1.0*abs(constr_junk - params.constr_values[constr_it]);
							}
							else
								consts[constr_it] = -1.0*abs((constr_junk - params.constr_values[constr_it])/params.constr_values[constr_it]);
						}
					}

				}

			}
		}

		if (params.results_flag)
		{
			results_stream << getDelim << drtranscost;
			results_stream << getDelim << g.vulnerability << getDelim << g.failure_periods << getDelim << g.total_periods;
			double temp_demand, temp_water;
			for (int month_it = 0; month_it < 12; month_it++)
			{
				results_stream << getDelim << monthly_tracker[month_it].av_water[0][0];
				results_stream << getDelim << monthly_tracker[month_it].Nr[0][0];
				results_stream << getDelim << monthly_tracker[month_it].total_monthly_leases[0][0];
				results_stream << getDelim << samples[month_it].demand[0];
				temp_demand = samples[month_it].demand[0];
				temp_water = monthly_tracker[month_it].av_water[0][0];
				results_stream << getDelim << temp_demand - temp_water;
			}
			results_stream << getDelim << sims_years_tracker.No[0][0];
			results_stream << getDelim << sims_years_tracker.total_Nx_tracker[0][0];
			results_stream << endl;
		}
	}

	/* Finalize Roulette Calculations */
	if (params.roulette_flag)
	{
		//deallocate memory
		finalize_roulette();
	}
}
