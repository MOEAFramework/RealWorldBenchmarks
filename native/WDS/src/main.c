/*******************************/
/* Last updated on 25-Mar-2014 */
/* Written by Mr. Qi Wang      */
/* q.wang.pomodoro@gmail.com   */
/* Centre for Water Systems    */
/* University of Exeter        */
/*******************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "epanet2.h"
#include "moeaframework.h"
#include "models.h"

int main(int argc, char* argv[]) {
	double vars[nvars];
	double objs[nobjs];
	double constrs[nconstrs];

	// initialize the communication with the MOEA Framework
	MOEA_Init(nobjs, nconstrs);

	// open the hydraulic solver
	ENopen(inputFile,"reportFile.rpt","");
	ENresetreport();
	ENsetreport("MESSAGES NO");	
 
	// evaluate the solutions sent from the MOEA Framework
	while (MOEA_Next_solution() == MOEA_SUCCESS) {
		MOEA_Read_doubles(nvars, vars);
		test_problem(vars, NULL, NULL, objs, constrs);
		MOEA_Write(objs, constrs);
	}

	// terminate the communication with the MOMEA Framework
	MOEA_Terminate();

	// close the hydraulic solver
	ENclose();
 
	return EXIT_SUCCESS;
}