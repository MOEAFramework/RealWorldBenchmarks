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


#ifndef Simulation_h
#define Simulation_h

class Simulation{
private:
	int initial_call;
	double Nrt, No, alpha, beta, alpha2, beta2;
	double critical_reliability_threshold,DemGrowthFactor;
	double ifri, in_loss, iRo, Po, TWR;
	double Pr, Px, reservoir_threshold, ReservoirCriticalLevel;
	double Ro, fri, Nro, Nr, To, CurrentResLevel, OldReservoirLevel, Ro_diff;
	double ENrt, Te1, d1, N1o, T1, Nx;
	double NextMonthAvWater, NextMonthExpAvWater, RestSpringExpAvWater;
	double N1, Inflows, Demand, Losses, NewWater, ResVariation;
	double use, P_lease_o, P_lease, AvWater;
	int NumberSims, NumberYears, OExerciseMonth, lease_flag, options_flag;
	int single_year, single_flag; //new 2009-11-2
	double avg_hist_NW; //used to be declared on line 568

	int CurrentYear;
	int CurrentMonth;
	int CurrentSim;
	double DroppedTransfers;

	double **LeaseCost, **NumberPurchasedLeases;
	double *NxTracker, *PoList;
	int **Failures, **CFailures;

	vector<double> NewMonthAllocationList, AvWaterList;
	
	double AnnualNewWateri[12];
	double ExpAlloRestYear[10], ExpYearDemand[11];
	double AnnualSupplyi[12], MonthlyWaterUsageList[12];
	double ReservoirList[12];

	double *TransferTracker;
	double temp_transfer_sum; //used to be declared on line 908

	double temp_sum_f, temp_mean_f, temp_sum_cf, temp_mean_cf, lease_cost_sum;
	double mean_Reliability, min_Reliability, sum_CReliability, min_CReliability, nthpercentile, uqpercentile, lqpercentile;
	double dropped_sum, TotalDroppedTransfers, UltimateTotalAvgCost;
	double number_transfers_sum, TotalNumberTransfers, yearly_lease_sum;
	double meanVARmean, ObjectiveVAR;
	double results_sum, temp_double;
	int high_count, low_count, highlow_error;

	double **FailureRecord;
	double **CFailureRecord;
	double *Reliability;
	double *CReliability;
	double **AverageMonthlyLeaseCost;
	double *AverageYearlyLeaseCost;
	double **TotalYearlyLeaseCost;
	double **TotalAnnualCosts;
	double **PermRightCosts;
	double *AverageAnnualCosts;
	double *StdDevInput; //input for StdDev function
	double *StdDevCost; //output from StdDev function
	double *cost_sorting_array;
	double **SortedTotalAnnualCosts;
	double **varvector;
	double *VAR; //the list of 95th percentile cost for each year
	double *CVAR; //contingent value at risk...the mean of the varvector
	double *VARmean;
	double ObjectiveCost;
	double **YearlyDroppedTransfers;
	double *DroppedTransfersVector;
	double **YearlyNumberTransfers;
	double *NumberTransfersVector;
	double EndofYearWaterObjective;

	double *obj, *constr, *xreal;
public:
	Simulation();
	~Simulation();
	void calc_LRGV(double* vars, double* objs, double* consts, string calc_param);
	//void setInitialCall(int val);
};

extern Simulation simulator;

#endif
