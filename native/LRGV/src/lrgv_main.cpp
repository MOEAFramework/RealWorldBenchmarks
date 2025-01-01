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

// Variables for the random number generator
double seed[52]; //a set of seeds
double oldrand[55]; //an array used in the random number generation
int jrand; //keeps track of how many numbers have been generated
int RS;

void usage(int argc, char* argv[])
{
	cerr << "Usage: " << argv[0] << " [OPTIONS]" << endl;
	//UPDATE THIS LIST
	cerr << "-m <mode> \t Mode. Options are std-io and sobol. REQUIRED." << endl;
	cerr << "-b <filename_base> \t The phrase used to name parameter and output files. REQUIRED." << endl;
	cerr << "-c <calculation_type> \t Calculation type.  Options are ten-year, drought-full, combined.  REQUIRED." << endl;
	cerr << "-s <seed> \t Seed. (optional)." << endl;
	cerr << "-r <realizations> \t Number of Monte Carlo realizations.  Optional here, but if you don't give it here, it must be in the control file!" << endl;
	cerr << "-h Help (this screen)." << endl;
	cerr << endl;

	cerr << "Available objectives are: ";
	vector<string>::const_iterator cii;
	for(cii=obj_avail.begin(); cii!=obj_avail.end(); cii++){
	  cerr << *cii << " ";
	}
	cerr << endl;
	cerr << "Available constraints are: ";
	for(cii=constr_avail.begin(); cii!=constr_avail.end(); cii++){
	  cerr << *cii << " ";
	}
	cerr << endl;

	exit(-1);	
	return;
}

int main(int argc, char **argv)
{
	//UPDATES:
	//-Uses getopt with wider array of command line options.
	//-Integrated MOEAframework-compatible code with old Sobol version and MORDM roulette features.
	//-Now the number of random samples can be input on the command line (or in the control file)
	//-No more LRGV_SAMPLER preprocessor definitions, with an intelligent params.mode value that controls how
	//data is handled.
	//-Two new objectives called aggregated cost and reliability.  Basic combination of several ten-year cost and
	//reliability measures with measures from the concurrent drought.
	
	
	//TO DO: 
	//
	//
	//Add parsing of getopt directly through some sort of function, for "embedded" CBorg-MS.
	//
	//Allow some decision variables to be hard coded and others to change.  This is already available in
	//"sobol mode" but not in std-io, as far as I can see.  
	
	//modes:
	//std-io reads inputs from standard in, outputs to standard out
	
	//cases:
	//should it be based on the papers? let them make their own case?
	//if so how should the decision variable string be read in?
	
	//local variables
	string input_filename; //input vectors for sampler, of form "*_parameters.txt"
	string ranges_filename;
	string local_calcparam;
	int num_sol = 0; //number of sampled solutions, read from vector input file
	
	//
	//PARSE COMMAND LINE INPUT
	//
	int opt;
	int RS = 0;
	params.mode = "unset";
	local_calcparam = "unset";
	params.NumberSims = -1;
	//cout << "local_calcparam is originally " << local_calcparam << "." << endl;

	//while ((opt = getopt(argc, argv, "m:b:c:s::h::")) != -1)
	while ((opt = getopt(argc, argv, "m:b:c:s:r:h")) != -1)
	{
		switch (opt)
		{
			case 'm':	//modeling mode
				//cout << "We hit the m case and optarg is: " << optarg << "." << endl;
				params.mode = optarg;
				//cout << "set mode to: " << params.mode << "." << endl;
				break;
			case 'b':	//filename base
				//cout << "We hit the b case and optarg is: " << optarg << "." << endl;
				params.filename_base = optarg;
				//cout << "Set params.filename_base to: " << params.filename_base << "." << endl;
				break;
			case 'c':	//calculation type (ten year, drought, etc)
				//cout << "We hit the c case and optarg is: " << optarg << "." << endl;
				local_calcparam = optarg;
				//cout << "Set local_calcparam to: " << local_calcparam << "." << endl;
				break;
			case 's':	//random seed
				//cout << "We hit the s case and optarg is: " << optarg << "." << endl;
				RS = atoi(optarg);
				//cout << "set RS to: " << RS << "." << endl;
				break;
			case 'r':	//realizations
				params.NumberSims = atoi(optarg);
				break;
			case 'h':	//help
				usage(argc, argv);
				break;
			default:
				usage(argc, argv);
				break;
		}
	}

	//check required parameters
	if (params.mode != "std-io" && params.mode != "sobol")
	{
		if (params.mode == "unset")  	cerr << "Error! m (mode) is a required parameter. Exiting..." << endl;
		else						cerr << "Mode is wrong.  You entered: " << params.mode << "." << endl;
		exit(-1);
	}
	
	if (local_calcparam != "ten-year" && local_calcparam != "drought-full" && local_calcparam != "combined")	//CHECK THIS
	{
		if (local_calcparam == "unset")  	cerr << "Error! m (mode) is a required parameter. Exiting..." << endl;
		else						cerr << "Mode is wrong (either ten-year, drought-full, or combined). You entered: " << params.mode << "." << endl;
		exit(-1);
	}
	
	//check mode compatibility
	if (local_calcparam == "combined" && params.mode == "sobol")
	{
		cerr << "Error! Sobol is not yet compatible with the combined (10-year and drought) mode.  Sorry!" << endl;
		exit(-1);
	}
	
	//
	//INITIALIZATION
	//
	
	//no matter what, initialize random number generator
	//Note: The cout commands are for debugging only.  They may interrupt the flow of the Borg standard/io communication if used!
	//cout << "Init seeds...";
	init_seeds();
	//cout << "OK!" << endl;
	
	//cout << "Randomize...";
	randomize(seed[RS]);
	//cout << "OK!" << endl;
	
	//initialize the model
	//cout << "Init_LRGV()...";
	if (local_calcparam == "drought-full" || local_calcparam == "ten-year")
	{
		//only initialize once and pass the parameter in directly
		init_LRGV(argv, local_calcparam);		
	}
	else if (local_calcparam == "combined")
	{
		init_LRGV(argv, "ten-year");
		init_LRGV(argv, "drought_noinit");
	}

	//cout << "OK!" << endl;
	
	//initialize the individual
	//Individual actual_ind;
	//Individual *ind = &actual_ind;
	//double *xreal   = ind->xreal;
	//cout << "Created individual and xreal pointer." << endl;
	//cout << "Obj size is: " << params.obj_names.size() << "!" << endl;
	
	//Parse the cases.  Cases 1-6 are used for DECISION VARIABLES ONLY.  Objectives and
	//constraints are handled in the control file.
	
	if (params.obj_names.size() <= 0) 	{cerr << "Error! Objectives count from control file is: " << params.obj_names.size() << ". Exiting."; exit(-1);}

	int nvars = -1; int nobjs = -1; int nconstrs = -1;
	
	nobjs = int(params.obj_names.size());
	//Fixed a bug, where the program would think there are still constraints even if the constraints
	//flag is turned off in the control file.
	if (params.constr_flag == 1)
	{
		nconstrs = int(params.constr_names.size());
	}
	else
	{
		nconstrs = 0;
		if (params.constr_names.size() != 0)
		{
			//This is an error because there are constraints in the control file, but the constraint flag has been turned off.
			cerr << "Error! Constraints count from control file is: " << params.constr_names.size() << ", but the constraint_flag indicates there are no constraints. Exiting."; exit(-1);
		}
	}
	
	if (params.model_case == 1)		{nvars = 1;}	//WRR: Case A
	else if (params.model_case == 2)	{nvars = 6;}	//WRR: Case B
	else if (params.model_case == 3)	{nvars = 8;}	//WRR: Case C/D, de Novo Case IV. 
	else if (params.model_case == 4)	{nvars = 3;}	//de Novo Case I
	else if (params.model_case == 5) 	{nvars = 4;}	//de Novo Case II
	else if (params.model_case == 6)	{nvars = 6;}	//de Novo Case III

	//Need to enable the ability to have some variables 'hard-coded'!
	
	//allocate memory
	//cout << "Allocating memory...with objs " << nobjs << ", constrs " << nconstrs << ", and vars " << nvars << "." << endl;
	//ind->obj = 	new double[nobjs];
	//ind->constr =	new double[nconstrs];
	//ind->xreal = 	new double[nvars];
	double *vars, *objs, *consts;
	vars = new double [nvars];
	objs = new double [nobjs];
	consts = new double [nconstrs];

	//cout << "OK!" << endl;
	
	//EXECUTION
	
	//Call a different execution for std-io under the "combined" scenario.  Do this comparison outside the loop to save time.	
	if (params.mode == "std-io")
	{
		MOEA_Init(nobjs, nconstrs);

		while(MOEA_Next_solution() == MOEA_SUCCESS)
		{
			MOEA_Read_doubles(nvars,vars); //first read the solution
			
			// TODO: Remove
			MOEA_Debug("Vars: %lf %lf %lf %lf %lf %lf %lf %lf\n", vars[0], vars[1], vars[2], vars[3], vars[4], vars[5], vars[6], vars[7]);
			
			//If the mode is 'combined' you need to run the drought scenario.
			//Otherwise, you can skip it. NOTE we should probably check that if
			//drtranscost is listed as an objective, or one of the other 'dr'
			//variables is listed, you MUST run combined!
			
			// 5/20/2015: The transform function was not called here, even though
			// the flag would have indicated that it was supposed to be used!
			if (params.discretize_flag) transform_LRGV(vars);
			
			if (local_calcparam == "combined")
			{
				calc_LRGV(vars, objs, consts, "ten-year");
				calc_LRGV(vars, objs, consts, "drought_noinit");
			}
			else
			{
				calc_LRGV(vars, objs, consts, local_calcparam); 
			}
			
			// TODO: Remove
			MOEA_Debug("Objs: %lf %lf %lf %lf %lf %lf\n", objs[0], objs[1], objs[2], objs[3], objs[4], objs[5]);
			MOEA_Debug("Constrs: %lf %lf %lf %lf %lf\n", consts[0], consts[1], consts[2], consts[3]);
			
			MOEA_Write(objs, consts);
			//cerr << "Just calculated" << endl;
		}	
		
	}
	else if (params.mode == "sobol")
	{
		//cout << "Processing for Sobol." << endl;
		
		//prepare filenames
		input_filename = params.filename_base + "_parameters.txt";
		ranges_filename = params.filename_base + "_ranges.txt";
		
		char junk_char[2000];
		int junk_int;
		double junk_double;
		int sol_it = 0;
		
		char ranges_char[1000];
		int order_counter = 0;
		int *file_order;
		double *parameter_array;
		params.number_sampled_param = 0;
		
		//there are only so many "possibilities" for sampling parameters
		//(currently 21).  The file order will contain a prescribed order
		//of the 21 parameters, onto which we will map the "actual" order
		//of parameters.
		params.total_possible_samples = 21;
		file_order = new int[params.total_possible_samples];
		
		//open ranges file
		ifstream ranges_stream;
		ranges_stream.open(ranges_filename.c_str(), ios::in);
		if (!ranges_stream) {cout << "Failed to open ranges file.  Exiting ..."; exit(-1);}
		
		//determine how many parameters are sampled (i.e., how many lines are in
		//the ranges file
		int ranges_it = 0;
		while(!ranges_stream.eof())
		{
			ranges_it++;
			ranges_stream.getline(junk_char,2000);
		}
		params.number_sampled_param = ranges_it - 1;
		
		//get ready to read the ranges file again
		ranges_stream.clear();
		ranges_stream.seekg(0,ios::beg);
		
		//now that we know the number of sampled params, we can populate this parameter
		parameter_array = new double[params.number_sampled_param];

		//assume nothing is in the file:
		strategy.Nrt_order = 							params.total_possible_samples;
		strategy.No_low_order = 						params.total_possible_samples;
		strategy.No_high_order = 						params.total_possible_samples;
		strategy.xi_order = 							params.total_possible_samples;
		strategy.alpha2_order = 						params.total_possible_samples;
		strategy.beta2_order = 						params.total_possible_samples;
		strategy.alpha_order = 						params.total_possible_samples;
		strategy.beta_order = 							params.total_possible_samples;
		params.ifri_order = 							params.total_possible_samples;
		params.iRo_order = 							params.total_possible_samples;
		params.DemGrowthFactor_order = 					params.total_possible_samples;
		params.Pr_order = 							params.total_possible_samples;
		params.Px_order = 							params.total_possible_samples;
		params.Po_order = 							params.total_possible_samples;
		params.OExerciseMonth_order = 					params.total_possible_samples;
		params.critical_reliability_threshold_order = 			params.total_possible_samples;
		params.inf_weight_order = 						params.total_possible_samples;
		params.los_weight_order = 						params.total_possible_samples;
		params.res_var_weight_order = 					params.total_possible_samples;
		params.lease_weight_order = 					params.total_possible_samples;
		params.demand_weight_order = 					params.total_possible_samples;

		//now see what the actual order is in the parameters file:
		for (ranges_it = 0; ranges_it < params.number_sampled_param; ranges_it++)
		{
			ranges_stream >> ranges_char;
			if (!strcmp(ranges_char, "rights")){strategy.Nrt_order = order_counter;	order_counter++;}
			else if (!strcmp(ranges_char, "options_low")){strategy.No_low_order = order_counter; order_counter++;}
			else if (!strcmp(ranges_char, "options_high")){strategy.No_high_order = order_counter; order_counter++;}
			else if (!strcmp(ranges_char, "xi")){strategy.xi_order = order_counter; order_counter++;}
			else if (!strcmp(ranges_char, "alpha2")){strategy.alpha2_order = order_counter; order_counter++;}
			else if (!strcmp(ranges_char, "beta2")){strategy.beta2_order = order_counter; order_counter++;}
			else if (!strcmp(ranges_char, "alpha")){strategy.alpha_order = order_counter; order_counter++;}
			else if (!strcmp(ranges_char, "beta")) {strategy.beta_order = order_counter; order_counter++;}
			else if (!strcmp(ranges_char, "initial_rights")) {params.ifri_order = order_counter; order_counter++;}
			else if (!strcmp(ranges_char, "initial_reservoir_volume")) {params.iRo_order = order_counter; order_counter++;}
			else if (!strcmp(ranges_char, "demand_growth_factor")) {params.DemGrowthFactor_order = order_counter; order_counter++;}
			else if (!strcmp(ranges_char, "rights_price")) {params.Pr_order = order_counter; order_counter++;}
			else if (!strcmp(ranges_char, "strike_price")) {params.Px_order = order_counter; order_counter++;}
			else if (!strcmp(ranges_char, "options_price")) {params.Po_order = order_counter; order_counter++;}
			else if (!strcmp(ranges_char, "option_exercise_month")) {params.OExerciseMonth_order = order_counter; order_counter++;}
			else if (!strcmp(ranges_char, "critical_reliability_threshold")) {params.critical_reliability_threshold_order = order_counter; order_counter++;}
			else if (!strcmp(ranges_char, "inf_weight")) {params.inf_weight_order = order_counter; order_counter++;}
			else if (!strcmp(ranges_char, "los_weight")) {params.los_weight_order = order_counter; order_counter++;}
			else if (!strcmp(ranges_char, "res_var_weight")) {params.res_var_weight_order = order_counter; order_counter++;}
			else if (!strcmp(ranges_char, "lease_weight")) {params.lease_weight_order = order_counter; order_counter++;}
			else if (!strcmp(ranges_char, "demand_weight")) {params.demand_weight_order = order_counter; order_counter++;}
			else
			{
				cout << "Error! An entry in your ranges file does not match the list of accepted variables. Exiting..." << endl;
				#ifndef LINUX
					system("PAUSE");
				#endif
				exit(1);
			}
			ranges_stream.ignore(1000, '\n');
		}
		ranges_stream.close();
		
		//match those orders with what they are supposed to be
		//i.e., rights is always first, so the first entry in the file_order array
		//shows that it will really be contained in the strategy.Nrt_order position
		file_order[0] = strategy.Nrt_order;
		file_order[1] = strategy.No_low_order;
		file_order[2] = strategy.No_high_order;
		file_order[3] = strategy.xi_order;
		file_order[4] = strategy.alpha2_order;
		file_order[5] = strategy.beta2_order;
		file_order[6] = strategy.alpha_order;
		file_order[7] = strategy.beta_order;
		file_order[8] = params.ifri_order;
		file_order[9] = params.iRo_order;
		file_order[10] = params.DemGrowthFactor_order;
		file_order[11] = params.Pr_order;
		file_order[12] = params.Px_order;
		file_order[13] = params.Po_order;
		file_order[14] = params.OExerciseMonth_order;
		file_order[15] = params.critical_reliability_threshold_order;
		file_order[16] = params.inf_weight_order;
		file_order[17] = params.los_weight_order;
		file_order[18] = params.res_var_weight_order;
		file_order[19] = params.lease_weight_order;
		file_order[20] = params.demand_weight_order;

		if (params.number_sampled_param > 0)
		{
			//now read input vectors and calcuate model output
			ifstream in;
			in.open(input_filename.c_str(), ios::in);
			while(!in.eof())
			{
				sol_it++;
				in.getline(junk_char,2000);
			}
			num_sol = sol_it - 1;
			//cout << "There are " << num_sol << " solutions in the file." << endl;

			in.clear();
			in.seekg(0,ios::beg);
			//right now we will only write for model_case == 3
			for (int i = 0; i < num_sol; i++)
			{
				string string_temp;
				getline(in,string_temp);
				istringstream iss(string_temp);
				for (int string_counter = 0; string_counter < params.number_sampled_param; string_counter++)
				{
					iss >> parameter_array[string_counter];
				}
				iss.clear();

				for (int dec_var_it = 0; dec_var_it < 8; dec_var_it++)//the decision variables
				{
					if (file_order[dec_var_it] < params.total_possible_samples)
					{
						vars[dec_var_it] = parameter_array[file_order[dec_var_it]];
					}
				}
				if (file_order[8] < params.total_possible_samples)
				{
					params.ifri_dist = "constant";
					params.ifri_param1 = parameter_array[file_order[8]];
				}
				if (file_order[9] < params.total_possible_samples) params.iRo = parameter_array[file_order[9]];
				if (file_order[10] < params.total_possible_samples) params.DemGrowthFactor = parameter_array[file_order[10]];
				if (file_order[11] < params.total_possible_samples) params.Pr = parameter_array[file_order[11]];
				if (file_order[12] < params.total_possible_samples) params.Px = parameter_array[file_order[12]];
				if (file_order[13] < params.total_possible_samples) params.Po = parameter_array[file_order[13]];
				if (file_order[14] < params.total_possible_samples) params.OExerciseMonth = (int) rounding(parameter_array[file_order[14]]); //rounding to int
				if (file_order[15] < params.total_possible_samples) params.critical_reliability_threshold = parameter_array[file_order[15]];
				if (file_order[16] < params.total_possible_samples) params.inf_weight = (int) rounding(parameter_array[file_order[16]]);
				if (file_order[17] < params.total_possible_samples) params.los_weight = (int) rounding(parameter_array[file_order[17]]);
				if (file_order[18] < params.total_possible_samples) params.res_var_weight = (int) rounding(parameter_array[file_order[18]]);
				if (file_order[19] < params.total_possible_samples) params.lease_weight = (int) rounding(parameter_array[file_order[19]]);
				if (file_order[20] < params.total_possible_samples) params.demand_weight = (int) rounding(parameter_array[file_order[20]]);

				if (params.discretize_flag) transform_LRGV(vars); //usually in the Sobol, the transform is not needed because the routines take care of any problems
				calc_LRGV(vars, objs, consts, local_calcparam);				
			}
			zap(file_order);
			zap(parameter_array);
			
			in.close();
		} //end "if the number of sampled params is greater than zero"
		else
		{
			//this should theoretically run one solution based on only the material contained in the
			//parameter file
			if (params.discretize_flag) transform_LRGV(vars);
			calc_LRGV(vars, objs, consts, local_calcparam);		
		}		
	} //return sobol
	
	return 0;
}
