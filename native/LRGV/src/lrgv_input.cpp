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


/**
	lrgv_input.cpp
	Purpose: Write simulation output.  Read input files, and read the
	control.txt parameter file.  Also provide declarations of global variables.
	
	@author JR Kasprzyk
	@version 2013-03-10
*/

#include <global.h>

year_tracker_structure annual_tracker;
sims_years_structure sims_years_tracker;
monthly_tracker_structure monthly_tracker[12];

hydro_structure hydro_sets[12];
lease_structure lease_sets[12];
demand_structure demand_sets[12];
param_structure params;

sampled_data samples[12];
future_structure futures[12];
strategy_structure strategy;

super_structure super;
timer_structure timers;
drought_structure drought;

global_reporting_structure g;

ofstream time_stream;
ofstream results_stream;
ofstream rng_stream;
ofstream out_stream;
ofstream monthly_stream;

Simulation simulator;

double start, endtime;

int sampler_count;

using namespace std;

ostream &getDelim(ostream &stream)
{
	stream << params.delim;
	return stream;
}

void write_results_header(filenames_structure &filenames, string calc_param)
{
	if (params.mode == "resample")	//legacy: processing_flag == 0
	{
		results_stream << "Algorithm" << getDelim;
		results_stream << "Configuration" << getDelim;
		results_stream << "RS" << getDelim;
		results_stream << "Index" << getDelim;
	}
	results_stream << "obj-cost";
	if(calc_param == "drought_full") results_stream << getDelim << "DroughtTransfersCost";
	results_stream << getDelim << "obj-rel";
	results_stream << getDelim << "obj-crel";
	results_stream << getDelim << "obj-surplus";
	if (params.model_case > 1)
	{
		results_stream << getDelim << "obj-costvar" << getDelim << "obj-drop";
		if (params.model_case > 2)
		{
			results_stream << getDelim << "obj-numleases";
		}
	}
	results_stream << getDelim << "obj-resilience" << getDelim << "fail-vol";
	results_stream << getDelim << "obj-vulnerability" << getDelim << "max-failureperiods";
	//JRK removed these constraint violation reporting header because the values are
	//no longer actually plotted.
	//results_stream << getDelim <<  "constr-rel" << getDelim << "constr-crel";
	//if (params.model_case > 1) results_stream << getDelim << "constr-cvar";
	//results_stream << getDelim << "init_rights";
	results_stream << getDelim << "rights";
	if (params.model_case > 1)
	{
		results_stream << getDelim << "xi";
		results_stream << getDelim << "options_low";
		results_stream << getDelim << "options_high";
		results_stream << getDelim << "alpha2";
		results_stream << getDelim << "beta2";
		if (params.model_case > 2)
		{
			results_stream << getDelim << "alpha" << getDelim << "beta";
		}
	}
	results_stream << getDelim << "ifri";
	char buffer[50];
	for (int header_it = 1; header_it <= params.NumberYears; header_it++)
	{
		sprintf(buffer, "rel-%d", header_it);
		results_stream << getDelim << buffer;
	}
	results_stream << getDelim << "rel-avg";
	for (int header_it = 1; header_it <= params.NumberYears; header_it++)
	{
		sprintf(buffer, "crel-%d", header_it);
		results_stream << getDelim << buffer;
	}
	results_stream << getDelim << "crel-avg";
	if (params.model_case > 1)
	{
		for (int header_it = 1; header_it <= params.NumberYears; header_it++)
		{
			sprintf(buffer, "drop-%d", header_it);
			results_stream << getDelim << buffer;
		}
	}
	results_stream << getDelim << "drop-avg";
	for (int header_it = 1; header_it <= params.NumberYears; header_it++)
	{
		sprintf(buffer, "totcost-%d", header_it);
		results_stream << getDelim << buffer;
	}
	results_stream << getDelim << "totcost-avg";
	for (int header_it = 1; header_it <= params.NumberYears; header_it++)
	{
		sprintf(buffer, "leasecost-%d", header_it);
		results_stream << getDelim << buffer;
	}
	results_stream << getDelim << "leasecost-avg";
	results_stream << getDelim << "costpercent-rights";
	if (params.model_case > 1)
	{
		results_stream << getDelim << "costpercent-options";
		if (params.model_case > 2)
		{
			results_stream << getDelim << "costpercent-leases";
		}
	}
	if (params.model_case > 1)
	{
		for (int header_it = 1; header_it <= params.NumberYears; header_it++)
		{
			sprintf(buffer, "num-high-opt-%d", header_it);
			results_stream << getDelim << buffer;
		}
		results_stream << getDelim << "num-high-opt-avg";
		for (int header_it = 1; header_it <= params.NumberYears; header_it++)
		{
			sprintf(buffer, "num-Nx-%d", header_it);
			results_stream << getDelim << buffer;
		}
		results_stream << getDelim << "num-Nx-avg";
		for (int header_it = 1; header_it <= params.NumberYears; header_it++)
		{
			sprintf(buffer, "avg-Nx-%d", header_it);
			results_stream << getDelim << buffer;
		}
		results_stream << getDelim << "avg-Nx-avg";
	}
	if (params.model_case > 2)
	{
		for (int header_it = 1; header_it <= params.NumberYears; header_it++)
		{
			sprintf(buffer, "avg-Nl-%d", header_it);
			results_stream << getDelim << buffer;
		}
		results_stream << getDelim << "avg-Nl-avg";
	}
	for (int header_it = 1; header_it <= params.NumberYears; header_it++)
	{
		sprintf(buffer, "endwtr-%d", header_it);
		results_stream << getDelim << buffer;
	}
	results_stream << getDelim << "endwtr-avg";
	//results_stream << getDelim << "drought-initrights";
	
	//The code was not meant to be run with "synchronous sampling" AND the contiguous drought at
	//the same time.  This snippet only outputs the continguous drought if Sobol is turned on and
	//synchronous sampling is turned off.  For different combinations of these modes, this code should
	//be tested further.
	
	//On 03-16-2015, we began testing this further indeed!
	//let's just output everything because we think that it will work ok.
	//but I wonder what happens when the init_lrgv gets called in the drought itself?
	//old if: if (params.mode == "sobol" && !params.sync_flag) //changed from "or" to "and", 11-26-2010
	//cout << "calc_param is" << calc_param << endl;
	//if (calc_param == "combined") //new version on 03-16-2015
	//{
		results_stream << getDelim << "drought-transcost";
		results_stream << getDelim << "drought-vulnerability";
		results_stream << getDelim << "drought-maxfailureperiods";
		results_stream << getDelim << "drought-totalperiods";
		for (int header_it = 1; header_it <= 12; header_it++)
		{
			sprintf(buffer, "drought-avwater%d", header_it);
			results_stream << getDelim << buffer;
			sprintf(buffer, "drought-Nr%d", header_it);
			results_stream << getDelim << buffer;
			sprintf(buffer, "drought-leases%d", header_it);
			results_stream << getDelim << buffer;
			sprintf(buffer, "drought-demand%d", header_it);
			results_stream << getDelim << buffer;
			sprintf(buffer, "drought-monthlysurplus%d", header_it);
			results_stream << getDelim << buffer;
		}
		results_stream << getDelim << "drought-No";
		results_stream << getDelim << "drought-Nx" << endl;
	//}
	//else
	//{
	//	results_stream << endl;
	//}
	
	//JRK: note the commented out section should make this work for now.
	//but eventually we should update the logic of how these things work with the new modes
	return;
}
void write_monthly_header(filenames_structure &filenames)
{
	char buffer[50];
	for (int header_it = 1; header_it <= (12*params.NumberYears); header_it++)
	{
		sprintf(buffer, "failurerate-%d", header_it);
		monthly_stream << buffer;
		if (header_it != (12*params.NumberYears))
		{
			monthly_stream << getDelim;
		}
	}
	for (int header_it = 1; header_it <= (12*params.NumberYears); header_it++)
	{
		sprintf(buffer, "avwater-%d", header_it);
		monthly_stream << getDelim << buffer;
	}
	for (int header_it = 1; header_it <= (12*params.NumberYears); header_it++)
	{
		sprintf(buffer, "vulnerability-%d", header_it);
		monthly_stream << getDelim << buffer;
	}
	for (int header_it = 1; header_it <= (12*params.NumberYears); header_it++)
	{
		sprintf(buffer, "leases-%d", header_it);
		monthly_stream << getDelim << buffer;
	}
	monthly_stream << endl;
	return;
}
void write_monthly_output()
{
	double temp_sum = 0.0;
	double placeholder = 0.0;
	
	//report failure record
	for (int year_it = 0; year_it < params.NumberYears; year_it++)
	{
		for (int month_it = 0; month_it < 12; month_it++)
		{
			placeholder = average_array_colwise(monthly_tracker[month_it].total_failures, params.NumberSims, params.NumberYears, year_it);
			if (placeholder == 0)
			{
				monthly_stream.unsetf(ios::fixed);
			}
			else 
			{
				monthly_stream.setf(ios::fixed);
				monthly_stream << setprecision(6);
			}
			monthly_stream << placeholder << getDelim;
		}
	}

	//report available water 
	monthly_stream.setf(ios::fixed);
	monthly_stream << setprecision(1);
	for (int year_it = 0; year_it < params.NumberYears; year_it++)
	{
		for (int month_it = 0; month_it < 12; month_it++)
		{
			placeholder = average_array_colwise(monthly_tracker[month_it].av_water, params.NumberSims, params.NumberYears, year_it);
			monthly_stream << placeholder << getDelim;
		}
	}
	monthly_stream.unsetf(ios::fixed);

	//report vulnerability
	for (int year_it = 0; year_it < params.NumberYears; year_it++)
	{
		for (int month_it = 0; month_it < 12; month_it++)
		{
			temp_sum = 0.0;
			for (int sims_it = 0; sims_it < params.NumberSims; sims_it++) //fixed bug, 2009-06-15_1 version
			{
				placeholder = monthly_tracker[month_it].random_monthly_demand[sims_it][year_it]-monthly_tracker[month_it].av_water[sims_it][year_it];
				if (placeholder > 0)
					temp_sum = temp_sum + placeholder;
			}

			if (temp_sum == 0)
			{
				monthly_stream.unsetf(ios::fixed);
			}
			else 
			{
				monthly_stream.setf(ios::fixed);
				monthly_stream << setprecision(6);
			}

			monthly_stream << (1/((double)params.NumberSims))*temp_sum;
			
			if (month_it == 11)
			{ 
				if (year_it != (params.NumberYears-1))
				{
					monthly_stream << getDelim;
				}
			}
			else monthly_stream << getDelim;
		}
	}
	monthly_stream.unsetf(ios::fixed);
	//report leases
	monthly_stream.setf(ios::fixed);
	monthly_stream << setprecision(1);
	for (int year_it = 0; year_it < params.NumberYears; year_it++)
	{
		for (int month_it = 0; month_it < 12; month_it++)
		{
			placeholder = average_array_colwise(monthly_tracker[month_it].total_monthly_leases, params.NumberSims, params.NumberYears, year_it);
			monthly_stream << placeholder << getDelim;
		}
	}
	monthly_stream.unsetf(ios::fixed);

	monthly_stream << endl;
	return;
}
void hydro_read(string hydro_filename)
{

	//DEBUG 10-01
	//cout << "Welcome to the hydro read function!!" << endl;
	//END DEBUG

	/* Usage: Reads hydrology data for random sampling.
	Transforms it according to Roulette Weighted Sampling */

	ifstream in;
	double junkd;

	//Read Initial Rights
	in.open("initrights.txt", ios::in);
	if(!in) {cout << "Failed to open init rights file!" << endl; exit(1);}
	general_1d_allocate(params.initrights_vec, 50000);
	for (int i = 0; i < 49999; i++)
	{
		in >> params.initrights_vec[i];
	}
	in.close();
	in.clear();

	//Read hydro file. Places data into hydro_sets

	openfile(in, hydro_filename);
	while(1)
	{
		if (in.eof()) break;
		else
		{
			char junk[255];
			in.getline(junk,255);

			//read inflow
			for (int month_it = 0; month_it<12; month_it++)
			{
				for (int hist_it = 0; hist_it<33; hist_it++)
				{
					in >> junkd;
					hydro_sets[month_it].inf.push_back(junkd);
				}
				in.getline(junk,255);
			}
			in.getline(junk,255);

			//read los
			for (int month_it = 0; month_it<12; month_it++)
			{
				for (int hist_it = 0; hist_it<33; hist_it++)
				{
					in >> junkd;
					hydro_sets[month_it].los.push_back(junkd);
				}
				in.getline(junk,255);
			}
			in.getline(junk,255);

			//read reservoir variation (this value
			//can be negative or positive)
			for (int month_it = 0; month_it<12; month_it++)
			{
				for (int hist_it = 0; hist_it<31; hist_it++)
				{
					in >> junkd;
					hydro_sets[month_it].res_var.push_back(junkd);
				}
				in.getline(junk,255);
			}
			in.getline(junk,255);

			//read new water (NW)
			for (int month_it = 0; month_it<12; month_it++)
			{
				for (int hist_it = 0; hist_it<33; hist_it++)
				{
					in >> junkd;
					hydro_sets[month_it].NW.push_back(junkd);
				}
				in.getline(junk,255);
			}
		}
		in.close();

		return;
	}
}


void lease_read(string lease_filename)
{

	ifstream read_strings;

	read_strings.open(lease_filename.c_str(), ios::in);

	if(!read_strings)
	{
		cout << "Failed to open input file: " << lease_filename << "!" << endl;
		exit(1);
	}

	else
	{

		//junk the headerline
		char junk[255];
		read_strings.getline(junk,255);

		string string_temp;

		//read low reservoir lease price list
		
		for (int month_it = 0; month_it<12; month_it++) //loop through the months
		{
			//get a whole line from the read_strings ifstream and put it in string_temp
			getline(read_strings, string_temp);

			//open up a string stream which will read string_temp	
			istringstream iss(string_temp);
			
			//declare a junk variable to read in from the string
			double junk_value;

			while (iss >> junk_value) //as long as there are values to read...
			{
				//we place them in our structure
				lease_sets[month_it].low_res.push_back(junk_value);
			}

			iss.clear();

			//after we run out of stuff to read, we loop through again

		} //end of for loop for the months

		read_strings.getline(junk,255);

		for (int month_it = 0; month_it<12; month_it++) //loop through the months
		{
			//get a whole line from the read_strings ifstream and put it in string_temp
			getline(read_strings, string_temp);

			//open up a string stream which will read string_temp	
			istringstream iss(string_temp);
			
			//declare a junk variable to read in from the string
			double junk_value;

			while (iss >> junk_value) //as long as there are values to read...
			{
				//we place them in our structure
				lease_sets[month_it].high_res.push_back(junk_value);
			}

			iss.clear();

			//after we run out of stuff to read, we loop through again

		} //end of for loop for the months

	} //end of the else brackets

	read_strings.close();

	return;

}

void demand_read(string demand_filename)
{
	ifstream read_strings;

	read_strings.open(demand_filename.c_str(), ios::in);

	if(!read_strings)
	{
		cout << "Failed to open input file: " << demand_filename << "!" << endl;
		exit(1);
	}

	else
	{

		//junk the headerline
		char junk[255];
		read_strings.getline(junk,255);

		string string_temp;
		
		for (int month_it = 0; month_it<12; month_it++) //loop through the months
		{
			//get a whole line from the read_strings ifstream and put it in string_temp
			getline(read_strings, string_temp);

			//open up a string stream which will read string_temp	
			istringstream iss(string_temp);
			
			//declare a junk variable to read in from the string
			double junk_value;

			while (iss >> junk_value) //as long as there are values to read...
			{
				//we place them in our structure
				demand_sets[month_it].demand.push_back(junk_value);
			}

			iss.clear();

			//after we run out of stuff to read, we loop through again

		} //end of for loop for the months

		read_strings.getline(junk,255);

		for (int month_it=0; month_it<12; month_it++)
		{
			double junk_value;

			read_strings >> junk_value;

			demand_sets[month_it].mean = junk_value;

			read_strings.getline(junk,255);

		}

		read_strings.getline(junk,255);

		for (int month_it=0; month_it<12; month_it++)
		{
			double junk_value;

			read_strings >> junk_value;

			demand_sets[month_it].std_dev = junk_value;

			read_strings.getline(junk,255);

		}

		read_strings.close();

	} //end of the else brackets

	return;

}

void control_read(filenames_structure &filenames)
{
	ifstream in;
	in.open(filenames.control.c_str(), ios::in);

	//TODO add parameter defaults in case someone
	//forgets a parameter

	char junk[1000];

	if (!in)
	{
		cout << "Failed to open input file: " << filenames.control << "!" << endl;
		exit(1);
	}
	else
	{
		//general_1d_allocate(params.ifri_dist, 255); //10-09-2012 changed to std::string
		while(!in.eof())
		{
			in >> junk; //character array for the computer to read
			//for each of these lines, if it finds the tag it reads the next value
			if (!strcmp(junk, "<transform>")) in >> params.dectransform_flag;
			//else if (!strcmp(junk, "<parameter_continuity_flag>")) in >> params.paramcheck_flag;
			else if (!strcmp(junk, "<discretize_flag>")) in >> params.discretize_flag;
			else if (!strcmp(junk, "<rights>")) in >> strategy.Nrt;
			else if (!strcmp(junk, "<options_low>")) in >> strategy.No_low;
			else if (!strcmp(junk, "<options_high>")) in >> strategy.No_high;
			else if (!strcmp(junk, "<xi>")) in >> strategy.xi;
			else if (!strcmp(junk, "<alpha2>")) in >> strategy.alpha2;
			else if (!strcmp(junk, "<beta2>")) in >> strategy.beta2;
			else if (!strcmp(junk, "<alpha>")) in >> strategy.alpha;
			else if (!strcmp(junk, "<beta>")) in >> strategy.beta;
			else if (!strcmp(junk, "<initial_rights_dist>")) in >> params.ifri_dist;
			else if (!strcmp(junk, "<initial_rights_param1>")) in >> params.ifri_param1;
			else if (!strcmp(junk, "<initial_rights_param2>")) in >> params.ifri_param2;
			else if (!strcmp(junk, "<critical_reliability_threshold>")) in >> params.critical_reliability_threshold;
			else if (!strcmp(junk, "<demand_growth_factor>")) in >> params.DemGrowthFactor;
			else if (!strcmp(junk, "<initial_reservoir_volume>")) in >> params.iRo;
			else if (!strcmp(junk, "<option_exercise_month>")) in >> params.OExerciseMonth;
			else if (!strcmp(junk, "<options_price>")) in >> params.Po;
			else if (!strcmp(junk, "<rights_price>")) in >> params.Pr;
			else if (!strcmp(junk, "<strike_price>")) in >> params.Px;
			else if (!strcmp(junk, "<problem_case>")) in >> params.model_case;
			else if (!strcmp(junk, "<monte_carlo>"))
			{
				int junkSims;
				in >> junkSims;

				//Since the 2014-02-10 version of the code, the value on the command line
				//supersedes whatever is in the control file. We used to warn about this,
				//but that caused a problem with the standard-io version.  Basically we had
				//to strip anything but the most fatal errors from being reported to the
				//command line.

				if (params.NumberSims == -1) //In other words, if the NumberSims hasn't been set yet...
				{
					//set it.
					params.NumberSims = junkSims;
				}	
			}
			else if (!strcmp(junk, "<number_years>")) in >> params.NumberYears;
			else if (!strcmp(junk, "<synchronous_sampling>")) in >> params.sync_flag;
			else if (!strcmp(junk, "<calendar_run>")) in >> params.single_flag;
			else if (!strcmp(junk, "<calendar_date>")) in >> params.single_year;
			else if (!strcmp(junk, "<instream_loss>")) in >> params.in_loss;
			else if (!strcmp(junk, "<reservoir_threshold>")) in >> params.reservoir_threshold;
			else if (!strcmp(junk, "<reservoir_critical_level>")) in >> params.ReservoirCriticalLevel;
			else if (!strcmp(junk, "<total_water_rights>")) in >> params.TWR;
			else if (!strcmp(junk, "<max_rights>")) in >> params.Nrt_max;
			else if (!strcmp(junk, "<max_options>")) in >> params.No_max;
			else if (!strcmp(junk, "<output_timing>")) in >> params.timing_flag;
			else if (!strcmp(junk, "<output_yearly>")) in >> params.results_flag;
			else if (!strcmp(junk, "<output_monthly>")) in >> params.monthly_flag;
			else if (!strcmp(junk, "<output_ensemble>")) in >> params.rng_flag;
			else if (!strcmp(junk, "<output_full-sim>")) in >> params.out_flag;
			else if (!strcmp(junk, "<output_yearly_header>")) in >> params.results_header_flag;
			else if (!strcmp(junk, "<output_delim>")) in >> params.delim_flag;
			else if (!strcmp(junk, "<output_monthly_header>")) in >> params.monthly_header_flag;
			else if (!strcmp(junk, "<roulette_flag>")) in >> params.roulette_flag;
			else if (!strcmp(junk, "<inf_weight>")) in >> params.inf_weight;
			else if (!strcmp(junk, "<res_var_weight>")) in >> params.res_var_weight;			
			else if (!strcmp(junk, "<lease_weight>")) in >> params.lease_weight;
			else if (!strcmp(junk, "<demand_weight>")) in >> params.demand_weight;
			else if (!strcmp(junk, "<objectives_flag>")) in >> params.obj_flag;
			else if (!strcmp(junk, "<constraints_flag>")) in >> params.constr_flag;
			else if (!strcmp(junk, "<objectives_begin>"))
			{
				in.ignore(2000,'\n');
				
				string str_junk;
				double double_junk;

				in >> str_junk;
				do
				{
 				  if (find(obj_avail.begin(), obj_avail.end(), str_junk) == obj_avail.end()){
				    cerr << "Error! objective " << str_junk << " not recognised. Supported objectives are: ";
				    static vector<string>::const_iterator cii;
				    for(cii=obj_avail.begin(); cii!=obj_avail.end(); cii++){
				      cerr << *cii << " ";
				    }
				    cerr << endl;
				    exit(-1);
				  }
					//read name
					params.obj_names.push_back(str_junk);
					//read scaling factor
					in >> double_junk;
					params.obj_scalingfactors.push_back(double_junk);
					in >> double_junk;
					params.obj_epsilons.push_back(double_junk);
					//skip to the next line and grab the first thing in that line
					in.ignore(2000,'\n');
					in >> str_junk;
				} while(str_junk != "<objectives_end>");
			}
			else if (!strcmp(junk, "<constraints_begin>"))
			{
				in.ignore(2000,'\n');
				
				string str_junk;
				double double_junk;

				in >> str_junk;
				do
				{
 				  if (find(constr_avail.begin(), constr_avail.end(), str_junk) == constr_avail.end()){
				    cerr << "Error! constraint " << str_junk << " not recognised. Supported constraints are: ";
				    static vector<string>::const_iterator cii;
				    for(cii=constr_avail.begin(); cii!=constr_avail.end(); cii++){
				      cerr << *cii << " ";
				    }
				    cerr << endl;
				    exit(-1);
				  }
					//read name
					params.constr_names.push_back(str_junk);
					
					//read comparator
					in >> str_junk;
					params.constr_comparators.push_back(str_junk);

					//read value
					in >> double_junk;
					params.constr_values.push_back(double_junk);

					//skip to the next line and grab the first thing in that line
					in.ignore(2000,'\n');
					in >> str_junk;
				} while(str_junk != "<constraints_end>");
			}
			in.ignore(2000,'\n');
		} //end while loop
		//general_1d_allocate(params.delim, 255); //10-9-2012, removed allocation and changed delim to std::string
		if (params.delim_flag == 0)
			params.delim = ",";
		else
			params.delim = "   ";
		in.close();
		
		//9/21/2012 add check to make sure NumberSims is defined.
		if (params.NumberSims == -1)
		{
			cerr << "NumberSims not entered on command line or control file. Exiting..." << endl;
			exit(-1);
		}
			
	} //end if-statement for valid input stream

	return;
}
void hist_read(filenames_structure &filenames)
{
	control_read(filenames); //7/21/2011 moved control read up so that roulette parameters are in memory before subsequent calls
	hydro_read(filenames.hydro);
	lease_read(filenames.lease);
	demand_read(filenames.demand);
	
}

void general_debug_output (ofstream &out, double **data, int rows, int cols)
{
	//out << endl;

	for (int row_it = 0; row_it < rows; row_it++)
	{
		for (int col_it = 0; col_it < cols; col_it++)
		{
			out << data[row_it][col_it] << "   ";
		}
		out << endl;
	}
	out << endl;

	return;
}

void sets_output_rng_text(ofstream &out, double _current_sim)
{
	int NumberYears = params.NumberYears;

	out << "Simulation " << _current_sim << endl;
	//same as debug output with some minor changes
	out << "Inflows" << endl;
	
	out.setf(ios_base::fixed, ios_base::floatfield);
	out.precision(5); //for the fixed type: precision is for numbers past decimal
	//testing the width and fill commands to even up those columns:
	//out.width(30);
	//out.fill('%');
	
	for (int year_it = 0; year_it < NumberYears; year_it++)
	{
		for (int month_it = 0; month_it < 12; month_it++)
		{
			out << samples[month_it].inf[year_it] << "   ";
		}
		out << endl;
	}

	out << "Losses" << endl;

	for (int year_it = 0; year_it < NumberYears; year_it++)
	{
		for (int month_it = 0; month_it < 12; month_it++)
		{
			out << samples[month_it].los[year_it] << "   ";
		}
		out << endl;
	}

	out << "ReservoirVariation" << endl;

	for (int year_it = 0; year_it < NumberYears; year_it++)
	{
		for (int month_it = 0; month_it < 12; month_it++)
		{
			out << samples[month_it].res_var[year_it] << "   ";
		}
		out << endl;
	}

	out << "Demand" << endl;

	for (int year_it = 0; year_it < NumberYears; year_it++)
	{
		for (int month_it = 0; month_it < 12; month_it++)
		{
			out << samples[month_it].demand[year_it] << "   ";
		}
		out << endl;
	}

	out << "LeasePriceLow" << endl;

	for (int year_it = 0; year_it < NumberYears; year_it++)
	{
		for (int month_it = 0; month_it < 12; month_it++)
		{
			out << samples[month_it].lease_low_res[year_it] << "   ";
		}
		out << endl;
	}

	out << "LeasePriceHigh" << endl;

	for (int year_it = 0; year_it < NumberYears; year_it++)
	{
		for (int month_it = 0; month_it < 12; month_it++)
		{
			out << samples[month_it].lease_high_res[year_it] << "   ";
		}
		out << endl;
	}



	return;
}
