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


#ifndef _global_lrgv_h_
#define _global_lrgv_h_

using namespace std;

struct hydro_structure{
	//change to dynamically-allocated data!
	vector <double> NW;
	vector <double> los;				int *los_cdf;
	vector <double> inf;				int *inf_cdf;
	vector <double> res_var;			int *res_var_cdf;
};

struct lease_structure{
	vector <double> high_res;	int *high_res_cdf;
	vector <double> low_res;	int *low_res_cdf;
};

struct demand_structure{
	vector<double> demand;		int *demand_cdf;
	double mean;
	double std_dev;
};

struct sampled_data{
	//double* NW;
	double* los;
	double* inf;
	double* res_var;
	double* lease_high_res;
	double* lease_low_res;
	double* demand;
};

struct year_tracker_structure{

	// a collection of vectors which all have 'number_of_years' elements
	// for example, a 10 year simulation will be populated by 10 values for each
	// of these arrays.  however, there would be a different structure
	// at each iteration of the simulation (???)

	//each of these is a pointer to what will become a list with 'number_years' elements

	//double* future_Nrt;
	//double* No;
	double* portfolio_cost;
	double* end_of_year_water;
	double* annual_demand;
	double* annual_NW;
	double* option_tracker;
	double* annual_nlpl;
	double* early_NW; //???
	double* late_NW; //???
	double* late_demand; //???
	double* ToLeasePrice; //???
	double* Pl5; //???
	//perhaps these next two should have been
	//local lists...
	//double* Nx_tracker;
	//double* Po_list;
};

struct sims_years_structure{
	// these are total lists kept over every simulation, but where
	// data is stored for every year.  that is, monthly data is
	// not stored.  each pointer will point to a multi-dimensional
	// array with #sims x # years elements...
	double** yearly_purchased_leases;
	double** yearly_exercised_options;
	double** ex_option_cost_list;
	double** total_Po_list;
	double** end_of_yr_water_tracker;
	double** total_Nx_tracker;
	double** early_NW;
	double** late_NW;
	double** ToLeasePrice; //note these are defined twice (artifact from Matlab code)
	double** early_demand; //but I think the second definition is the one that sticks...
	double** late_demand;
	double** Pl5;
	double** No;
};

struct monthly_tracker_structure{
	//similar to the other trackers, but this one will be an array of
	//structures, one structure for each month.  each pointer will have
	//a multi-dimensional array with #sims x #years elements...
	double** total_monthly_leases;
	double** yearly_lease_cost;
	double** res_level_tracker;
	double** res_threshold_tracker;
	double** random_monthly_demand;
	double** losses;
	double** inflows;
	double** res_var;
	double** lease_low_res;
	double** lease_high_res;
	double** total_failures;
	double** total_cfailures;
	double** dropped_transfer_tracker;
	//(new added trackers are below)
	double** av_water;
	double** Nr;
};

struct future_structure{
	// not sure how these are going to be indexed but for the time being we will try having
	// a structure for each month, and inside that structure, there will be an array with
	// 'number years' elements

	double* demand_mean;
	double* demand_std_dev;
	double* ENr;
};

struct filenames_structure{
	string hydro;
	string lease;
	string demand;
	string control;
	string timing;
	string results;
	string rng;
	string out;
	string monthly;
	//char *drought;
};

struct param_structure{
	//control file:
	//took out the ability to specify ifri directly,
	//instead we specify a 'constant' distribution and
	//the value is stored in param1
	string ifri_dist;
	int ifri_order;
	double ifri_param1;
	double ifri_param2;
	double *initrights_vec;
	double iRo; int iRo_order;
	double DemGrowthFactor; int DemGrowthFactor_order;
	double Pr; int Pr_order;
	double Px; int Px_order;
	double Pohigh;
	double Polow;
	double Po; int Po_order;
	double TWR;
	double CityAvgAnWaterUse;
	int OExerciseMonth; int OExerciseMonth_order;
	double critical_reliability_threshold; int critical_reliability_threshold_order;
	double in_loss;
	double reservoir_threshold;
	double ReservoirCriticalLevel;
	int NumberSims;
	int NumberYears;
	int model_case;
	double Nrt_max;
	double No_max;
	int dectransform_flag;
	int sync_flag;
	int timing_flag;
	int results_flag;
	int monthly_flag;
	int monthly_header_flag;
	int rng_flag;
	int out_flag;
	int single_flag;
	int single_year;
	int paramcheck_flag;
	int discretize_flag;
	int results_header_flag;
	int delim_flag;

	//roulette sampling stuff
	int roulette_flag;
	
	//inf
	int inf_weight; int inf_weight_order;
	double inf_gamma; int inf_gamma_order;
	int inf_cdf_max;

	//los
	int los_weight; int los_weight_order;
	double los_gamma; int los_gamma_order;
	int los_cdf_max;

	//res_var
	int res_var_weight; int res_var_weight_order;
	double res_var_gamma; int res_var_gamma_order;
	int res_var_cdf_max;

	//lease
	int lease_weight; int lease_weight_order;
	double lease_high_gamma; int lease_high_gamma_order;
	int lease_high_cdf_max;
	double lease_low_gamma; int lease_low_gamma_order;
	int lease_low_cdf_max;

	//demand
	int demand_weight; int demand_weight_order;
	double demand_gamma; int demand_gamma_order;
	int demand_cdf_max;

	
	int total_possible_samples;
	int number_sampled_param;

	string delim;
	int obj_flag;
	int constr_flag;
	vector<string> obj_names;
	vector<double> obj_scalingfactors;
	vector<double> obj_epsilons; //added 2013-03-10
	vector<string> constr_names;
	vector<string> constr_comparators;
	vector<double> constr_values;	
	
	//command line:
	//int processing_flag;	//replaced by mode below
	int input_flag;
	string filename_base;
	
	//new command line:
	string mode;
};

struct strategy_structure{
	//double alpha[10];
	//double beta[10];
	//double alpha2[10];
	//double beta2[10];
	double Nrt; int Nrt_order;
	//double lambda[9];
	double xi; int xi_order;
	//double sigma;
	double No_low; int No_low_order;
	double No_high; int No_high_order;
	double alpha, beta, alpha2, beta2;
	int alpha_order, beta_order, alpha2_order, beta2_order;
};

struct timer_structure{
	double before_loop_start, before_loop_end, before_loop_sum;
	double monte_carlo_start, monte_carlo_end, monte_carlo_sum;
	double calcs_start, calcs_end, calcs_sum;
	double obj_start, obj_end, obj_sum;
};

struct year_structure{
	//contains structures for "synchronous" sampling
	sampled_data samples[12];
};

struct super_structure{
	year_structure *y;
};

struct global_reporting_structure{
	int alg;
	int config;
	int RS;
	int index;
	double vulnerability;
	int failure_periods;
	int total_periods;
	//trying out the saving of objectives for aggregation in drought run...
	double fullperiod_cost;
	double fullperiod_cvar;
	double fullperiod_rel;
	double fullperiod_crel;
};

//New drought structure
struct drought_structure{
	int NumberSims;
	int NumberYears;
	int single_year;
	int single_flag;
};

//Variables global to the LRGV simulation are declared in lrgv_input.cpp

extern hydro_structure hydro_sets[12];
extern lease_structure lease_sets[12];
extern demand_structure demand_sets[12];
extern param_structure params;
extern super_structure super;
extern drought_structure drought;

extern double start, endtime;

extern year_tracker_structure annual_tracker;
extern sims_years_structure sims_years_tracker;
extern monthly_tracker_structure monthly_tracker[12];

extern sampled_data samples[12];
extern future_structure futures[12];
extern strategy_structure strategy;

extern global_reporting_structure g;

extern timer_structure timers;
extern ofstream time_stream;
extern ofstream results_stream;
extern ofstream rng_stream;
extern ofstream out_stream;
extern ofstream monthly_stream;

extern int sampler_count;

//function prototypes for calculations (lrgv_functions.cpp)
double resilience_calc();
double failvol_calc();
void vulnerability_calc(int local_sims, int local_years);
void transfer_tracker(double *(&TransferTracker), double AvWater, double Demand);
//calls from optimization:
void init_LRGV(char** argv, string init_param);
void transform_LRGV(double* vars);
void calc_LRGV(double* vars, double* objs, double* consts, string calc_param);
void finalize_LRGV();


//function prototypes for input/output (input.cpp)
//void param_read(filenames_structure &filenames);
ostream &getDelim(ostream &stream);
void write_results_header(filenames_structure &filenames, string calc_param);
void write_monthly_header(filenames_structure &filenames);
void write_monthly_output();
void hydro_read(string hydro_filename);
void lease_read(string lease_filename);
void demand_read(string demand_filename);
void control_read(filenames_structure &filenames);
void hist_read(filenames_structure &filenames);
void general_debug_output (ofstream &out, double **data, int rows, int cols);
void sets_output_rng_text(ofstream &out, double _current_sim);

//function prototypes for output (output.cpp)
//void write_debug_header (ofstream &out);
//void years_months_debug_output (ofstream &out, double **data);
//void years_months_debug_output (ofstream &out, double data[][12]);
//void years_debug_output (ofstream &out, double *data);
////void years_debug_output (ofstream &out, double data[12]);
//void sets_output(ofstream &out, double _current_sim);
//void sets_output_rng_text(ofstream &out, double _current_sim);
//void general_debug_output (ofstream &out, double **data, int rows_1, int rows_2, int cols);

//function prototype for testing output (test_io.cpp, parameters hard-coded)
//void testing_output (ofstream &out);

//function prototypes for feasibility checking (utility.cpp)
//void alpha_feasibility(double a, double b);
//void debug_param_feasibility ();

//random number sampling (rand.cpp)
void randsample (const hydro_structure& set, sampled_data& sample, int NumberYears);
void randsample (const lease_structure& set, sampled_data& sample, int NumberYears);
void randsample (const demand_structure& set, sampled_data& sample, int NumberYears, double DemGrowthFactor);

//array math (utility.cpp)
double average_array (double *data_set, const int length);
double average_array (vector<double> data, const int length);
double average_array_colwise (double **data, int rows, int cols, int col_of_interest);
double sum(double *data, int length);
double sum(vector<double> data, int length);
double min_array(double *data, const int length);
double max_array(double *data, const int length);
double max_array(vector<double> data, const int length);
int max_index_array(double *data, const int length);
double std_dev(double *data, int length);
void zeroes(double *data, const int length);
void zeroes(int *data, const int length);
void zeroes(double data[][12], const int rows, const int cols);
void zeroes(double **data, const int rows, const int cols);
void zeroes(int data[][12], const int rows, const int cols);
void zeroes(int **data, const int rows, const int cols);
double rounding(double input);
double rounding(double input, double dec_places);

void openfile(ofstream &stream, string filename);
void openfile(ifstream &stream, string filename);
void winexit();
void winexit(string message);

//memory allocation / deallocation
//note: pointer memory is allocated by reference
//as seen in the following function calls
void general_2d_allocate(double **(&data), int rows, int cols);
void general_2d_allocate(int **(&data), int rows, int cols);
void general_1d_allocate(double *(&data), int length);
void general_1d_allocate(char *(&data), int length);
void general_1d_allocate(int *(&data), int length);
void zap(double **(&data), int rows);
void zap(int **(&data), int rows);
void zap(double *(&data));
void zap(int *(&data));
void global_trackers_allocation(int &initial_call);
//void zap(double (&data));
void zap (char *(&data));

//weighted sampling
void init_roulette();
void finalize_roulette();
int init_roulette_calcs(vector <double> &input_data, int *(&output_cdf), int highclassix, int weight);
double roulette_draw(vector <double> dataset, int *cdf);

template <class myType>
void debug_display_1d (myType a, int size)
{
	for (int i = 0; i < size; i++)
	{
		cout << a[i] << "  ";
	}
	cout << endl;
	return;
}

template <class myType>
void debug_display_2d (myType a, int size1, int size2)
{
	for (int i = 0; i < size1; i++)
	{
		for (int j = 0; j < size2; j++)
		{
			cout << a[i][j] << "  ";
		}
		cout << endl;
	}
	return;
}

template <class myType>
void debug_display_3d (myType a, int size1, int size2, int size3)
{
	cout << "Displaying a " << size1 << " by " << size2 << " by " << size3 << " matrix." << endl;
	for (int i = 0; i < size1; i++)
	{
		cout << "First dimension = " << i << ":" << endl;
		for (int j = 0; j < size2; j++)
		{
			cout << "Row " << j << ":" << endl;
			for (int k = 0; k < size3; k++)
			{
				//if (a[i][j][k] != -1.0)
				//{
					cout << a[i][j][k] << "  ";
				//}
				
			}
			cout << endl;
		}
	}
	return;
}

#endif
