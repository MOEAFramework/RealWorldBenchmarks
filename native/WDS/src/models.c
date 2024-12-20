/*******************************/
/* Last updated on 25-Mar-2014 */
/* Written by Mr. Qi Wang      */
/* q.wang.pomodoro@gmail.com   */
/* Centre for Water Systems    */
/* University of Exeter        */
/*******************************/

/* Test problem definitions */
#include <math.h>
#include "models.h"
#include "epanet2.h"

int nobjs = 2;
int nconstrs = 1;

#ifdef TRN //#01
char* inputFile = "TRN.inp";
int nvars = 8;

void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
	const float diameterList[8]={152.0, 203.0, 254.0, 305.0, 356.0, 407.0, 458.0, 509.0};
	const float newPipeUnitCost[8]={49.54, 63.32, 94.82, 132.87, 170.93, 194.88, 232.94, 264.1};
	const float cleanPipeUnitCost[3]={60.7, 55.12, 55.12};
	const float demand1[10]={12.62,12.62,0,18.93,18.93,18.93,12.62,18.93,18.93,12.62};
	const float demand2[10]={12.62,12.62,0,18.93,82.03,18.93,12.62,18.93,18.93,12.62};
	const float demand3[10]={12.62,12.62,0,18.93,18.93,18.93,12.62,18.93,18.93,50.48};
	const float minPressure1[10]={28.18,17.61,17.61,35.22,35.22,35.22,35.22,35.22,35.22,35.22};
	const float minPressure2[10]={14.09,14.09,14.09,14.09,10.57,14.09,14.09,14.09,14.09,14.09};
	const float minPressure3[10]={14.09,14.09,14.09,14.09,14.09,14.09,14.09,14.09,14.09,10.57};
	const float originalRoughness[3]={75.0, 80.0, 80.0};
	float totalCost=0.0;
	float networkResilience=0.0;
	int errorCode=-1;
	int errorSum=0;
	int i=0;
	int j=0;
	int k=0;
	float retrievedData=0.0;
	float juncDemand[10], juncHead[10], juncU[10], minHead[10], reservoirDischarge[2], reservoirHead[2];
	float minResilience=1;
	float pressureViolation=0.0;
	int fromNode[17], toNode[17];
	int nPipe=0;
	int pipeIndex[17];
	float sumDiameter=0.0;
	float maxDiameter=0.0;
	float X=0.0;
	float Xmax=0.0;
	int newPipeDiameterIndex[5];
	float newPipeDiameter[5];
	int existingPipeOption[3];
	int duplicatePipeDiameterIndex[3];
	float duplicatePipeDiameter[3];

	// open the hydraulic solver
    // ENopen("TRN.inp","reportFile.rpt","");
	// ENresetreport();
	// ENsetreport("MESSAGES NO");

	// initialise variables
	// set pipe diameters and compute total cost
	for (i=0; i<5; i++)
	{
		newPipeDiameterIndex[i]=floor(xreal[i]);
		newPipeDiameter[i]=diameterList[newPipeDiameterIndex[i]];
		ENsetlinkvalue(9+1+i, EN_DIAMETER, newPipeDiameter[i]);
		ENgetlinkvalue(9+1+i, EN_LENGTH, &retrievedData);
		totalCost+=newPipeUnitCost[newPipeDiameterIndex[i]]*retrievedData;
	}
	for (i=0; i<3; i++)
	{
		existingPipeOption[i]=floor(xreal[5+i]);
		if (existingPipeOption[i]==0) // leaving alone
		{
			ENsetlinkvalue(1+i, EN_ROUGHNESS, originalRoughness[i]);
			ENsetlinkvalue(9+5+1+i, EN_DIAMETER, 0.0001);
			ENsetlinkvalue(9+5+1+i, EN_INITSTATUS, 0);
			totalCost+=0.0;
		}
		else if (existingPipeOption[i]==1) // cleaning
		{
			ENsetlinkvalue(1+i, EN_ROUGHNESS, 120.0);
			ENsetlinkvalue(9+5+1+i, EN_DIAMETER, 0.0001);
			ENsetlinkvalue(9+5+1+i, EN_INITSTATUS, 0);
			ENgetlinkvalue(1+i, EN_LENGTH, &retrievedData);
			totalCost+=cleanPipeUnitCost[i]*retrievedData;
		}
		else if (existingPipeOption[i]>=2) // duplication
		{
			duplicatePipeDiameterIndex[i]=existingPipeOption[i]-2;
			duplicatePipeDiameter[i]=diameterList[duplicatePipeDiameterIndex[i]];
			ENsetlinkvalue(1+i, EN_ROUGHNESS, originalRoughness[i]);
			ENsetlinkvalue(9+5+1+i, EN_DIAMETER, duplicatePipeDiameter[i]);
			ENsetlinkvalue(9+5+1+i, EN_INITSTATUS, 1);
			ENgetlinkvalue(9+5+1+i, EN_LENGTH, &retrievedData);
			totalCost+=newPipeUnitCost[duplicatePipeDiameterIndex[i]]*retrievedData;
		}
	}
	
	// set demand pattern 1
	for (i=0; i<10; i++)
	{
		ENsetnodevalue(1+i, EN_BASEDEMAND, demand1[i]);
	}

	// run the hydraulic simulation
	errorCode=ENsolveH();

	// retrieve static and simulated values of each node
	for (i=0; i<10; i++)
	{
		ENgetnodevalue(1+i, EN_ELEVATION, &retrievedData);
		minHead[i]=retrievedData+minPressure1[i];
		ENgetnodevalue(1+i, EN_DEMAND, &retrievedData);
		juncDemand[i]=retrievedData;
		ENgetnodevalue(1+i, EN_HEAD, &retrievedData);
		juncHead[i]=retrievedData;
		ENgetnodevalue(1+i, EN_PRESSURE, &retrievedData);
		if (retrievedData<minPressure1[i])
		{
			pressureViolation+=(minPressure1[i]-retrievedData);
		}
	}
	for (i=0; i<2; i++)
	{
		ENgetnodevalue(10+1+i, EN_DEMAND, &retrievedData);
		reservoirDischarge[i]=retrievedData;
		ENgetnodevalue(10+1+i, EN_HEAD, &retrievedData);
		reservoirHead[i]=retrievedData;	
	}

	// calculate the uniformity of each junction
	for (i=0; i<17; i++)
	{
		ENgetlinknodes(1+i, &fromNode[i], &toNode[i]);
	}
	for (i=1; i<11; i++)
	{
		nPipe=0;
		maxDiameter=0.0;
		sumDiameter=0.0;
		for (j=0; j<17; j++)
		{
			if (fromNode[j]==i)
			{
				ENgetlinkvalue(1+j, EN_DIAMETER, &retrievedData);
				if (retrievedData>0.01)
				{
					nPipe+=1;
					pipeIndex[nPipe-1]=1+j;
				}
			}
			if (toNode[j]==i)
			{
				ENgetlinkvalue(1+j, EN_DIAMETER, &retrievedData);
				if (retrievedData>0.01)
				{
					nPipe+=1;
					pipeIndex[nPipe-1]=1+j;
				}
			}
		}
		for (k=0; k<nPipe; k++)
		{
			ENgetlinkvalue(pipeIndex[k], EN_DIAMETER, &retrievedData);
			sumDiameter+=retrievedData;
			if (retrievedData>maxDiameter)
			{
				maxDiameter=retrievedData;
			}
		}
		juncU[i-1]=sumDiameter/(nPipe*maxDiameter);
	}

	// compute the network resilience for demand pattern 1
	for (i=0; i<10; i++)
	{
		X+=juncU[i]*juncDemand[i]*(juncHead[i]-minHead[i]);
		Xmax-=juncDemand[i]*minHead[i];
	}
	for (i=0; i<2; i++)
	{
		Xmax+=-1*reservoirDischarge[i]*reservoirHead[i];
	}
	networkResilience=X/Xmax;
	if (minResilience>networkResilience)
	{
		minResilience=networkResilience;
	}
	errorSum+=errorCode;

	// set demand pattern 2
	for (i=0; i<10; i++)
	{
		ENsetnodevalue(1+i, EN_BASEDEMAND, demand2[i]);
	}

	// run the hydraulic simulation
	errorCode=ENsolveH();

	// retrieve static and simulated values of each node
	for (i=0; i<10; i++)
	{
		ENgetnodevalue(1+i, EN_ELEVATION, &retrievedData);
		minHead[i]=retrievedData+minPressure2[i];
		ENgetnodevalue(1+i, EN_DEMAND, &retrievedData);
		juncDemand[i]=retrievedData;
		ENgetnodevalue(1+i, EN_HEAD, &retrievedData);
		juncHead[i]=retrievedData;
		ENgetnodevalue(1+i, EN_PRESSURE, &retrievedData);
		if (retrievedData<minPressure2[i])
		{
			pressureViolation+=(minPressure2[i]-retrievedData);
		}
	}
	for (i=0; i<2; i++)
	{
		ENgetnodevalue(10+1+i, EN_DEMAND, &retrievedData);
		reservoirDischarge[i]=retrievedData;
		ENgetnodevalue(10+1+i, EN_HEAD, &retrievedData);
		reservoirHead[i]=retrievedData;	
	}

	// compute the network resilience for demand pattern 2
	X=0.0;
	Xmax=0.0;
	for (i=0; i<10; i++)
	{
		X+=juncU[i]*juncDemand[i]*(juncHead[i]-minHead[i]);
		Xmax-=juncDemand[i]*minHead[i];
	}
	for (i=0; i<2; i++)
	{
		Xmax+=-1*reservoirDischarge[i]*reservoirHead[i];
	}
	networkResilience=X/Xmax;
	if (minResilience>networkResilience)
	{
		minResilience=networkResilience;
	}
	errorSum+=errorCode;

	// set demand pattern 3
	for (i=0; i<10; i++)
	{
		ENsetnodevalue(1+i, EN_BASEDEMAND, demand3[i]);
	}

	// run the hydraulic simulation
	errorCode=ENsolveH();

	// retrieve static and simulated values of each node
	for (i=0; i<10; i++)
	{
		ENgetnodevalue(1+i, EN_ELEVATION, &retrievedData);
		minHead[i]=retrievedData+minPressure3[i];
		ENgetnodevalue(1+i, EN_DEMAND, &retrievedData);
		juncDemand[i]=retrievedData;
		ENgetnodevalue(1+i, EN_HEAD, &retrievedData);
		juncHead[i]=retrievedData;
		ENgetnodevalue(1+i, EN_PRESSURE, &retrievedData);
		if (retrievedData<minPressure3[i])
		{
			pressureViolation+=(minPressure3[i]-retrievedData);
		}
	}
	for (i=0; i<2; i++)
	{
		ENgetnodevalue(10+1+i, EN_DEMAND, &retrievedData);
		reservoirDischarge[i]=retrievedData;
		ENgetnodevalue(10+1+i, EN_HEAD, &retrievedData);
		reservoirHead[i]=retrievedData;	
	}

	// compute the network resilience for demand pattern 3
	X=0.0;
	Xmax=0.0;
	for (i=0; i<10; i++)
	{
		X+=juncU[i]*juncDemand[i]*(juncHead[i]-minHead[i]);
		Xmax-=juncDemand[i]*minHead[i];
	}
	for (i=0; i<2; i++)
	{
		Xmax+=-1*reservoirDischarge[i]*reservoirHead[i];
	}
	networkResilience=X/Xmax;
	if (minResilience>networkResilience)
	{
		minResilience=networkResilience;
	}
	errorSum+=errorCode;

	// close the hydraulic solver
	// ENclose();

	// report the objectives and constraint violation
    obj[0]=totalCost/1000000.0; // $ => $ MM
    obj[1]=minResilience; // maximized
	constr[0]=-(pressureViolation+errorSum);
    return;
}
#endif

#ifdef TLN //#02
char* inputFile = "TLN.inp";
int nvars = 8;

void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
	const float diameterList[14]={1.0, 2.0, 3.0, 4.0, 6.0, 8.0, 10.0, 12.0, 14.0, 16.0, 18.0, 20.0, 22.0, 24.0};
	const float unitCost[14]={2.0, 5.0, 8.0 ,11.0, 16.0, 23.0, 32.0, 50.0, 60.0, 90.0, 130.0, 170.0, 300.0, 550.0};
	float totalCost, networkResilience;
	int errorCode, i, j, k;
	float retrievedData, juncDemand[6], juncHead[6], juncPressure[6], juncU[6], reservoirDischarge[1], reservoirHead[1], pipeDiameter[8];
	float minHead[6], minPressure, pressureViolation;
	int pipeSetting[8];
	int fromNode[8], toNode[8];
	int nPipe, pipeIndex[8];
	float sumDiameter, maxDiameter;
	float X, Xmax;
	
	// open the hydraulic solver
    // ENopen("TLN.inp","reportFile.rpt","");
	// ENresetreport();
	// ENsetreport("MESSAGES NO");
	
	// initialise variables
	errorCode=-1;
	totalCost=0.0;
	networkResilience=-1.0;
	minPressure=30.0;
	pressureViolation=0.0;
	X=0.0;
	Xmax=0.0;
	
	// set pipe diameters and compute total cost
	for (i=0; i<8; i++)
	{
		pipeSetting[i]=floor(xreal[i]);
		pipeDiameter[i]=25.4*diameterList[pipeSetting[i]]; // in. => mm
		ENsetlinkvalue(1+i, EN_DIAMETER, pipeDiameter[i]);
		ENgetlinkvalue(1+i, EN_LENGTH, &retrievedData);
		totalCost+=unitCost[pipeSetting[i]]*retrievedData;
	}
	
	// run the hydraulic simulation
	errorCode=ENsolveH();

	// retrieve static and simulated values of each node
	for (i=0; i<6; i++)
	{
		ENgetnodevalue(1+i, EN_DEMAND, &retrievedData);
		juncDemand[i]=retrievedData;
		ENgetnodevalue(1+i, EN_HEAD, &retrievedData);
		juncHead[i]=retrievedData;
		ENgetnodevalue(1+i, EN_PRESSURE, &retrievedData);
		juncPressure[i]=retrievedData;
		if (juncPressure[i]<minPressure)
		{
			pressureViolation+=(minPressure-juncPressure[i]);
		}
		ENgetnodevalue(1+i, EN_ELEVATION, &retrievedData);
		minHead[i]=retrievedData+minPressure;
	}
	for (i=0; i<1; i++)
	{
		ENgetnodevalue(6+1+i, EN_DEMAND, &retrievedData);
		reservoirDischarge[i]=retrievedData;
		ENgetnodevalue(6+1+i, EN_HEAD, &retrievedData);
		reservoirHead[i]=retrievedData;
	}
	
	// calculate the uniformity of each junction
	for (i=0; i<8; i++)
	{
		ENgetlinknodes(1+i, &fromNode[i], &toNode[i]);
	}
	for (i=1; i<7; i++)
	{
		nPipe=0;
		maxDiameter=0;
		sumDiameter=0;
		for (j=0; j<8; j++)
		{
			if (fromNode[j]==i)
			{
				ENgetlinkvalue(1+j, EN_DIAMETER, &retrievedData);
				if (retrievedData>0.01)
				{
					nPipe+=1;
					pipeIndex[nPipe-1]=1+j;
				}
			}
			if (toNode[j]==i)
			{
				ENgetlinkvalue(1+j, EN_DIAMETER, &retrievedData);
				if (retrievedData>0.01)
				{
					nPipe+=1;
					pipeIndex[nPipe-1]=1+j;
				}
			}
		}
		for (k=0; k<nPipe; k++)
		{
			ENgetlinkvalue(pipeIndex[k], EN_DIAMETER, &retrievedData);
			sumDiameter+=retrievedData;
			if (retrievedData>maxDiameter)
			{
				maxDiameter=retrievedData;
			}
		}
		juncU[i-1]=sumDiameter/(nPipe*maxDiameter);
	}

	// compute the network resilience
	for (i=0; i<6; i++)
	{
		X+=juncU[i]*juncDemand[i]*(juncHead[i]-minHead[i]);
		Xmax-=juncDemand[i]*minHead[i];
	}
	for (i=0; i<1; i++)
	{
		Xmax+=-1*reservoirDischarge[i]*reservoirHead[i];
	}
	networkResilience=X/Xmax;
	
	// close the hydraulic solver
	// ENclose();
	
	// report the objectives and constraint violation
    obj[0]=totalCost/1000000.0; // $ => $ MM
    obj[1]=-networkResilience; // for minimisation purpose
	constr[0]=-(pressureViolation+errorCode);
	return;
}
#endif

#ifdef BAK //#03
char* inputFile = "BAK.inp";
int nvars = 9;

void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
	const float diameterList[11]={300.0, 350.0, 400.0, 450.0, 500.0, 600.0, 700.0, 800.0, 900.0, 1000.0, 1100.0};
	const float unitCost[11]={118.0, 129.0, 145.0, 160.0, 181.0, 214.0, 242.0, 285.0, 325.0, 370.0, 434.0};
	float totalCost=0.0;
	float networkResilience=0.0;
	int errorCode=-1;
	int i=0;
	int j=0;
	int k=0;
	float retrievedData=0.0;
	float minHead[35], juncDemand[35], juncHead[35], juncU[35], reservoirDischarge[1], reservoirHead[1], pipeDiameter[9];
	float pressureViolation=0.0;
	float minPressure=15.0;
	int pipeSetting[9];
	int fromNode[58], toNode[58];
	int nPipe=0;
	int pipeIndex[58];
	float sumDiameter=0.0;
	float maxDiameter=0.0;
	float X=0.0;
	float Xmax=0.0;

	// open the hydraulic solver
    // ENopen("BAK.inp","reportFile.rpt","");
	// ENresetreport();
	// ENsetreport("MESSAGES NO");

	// initialise variables
	// set pipe diameters and compute total cost
	for (i=0; i<9; i++)
	{
		pipeSetting[i]=floor(xreal[i]);
		pipeDiameter[i]=diameterList[pipeSetting[i]];
		ENsetlinkvalue(1+i, EN_DIAMETER, pipeDiameter[i]);
		ENgetlinkvalue(1+i, EN_LENGTH, &retrievedData);
		totalCost+=unitCost[pipeSetting[i]]*retrievedData;
	}
	
	// run the hydraulic simulation
	errorCode=ENsolveH();

	// retrieve static and simulated values of each node
	for (i=0; i<35; i++)
	{
		ENgetnodevalue(1+i, EN_ELEVATION, &retrievedData);
		minHead[i]=retrievedData+minPressure;
		ENgetnodevalue(1+i, EN_DEMAND, &retrievedData);
		juncDemand[i]=retrievedData;
		ENgetnodevalue(1+i, EN_HEAD, &retrievedData);
		juncHead[i]=retrievedData;
		ENgetnodevalue(1+i, EN_PRESSURE, &retrievedData);
		if (retrievedData<minPressure)
		{
			pressureViolation+=(minPressure-retrievedData);
		}
	}
	for (i=0; i<1; i++)
	{
		ENgetnodevalue(35+1+i, EN_DEMAND, &retrievedData);
		reservoirDischarge[i]=retrievedData;
		ENgetnodevalue(35+1+i, EN_HEAD, &retrievedData);
		reservoirHead[i]=retrievedData;	
	}

	// calculate the uniformity of each junction
	for (i=0; i<58; i++)
	{
		ENgetlinknodes(1+i, &fromNode[i], &toNode[i]);
	}
	for (i=1; i<36; i++)
	{
		nPipe=0;
		maxDiameter=0;
		sumDiameter=0;
		for (j=0; j<58; j++)
		{
			if (fromNode[j]==i)
			{
				ENgetlinkvalue(1+j, EN_DIAMETER, &retrievedData);
				if (retrievedData>0.01)
				{
					nPipe+=1;
					pipeIndex[nPipe-1]=1+j;
				}
			}
			if (toNode[j]==i)
			{
				ENgetlinkvalue(1+j, EN_DIAMETER, &retrievedData);
				if (retrievedData>0.01)
				{
					nPipe+=1;
					pipeIndex[nPipe-1]=1+j;
				}
			}
		}
		for (k=0; k<nPipe; k++)
		{
			ENgetlinkvalue(pipeIndex[k], EN_DIAMETER, &retrievedData);
			sumDiameter+=retrievedData;
			if (retrievedData>maxDiameter)
			{
				maxDiameter=retrievedData;
			}
		}
		juncU[i-1]=sumDiameter/(nPipe*maxDiameter);
	}
	
	// compute the network resilience
	for (i=0; i<35; i++)
	{
		X+=juncU[i]*juncDemand[i]*(juncHead[i]-minHead[i]);
		Xmax-=juncDemand[i]*minHead[i];
	}
	for (i=0; i<1; i++)
	{
		Xmax+=-1*reservoirDischarge[i]*reservoirHead[i];
	}
	networkResilience=X/Xmax;

	// close the hydraulic solver
	// ENclose();

	// report the objectives and constraint violation
    obj[0]=totalCost/1000000.0; // $ => $ MM
    obj[1]=-networkResilience; // for minimisation purpose
	constr[0]=-(pressureViolation+errorCode);
    return;
}
#endif

#ifdef NYT //#04
char* inputFile = "NYT.inp";
int nvars = 21;

void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
	const float minHead[19]={255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 260.0, 272.8, 255.0, 255.0, 255.0};
	const float diameterList[15]={36.0, 48.0, 60.0, 72.0, 84.0, 96.0, 108.0, 120.0, 132.0, 144.0, 156.0, 168.0, 180.0, 192.0, 204.0};
	float totalCost=0.0;
	float networkResilience=0.0;
	int errorCode=-1;
	int i, j, k;
	float exponent=1.24;
	float retrievedData, juncDemand[19], juncHead[19], juncU[19], reservoirDischarge[1], reservoirHead[1], pipeDiameter[21];
	float violation=0.0;
	int pipeSetting[21];
	int fromNode[42], toNode[42];
	int nPipe=0;
	int pipeIndex[42];
	float sumDiameter=0.0;
	float maxDiameter=0.0;
	float X=0.0;
	float Xmax=0.0;

	// open the hydraulic solver
    // ENopen("NYT.inp","reportFile.rpt","");
	// ENresetreport();
	// ENsetreport("MESSAGES NO");

	// initialise variables
	// set pipe diameters and compute total cost
	for (i=0; i<21; i++)
	{
		pipeSetting[i]=floor(xreal[i]);
		if (pipeSetting[i]==0)
		{
			pipeDiameter[i]=0.0001;
			ENsetlinkvalue(21+1+i, EN_DIAMETER, pipeDiameter[i]);
			ENsetlinkvalue(21+1+i, EN_INITSTATUS, 0);
			ENgetlinkvalue(21+1+i, EN_LENGTH, &retrievedData);
			totalCost+=0.0;
		}
		else
		{
			pipeDiameter[i]=diameterList[pipeSetting[i]-1];
			ENsetlinkvalue(21+1+i, EN_DIAMETER, pipeDiameter[i]);
			ENsetlinkvalue(21+1+i, EN_INITSTATUS, 1);
			ENgetlinkvalue(21+1+i, EN_LENGTH, &retrievedData);
			totalCost+=1.1*pow(pipeDiameter[i],exponent)*retrievedData;
		}
	}

	// run the EPS and retrieve the results
	errorCode=ENsolveH();
	// ENsaveinpfile("cSol.inp");
	// retrieve static and simulated values of each node
	for (i=0; i<19; i++)
	{
		ENgetnodevalue(1+i, EN_DEMAND, &retrievedData);
		juncDemand[i]=retrievedData;
		ENgetnodevalue(1+i, EN_HEAD, &retrievedData);
		juncHead[i]=retrievedData;
		if (juncHead[i]<minHead[i])
		{
			violation+=(minHead[i]-juncHead[i]);
		}
	}
	for (i=0; i<1; i++)
	{
		ENgetnodevalue(19+1+i, EN_DEMAND, &retrievedData);
		reservoirDischarge[i]=retrievedData;
		ENgetnodevalue(19+1+i, EN_HEAD, &retrievedData);
		reservoirHead[i]=retrievedData;
	}

	// calculate the uniformity of each junction
	for (i=0; i<42; i++)
	{
		ENgetlinknodes(1+i, &fromNode[i], &toNode[i]);
	}
	for (i=1; i<20; i++)
	{
		nPipe=0;
		maxDiameter=0.0;
		sumDiameter=0.0;
		for (j=0; j<42; j++)
		{
			if (fromNode[j]==i)
			{
				ENgetlinkvalue(1+j, EN_DIAMETER, &retrievedData);
				if (retrievedData>0.01)
				{
					nPipe+=1;
					pipeIndex[nPipe-1]=1+j;
				}
			}
			if (toNode[j]==i)
			{
				ENgetlinkvalue(1+j, EN_DIAMETER, &retrievedData);
				if (retrievedData>0.01)
				{
					nPipe+=1;
					pipeIndex[nPipe-1]=1+j;
				}
			}
		}
		for (k=0; k<nPipe; k++)
		{
			ENgetlinkvalue(pipeIndex[k], EN_DIAMETER, &retrievedData);
			sumDiameter+=retrievedData;
			if (retrievedData>maxDiameter)
			{
				maxDiameter=retrievedData;
			}
		}
		juncU[i-1]=sumDiameter/(nPipe*maxDiameter);
	}
	
	// compute the network resilience
	for (i=0; i<19; i++)
	{
		X+=juncU[i]*juncDemand[i]*(juncHead[i]-minHead[i]);
		Xmax-=juncDemand[i]*minHead[i];
	}
	for (i=0; i<1; i++)
	{
		Xmax+=-1*reservoirDischarge[i]*reservoirHead[i];
	}
	networkResilience=X/Xmax;

	// close the hydraulic solver
	// ENclose();

	// report the objectives and constraint violation
    obj[0]=totalCost/1000000.0; // $ => $ MM
    obj[1]=-networkResilience; // for minimisation purpose
	constr[0]=-(violation+errorCode);
    return;
}
#endif

#ifdef BLA //#05
char* inputFile = "BLA.inp";
int nvars = 23;

void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
	const float diameterList[14]={25.40,50.80,76.20,101.60,152.40,203.20,254.00,304.80,355.60,406.40,457.20,508.00,558.80,609.60};
	const float unitCost[14]={0.52,2.10,4.72,8.40,18.90,33.60,52.50,75.59,102.89,134.39,170.09,209.98,254.08,302.37};
	const float maxPressure[30]={62.99,65.73,69.09,59.18,62.84,66.65,67.26,67.26,72.59,69.09,63.60,72.44,64.36,62.38,61.92,60.55,72.74,62.08,60.40,63.29,62.84,62.08,58.27,51.71,70.00,75.64,74.88,75.94,69.39,68.48};
	const int flexiblePipeIndex[23]={1,2,3,4,5,6,7,8,9,10,11,13,14,15,16,18,21,23,25,26,27,28,32};
	const int fixedPipeSetting[12]={4,4,4,7,5,6,4,4,4,4,4,4};
	const int fixedPipeIndex[12]={12,17,19,20,22,24,29,30,31,33,34,35};
	float minPressure=30.0;
	float totalCost=0.0;
	float networkResilience=0.0;
	int errorCode=-1;
	int i=0;
	int j=0;
	int k=0;
	float retrievedData=0.0;
	float juncDemand[30], juncHead[30], juncU[30], minHead[30], reservoirDischarge[1], reservoirHead[1];
	float flexiblePipeDiameter[23];
	float pressureViolation=0.0;
	float velocityViolation=0.0;
	int pipeSetting[23];
	int fromNode[35], toNode[35];
	int nPipe=0;
	int pipeIndex[35];
	float sumDiameter=0.0;
	float maxDiameter=0.0;
	float X=0.0;
	float Xmax=0.0;
	
	// open the hydraulic solver
    // ENopen("BLA.inp","reportFile.rpt","");
	// ENresetreport();
	// ENsetreport("MESSAGES NO");

	// set pipe diameters and compute total cost
	for (i=0; i<23; i++)
	{
		pipeSetting[i]=floor(xreal[i]);
		flexiblePipeDiameter[i]=diameterList[pipeSetting[i]];
		ENsetlinkvalue(flexiblePipeIndex[i], EN_DIAMETER, flexiblePipeDiameter[i]);
		ENgetlinkvalue(flexiblePipeIndex[i], EN_LENGTH, &retrievedData);
		totalCost+=unitCost[pipeSetting[i]]*retrievedData;
	}

	for (i=0; i<12; i++)
	{
		ENgetlinkvalue(fixedPipeIndex[i], EN_LENGTH, &retrievedData);
		totalCost+=unitCost[fixedPipeSetting[i]]*retrievedData;		
	}
	// ENsaveinpfile("randomSolution_Blacksburg");
	// run the hydraulic simulation
	errorCode=ENsolveH();

	// retrieve static and simulated values of each node
	for (i=0; i<30; i++)
	{
		ENgetnodevalue(1+i, EN_ELEVATION, &retrievedData);
		minHead[i]=retrievedData+minPressure;
		ENgetnodevalue(1+i, EN_DEMAND, &retrievedData);
		juncDemand[i]=retrievedData;
		ENgetnodevalue(1+i, EN_HEAD, &retrievedData);
		juncHead[i]=retrievedData;
		ENgetnodevalue(1+i, EN_PRESSURE, &retrievedData);
		if (retrievedData<minPressure)
		{
			pressureViolation+=(minPressure-retrievedData);
		}
		else if (retrievedData>maxPressure[i])
		{
			pressureViolation+=(retrievedData-maxPressure[i]);
		}
	}
	for (i=0; i<1; i++)
	{
		ENgetnodevalue(30+1+i, EN_DEMAND, &retrievedData);
		reservoirDischarge[i]=retrievedData;
		ENgetnodevalue(30+1+i, EN_HEAD, &retrievedData);
		reservoirHead[i]=retrievedData;	
	}
	// retrieve simulated values of each pipe
	for (i=0; i<35; i++)
	{
		ENgetlinkvalue(1+i, EN_VELOCITY, &retrievedData);
		if (retrievedData>2.0)
		{
			velocityViolation+=(retrievedData-2.0);
		}
	}

	// calculate the uniformity of each junction
	for (i=0; i<35; i++)
	{
		ENgetlinknodes(1+i, &fromNode[i], &toNode[i]);
	}
	for (i=1; i<31; i++)
	{
		nPipe=0;
		maxDiameter=0.0;
		sumDiameter=0.0;
		for (j=0; j<35; j++)
		{
			if (fromNode[j]==i)
			{
				ENgetlinkvalue(1+j, EN_DIAMETER, &retrievedData);
				if (retrievedData>0.01)
				{
					nPipe+=1;
					pipeIndex[nPipe-1]=1+j;
				}
			}
			if (toNode[j]==i)
			{
				ENgetlinkvalue(1+j, EN_DIAMETER, &retrievedData);
				if (retrievedData>0.01)
				{
					nPipe+=1;
					pipeIndex[nPipe-1]=1+j;
				}
			}
		}
		for (k=0; k<nPipe; k++)
		{
			ENgetlinkvalue(pipeIndex[k], EN_DIAMETER, &retrievedData);
			sumDiameter+=retrievedData;
			if (retrievedData>maxDiameter)
			{
				maxDiameter=retrievedData;
			}
		}
		juncU[i-1]=sumDiameter/(nPipe*maxDiameter);
	}

	// compute the network resilience
	for (i=0; i<30; i++)
	{
		X+=juncU[i]*juncDemand[i]*(juncHead[i]-minHead[i]);
		Xmax-=juncDemand[i]*minHead[i];
	}
	for (i=0; i<1; i++)
	{
		Xmax+=-1*reservoirDischarge[i]*reservoirHead[i];
	}
	networkResilience=X/Xmax;

	// close the hydraulic solver
	// ENclose();

	// report the objectives and constraint violation
    obj[0]=totalCost/1000000.0; // $ => $ MM
    obj[1]=-networkResilience; // for minimisation purpose
	constr[0]=-(pressureViolation+velocityViolation+errorCode);
    return;
}
#endif

#ifdef HAN //#06
char* inputFile = "HAN.inp";
int nvars = 34;

void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
	const float diameterList[6]={12.0, 16.0, 20.0, 24.0, 30.0, 40.0};
	float totalCost=0.0;
	float networkResilience=0.0;
	int errorCode=-1;
	int i, j, k;
	float retrievedData=0.0;
	float juncDemand[31], juncHead[31], juncU[31], reservoirDischarge[1], reservoirHead[1], pipeDiameter[34];
	float minHead=30.0;
	float headViolation=0.0;
	int pipeSetting[34];
	int fromNode[34], toNode[34];
	int nPipe, pipeIndex[34];
	float sumDiameter, maxDiameter;
	float X, Xmax;
	float exponent=1.5;

	// open the hydraulic solver
    // ENopen("HAN.inp","reportFile.rpt","");
	// ENresetreport();
	// ENsetreport("MESSAGES NO");

	// initialise variables
	// set pipe diameters and compute total cost
	for (i=0; i<34; i++)
	{
		pipeSetting[i]=floor(xreal[i]);
		pipeDiameter[i]=25.4*diameterList[pipeSetting[i]]; // in. => mm
		ENsetlinkvalue(i+1, EN_DIAMETER, pipeDiameter[i]);
		ENgetlinkvalue(i+1, EN_LENGTH, &retrievedData);
		totalCost+=1.1*pow(diameterList[pipeSetting[i]],exponent)*retrievedData;
	}

	// run the hydraulic simulation
	errorCode=ENsolveH();

	// retrieve static and simulated values of each node
	for (i=0; i<31; i++)
	{
		ENgetnodevalue(1+i, EN_DEMAND, &retrievedData);
		juncDemand[i]=retrievedData;
		ENgetnodevalue(1+i, EN_HEAD, &retrievedData);
		juncHead[i]=retrievedData;
		if (retrievedData<minHead)
		{
			headViolation+=(minHead-retrievedData);
		}
	}
	for (i=0; i<1; i++)
	{
		ENgetnodevalue(31+1+i, EN_DEMAND, &retrievedData);
		reservoirDischarge[i]=retrievedData;
		ENgetnodevalue(31+1+i, EN_HEAD, &retrievedData);
		reservoirHead[i]=retrievedData;
	}

	// calculate the uniformity of each junction
	for (i=0; i<34; i++)
	{
		ENgetlinknodes(1+i, &fromNode[i], &toNode[i]);
	}
	for (i=1; i<32; i++)
	{
		nPipe=0;
		maxDiameter=0;
		sumDiameter=0;
		for (j=0; j<34; j++)
		{
			if (fromNode[j]==i)
			{
				ENgetlinkvalue(1+j, EN_DIAMETER, &retrievedData);
				if (retrievedData>0.01)
				{
					nPipe+=1;
					pipeIndex[nPipe-1]=1+j;
				}
			}
			if (toNode[j]==i)
			{
				ENgetlinkvalue(1+j, EN_DIAMETER, &retrievedData);
				if (retrievedData>0.01)
				{
					nPipe+=1;
					pipeIndex[nPipe-1]=1+j;
				}
			}
		}
		for (k=0; k<nPipe; k++)
		{
			ENgetlinkvalue(pipeIndex[k], EN_DIAMETER, &retrievedData);
			sumDiameter+=retrievedData;
			if (retrievedData>maxDiameter)
			{
				maxDiameter=retrievedData;
			}
		}
		juncU[i-1]=sumDiameter/(nPipe*maxDiameter);
	}
	
	// compute the network resilience
	X=0.0;
	Xmax=0.0;
	for (i=0; i<31; i++)
	{
		X+=juncU[i]*juncDemand[i]*(juncHead[i]-minHead);
		Xmax-=juncDemand[i]*minHead;
	}
	for (i=0; i<1; i++)
	{
		Xmax+=-1*reservoirDischarge[i]*reservoirHead[i];
	}
	networkResilience=X/Xmax;

	// close the hydraulic solver
	// ENclose();

	// report the objectives and constraint violation
    obj[0]=totalCost/1000000.0; // $ => $ MM
    obj[1]=-networkResilience; // for minimisation purpose
	constr[0]=-(headViolation+errorCode);
	return;
}
#endif

#ifdef GOY //#07
char* inputFile = "GOY.inp";
int nvars = 30;

void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
	const float diameterList[8]={80.0, 100.0, 125.0, 150.0, 200.0, 250.0, 300.0, 350.0};
	const float unitCost[8]={37.890, 38.933, 40.563, 42.554, 47.624, 54.125, 62.109, 71.524};
	float totalCost=0.0;
	float networkResilience=0.0;
	int errorCode=-1;
	int i=0;
	int j=0;
	int k=0;
	float retrievedData=0.0;
	float minHead[22], juncDemand[22], juncHead[22], juncU[22], reservoirDischarge[1], reservoirHead[1], pumpPower[1], pipeDiameter[30];
	float pressureViolation=0.0;
	float minPressure=15.0;
	int pipeSetting[30];
	int fromNode[30], toNode[30];
	int nPipe=0;
	int pipeIndex[30];
	float sumDiameter=0.0;
	float maxDiameter=0.0;
	float X=0.0;
	float Xmax=0.0;

	// open the hydraulic solver
    // ENopen("GOY.inp","reportFile.rpt","");
	// ENresetreport();
	// ENsetreport("MESSAGES NO");

	// initialise variables
	// set pipe diameters and compute total cost
	for (i=0; i<30; i++)
	{
		pipeSetting[i]=floor(xreal[i]);
		pipeDiameter[i]=diameterList[pipeSetting[i]];
		ENsetlinkvalue(1+i, EN_DIAMETER, pipeDiameter[i]);
		ENgetlinkvalue(1+i, EN_LENGTH, &retrievedData);
		totalCost+=unitCost[pipeSetting[i]]*retrievedData;
	}
	
	// run the hydraulic simulation
	errorCode=ENsolveH();

	// retrieve static and simulated values of each node
	for (i=0; i<22; i++)
	{
		ENgetnodevalue(1+i, EN_ELEVATION, &retrievedData);
		minHead[i]=retrievedData+minPressure;
		ENgetnodevalue(1+i, EN_DEMAND, &retrievedData);
		juncDemand[i]=retrievedData;
		ENgetnodevalue(1+i, EN_HEAD, &retrievedData);
		juncHead[i]=retrievedData;
		ENgetnodevalue(1+i, EN_PRESSURE, &retrievedData);
		if (retrievedData<minPressure)
		{
			pressureViolation+=(minPressure-retrievedData);
		}
	}
	for (i=0; i<1; i++)
	{
		ENgetnodevalue(22+1+i, EN_DEMAND, &retrievedData);
		reservoirDischarge[i]=retrievedData;
		ENgetnodevalue(22+1+i, EN_HEAD, &retrievedData);
		reservoirHead[i]=retrievedData;	
	}
	// retrieve simulated values of each pump
	for (i=0; i<1; i++)
	{
		ENgetlinkvalue(30+1+i, EN_ENERGY, &retrievedData);
		pumpPower[i]=retrievedData;
	}

	// calculate the uniformity of each junction
	for (i=0; i<30; i++)
	{
		ENgetlinknodes(1+i, &fromNode[i], &toNode[i]);
	}
	for (i=1; i<23; i++)
	{
		nPipe=0;
		maxDiameter=0;
		sumDiameter=0;
		for (j=0; j<30; j++)
		{
			if (fromNode[j]==i)
			{
				ENgetlinkvalue(1+j, EN_DIAMETER, &retrievedData);
				if (retrievedData>0.01)
				{
					nPipe+=1;
					pipeIndex[nPipe-1]=1+j;
				}
			}
			if (toNode[j]==i)
			{
				ENgetlinkvalue(1+j, EN_DIAMETER, &retrievedData);
				if (retrievedData>0.01)
				{
					nPipe+=1;
					pipeIndex[nPipe-1]=1+j;
				}
			}
		}
		for (k=0; k<nPipe; k++)
		{
			ENgetlinkvalue(pipeIndex[k], EN_DIAMETER, &retrievedData);
			sumDiameter+=retrievedData;
			if (retrievedData>maxDiameter)
			{
				maxDiameter=retrievedData;
			}
		}
		juncU[i-1]=sumDiameter/(nPipe*maxDiameter);
	}
	
	// compute the network resilience
	for (i=0; i<22; i++)
	{
		X+=juncU[i]*juncDemand[i]*(juncHead[i]-minHead[i]);
		Xmax-=juncDemand[i]*minHead[i];
	}
	for (i=0; i<1; i++)
	{
		Xmax+=-1*reservoirDischarge[i]*reservoirHead[i];
	}
	for (i=0; i<1; i++)
	{
		Xmax+=1000*pumpPower[i]/9.81; // l/s => m3/s
	}
	networkResilience=X/Xmax;

	// close the hydraulic solver
	// ENclose();

	// report the objectives and constraint violation
	totalCost=totalCost/1000000.0;
    obj[0]=totalCost; // $ => $ MM
    obj[1]=-networkResilience; // for minimisation purpose
	constr[0]=-(pressureViolation+errorCode);
    return;
}
#endif

#ifdef FOS //#08
char* inputFile = "FOS.inp";
int nvars = 58;

void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
	const float diameterList[22]={16,20.4,26,32.6,40.8,51.4,61.4,73.6,90,102.2,114.6,130.8,147.2,163.6,184,204.6,229.2,257.8,290.6,327.4,368.2,409.2};
	const float unitCost[22]={0.38,0.56,0.88,1.35,2.02,3.21,4.44,6.45,9.59,11.98,14.93,19.61,24.78,30.55,38.71,47.63,59.7,75.61,99.58,126.48,160.29,197.71};
	const float maxPressure[36]={55.85,56.6,57.65,58.5,59.76,55.6,53.1,54.5,55,56.83,57.3,58.36,59.1,58.4,57.5,56.7,55.5,56.9,58.1,58.17,58.2,57.1,56.8,53.5,56.6,57.6,57.1,55.35,56.5,56.9,56.6,56.8,56.4,56.3,55.57,55.1};
	float totalCost=0.0;
	float networkResilience=0.0;
	int errorCode=-1;
	int i=0;
	int j=0;
	int k=0;
	float retrievedData=0.0;
	float minHead[36], juncDemand[36], juncHead[36], juncU[36], reservoirDischarge[1], reservoirHead[1], pipeDiameter[58];
	float minPressure=40.0;
	float pressureViolation=0.0;
	float velocityViolation=0.0;
	int pipeSetting[58];
	int fromNode[58], toNode[58];
	int nPipe=0;
	int pipeIndex[58];
	float sumDiameter=0.0;
	float maxDiameter=0.0;
	float X=0.0;
	float Xmax=0.0;

	// open the hydraulic solver
    // ENopen("FOS.inp","reportFile.rpt","");
	// ENresetreport();
	// ENsetreport("MESSAGES NO");

	// initialise variables
	// set pipe diameters and compute total cost
	for (i=0; i<58; i++)
	{
		pipeSetting[i]=floor(xreal[i]);
		pipeDiameter[i]=diameterList[pipeSetting[i]];
		ENsetlinkvalue(1+i, EN_DIAMETER, pipeDiameter[i]);
		ENgetlinkvalue(1+i, EN_LENGTH, &retrievedData);
		totalCost+=unitCost[pipeSetting[i]]*retrievedData;
	}
	
	// run the hydraulic simulation
	errorCode=ENsolveH();

	// retrieve static and simulated values of each node
	for (i=0; i<36; i++)
	{
		ENgetnodevalue(1+i, EN_ELEVATION, &retrievedData);
		minHead[i]=retrievedData+minPressure;
		ENgetnodevalue(1+i, EN_DEMAND, &retrievedData);
		juncDemand[i]=retrievedData;
		ENgetnodevalue(1+i, EN_HEAD, &retrievedData);
		juncHead[i]=retrievedData;
		ENgetnodevalue(1+i, EN_PRESSURE, &retrievedData);
		if (retrievedData<minPressure)
		{
			pressureViolation+=(minPressure-retrievedData);
		}
		else if (retrievedData>maxPressure[i])
		{
			pressureViolation+=(retrievedData-maxPressure[i]);
		}
	}
	for (i=0; i<1; i++)
	{
		ENgetnodevalue(36+1+i, EN_DEMAND, &retrievedData);
		reservoirDischarge[i]=retrievedData;
		ENgetnodevalue(36+1+i, EN_HEAD, &retrievedData);
		reservoirHead[i]=retrievedData;	
	}
	// retrieve simulated values of each pipe
	for (i=0; i<58; i++)
	{
		ENgetlinkvalue(1+i, EN_VELOCITY, &retrievedData);
		if (retrievedData>1.0)
		{
			velocityViolation+=(retrievedData-1.0);
		}
	}

	// calculate the uniformity of each junction
	for (i=0; i<58; i++)
	{
		ENgetlinknodes(1+i, &fromNode[i], &toNode[i]);
	}
	for (i=1; i<37; i++)
	{
		nPipe=0;
		maxDiameter=0.0;
		sumDiameter=0.0;
		for (j=0; j<58; j++)
		{
			if (fromNode[j]==i)
			{
				ENgetlinkvalue(1+j, EN_DIAMETER, &retrievedData);
				if (retrievedData>0.01)
				{
					nPipe+=1;
					pipeIndex[nPipe-1]=1+j;
				}
			}
			if (toNode[j]==i)
			{
				ENgetlinkvalue(1+j, EN_DIAMETER, &retrievedData);
				if (retrievedData>0.01)
				{
					nPipe+=1;
					pipeIndex[nPipe-1]=1+j;
				}
			}
		}
		for (k=0; k<nPipe; k++)
		{
			ENgetlinkvalue(pipeIndex[k], EN_DIAMETER, &retrievedData);
			sumDiameter+=retrievedData;
			if (retrievedData>maxDiameter)
			{
				maxDiameter=retrievedData;
			}
		}
		juncU[i-1]=sumDiameter/(nPipe*maxDiameter);
	}
	
	// compute the network resilience
	for (i=0; i<36; i++)
	{
		X+=juncU[i]*juncDemand[i]*(juncHead[i]-minHead[i]);
		Xmax-=juncDemand[i]*minHead[i];
	}
	for (i=0; i<1; i++)
	{
		Xmax+=-1*reservoirDischarge[i]*reservoirHead[i];
	}
	networkResilience=X/Xmax;

	// close the hydraulic solver
	// ENclose();

	// report the objectives and constraint violation
    obj[0]=totalCost/1000000.0; // € => € MM
    obj[1]=-networkResilience; // for minimisation purpose
	constr[0]=-(pressureViolation+velocityViolation+errorCode);
    return;
}
#endif

#ifdef PES//#09
char* inputFile = "PES.inp";
int nvars = 99;

void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
	const float diameterList[13]={100.0,125.0,150.0,200.0,250.0,300.0,350.0,400.0,450.0,500.0,600.0,700.0,800.0};
	const float unitCost[13]={27.7,38,40.5,55.4,75,92.4,123.1,141.9,169.3,191.5,246,319.6,391.1};
	const float maxPressure[68]={54.1,52,53.5,53.2,54.8,50,50.5,51.8,52.7,51.8,29,53.8,53.8,37.8,53,53.5,54.2,54.3,55.2,54.9,55,38.3,53.8,55.4,53.2,52.7,53.4,53.8,54.2,55.2,53.2,54.1,53.3,54.9,50.3,50.3,52.8,54.1,54.1,28.5,29.7,55.9,54.9,54.2,53.7,55.2,55.2,55.4,55.2,55.4,53.7,54.5,54.2,53.8,53.4,53.2,53.9,54,52.8,52.8,54.9,54.9,53.7,54.9,55.5,37.8,54.9,55.5};
	float totalCost=0.0;
	float networkResilience=0.0;
	int errorCode=-1;
	int i=0;
	int j=0;
	int k=0;
	float retrievedData=0.0;
	float minHead[68], juncDemand[68], juncHead[68], juncU[68], reservoirDischarge[3], reservoirHead[3], pipeDiameter[99];
	float minPressure=20.0;
	float pressureViolation=0.0;
	float velocityViolation=0.0;
	int pipeSetting[99];
	int fromNode[99], toNode[99];
	int nPipe=0;
	int pipeIndex[99];
	float sumDiameter=0.0;
	float maxDiameter=0.0;
	float X=0.0;
	float Xmax=0.0;

	// open the hydraulic solver
    // ENopen("PES.inp","reportFile.rpt","");
	// ENresetreport();
	// ENsetreport("MESSAGES NO");

	// initialise variables
	// set pipe diameters and compute total cost
	for (i=0; i<99; i++)
	{
		pipeSetting[i]=floor(xreal[i]);
		pipeDiameter[i]=diameterList[pipeSetting[i]];
		ENsetlinkvalue(1+i, EN_DIAMETER, pipeDiameter[i]);
		ENgetlinkvalue(1+i, EN_LENGTH, &retrievedData);
		totalCost+=unitCost[pipeSetting[i]]*retrievedData;
	}
	
	// run the hydraulic simulation
	errorCode=ENsolveH();

	// retrieve static and simulated values of each node
	for (i=0; i<68; i++)
	{
		ENgetnodevalue(1+i, EN_ELEVATION, &retrievedData);
		minHead[i]=retrievedData+minPressure;
		ENgetnodevalue(1+i, EN_DEMAND, &retrievedData);
		juncDemand[i]=retrievedData;
		ENgetnodevalue(1+i, EN_HEAD, &retrievedData);
		juncHead[i]=retrievedData;
		ENgetnodevalue(1+i, EN_PRESSURE, &retrievedData);
		if (retrievedData<minPressure)
		{
			pressureViolation+=(minPressure-retrievedData);
		}
		else if (retrievedData>maxPressure[i])
		{
			pressureViolation+=(retrievedData-maxPressure[i]);
		}
	}
	for (i=0; i<3; i++)
	{
		ENgetnodevalue(68+1+i, EN_DEMAND, &retrievedData);
		reservoirDischarge[i]=retrievedData;
		ENgetnodevalue(68+1+i, EN_HEAD, &retrievedData);
		reservoirHead[i]=retrievedData;	
	}
	// retrieve simulated values of each pipe
	for (i=0; i<99; i++)
	{
		ENgetlinkvalue(1+i, EN_VELOCITY, &retrievedData);
		if (retrievedData>2.0)
		{
			velocityViolation+=(retrievedData-2.0);
		}
	}

	// calculate the uniformity of each junction
	for (i=0; i<99; i++)
	{
		ENgetlinknodes(1+i, &fromNode[i], &toNode[i]);
	}
	for (i=1; i<69; i++)
	{
		nPipe=0;
		maxDiameter=0;
		sumDiameter=0;
		for (j=0; j<99; j++)
		{
			if (fromNode[j]==i)
			{
				ENgetlinkvalue(1+j, EN_DIAMETER, &retrievedData);
				if (retrievedData>0.01)
				{
					nPipe+=1;
					pipeIndex[nPipe-1]=1+j;
				}
			}
			if (toNode[j]==i)
			{
				ENgetlinkvalue(1+j, EN_DIAMETER, &retrievedData);
				if (retrievedData>0.01)
				{
					nPipe+=1;
					pipeIndex[nPipe-1]=1+j;
				}
			}
		}
		for (k=0; k<nPipe; k++)
		{
			ENgetlinkvalue(pipeIndex[k], EN_DIAMETER, &retrievedData);
			sumDiameter+=retrievedData;
			if (retrievedData>maxDiameter)
			{
				maxDiameter=retrievedData;
			}
		}
		juncU[i-1]=sumDiameter/(nPipe*maxDiameter);
	}
	
	// compute the network resilience
	for (i=0; i<68; i++)
	{
		X+=juncU[i]*juncDemand[i]*(juncHead[i]-minHead[i]);
		Xmax-=juncDemand[i]*minHead[i];
	}
	for (i=0; i<3; i++)
	{
		Xmax+=-1*reservoirDischarge[i]*reservoirHead[i];
	}
	networkResilience=X/Xmax;

	// close the hydraulic solver
	// ENclose();

	// report the objectives and constraint violation
    obj[0]=totalCost/1000000.0; // € => € MM
    obj[1]=-networkResilience; // for minimisation purpose
	constr[0]=-(pressureViolation+velocityViolation+errorCode);
    return;
}
#endif

#ifdef MOD//#10
char* inputFile = "MOD.inp";
int nvars = 317;

void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
	const float diameterList[13]={100.0,125.0,150.0,200.0,250.0,300.0,350.0,400.0,450.0,500.0,600.0,700.0,800.0};
	const float unitCost[13]={27.7,38,40.5,55.4,75,92.4,123.1,141.9,169.3,191.5,246,319.6,391.1};
	const float maxPressure[268]={35.007,34.875,35.797,37.254,38.235,38.545,38.545,38.413,36.321,37.497,38.000,37.112,36.426,37.481,33.243,35.150,34.971,37.906,37.739,36.785,37.188,36.877,37.513,39.295,39.387,39.846,40.175,38.355,38.204,38.403,38.361,38.700,41.239,41.163,40.987,41.800,41.853,41.935,40.935,42.905,43.119,41.833,41.001,40.929,40.726,40.363,40.820,40.794,42.823,41.155,41.668,41.722,35.224,37.377,38.016,38.084,38.365,38.451,37.735,39.016,39.451,39.395,36.549,36.058,36.693,36.282,35.773,35.547,34.799,33.911,33.688,33.436,33.047,32.670,33.065,33.408,33.757,35.895,37.585,37.751,37.687,37.455,38.617,38.046,38.339,39.509,38.888,39.608,38.914,38.800,39.305,38.860,38.571,36.861,37.332,37.395,37.529,37.503,37.761,39.724,40.243,40.840,40.716,40.754,41.123,39.650,40.227,40.203,40.546,40.580,42.183,39.742,40.287,39.576,38.544,43.811,43.905,43.769,43.797,43.480,43.468,42.755,42.500,42.452,42.402,40.740,42.229,42.640,42.083,41.498,40.874,38.134,38.806,38.976,38.940,38.583,39.133,39.443,40.375,35.150,35.396,34.659,34.659,35.051,34.795,36.549,36.890,36.549,38.814,39.183,38.690,38.688,38.481,36.246,36.996,36.964,37.421,37.745,38.615,38.732,39.796,39.131,39.507,38.573,38.235,41.833,41.746,41.616,40.415,38.407,38.451,38.459,38.483,42.743,42.590,42.701,43.017,43.384,43.404,43.306,44.108,43.953,43.366,42.690,42.155,41.674,40.806,41.325,41.271,41.157,40.728,40.732,42.296,40.095,41.111,40.155,39.473,40.061,39.966,39.565,39.796,37.800,38.297,39.469,37.735,38.303,36.621,36.465,37.637,37.262,37.842,38.010,37.200,34.201,34.651,33.502,33.340,39.451,40.580,42.356,40.333,39.403,42.951,42.755,42.434,42.556,42.843,43.460,43.450,36.008,38.816,39.110,39.612,39.642,39.505,41.959,40.087,38.343,39.195,39.329,41.582,41.434,42.590,42.498,42.452,42.446,43.795,43.168,38.204,38.669,37.555,36.487,37.850,37.595,37.727,43.003,35.849,34.957,34.919,34.919,33.949,33.714,33.546,36.745,38.537,37.691,38.289,38.888};
	float totalCost=0.0;
	float networkResilience=0.0;
	int errorCode=-1;
	int i=0;
	int j=0;
	int k=0;
	float retrievedData=0.0;
	float minHead[268], juncDemand[268], juncHead[268], juncU[268], reservoirDischarge[4], reservoirHead[4], pipeDiameter[317];
	float pressureViolation=0.0;
	float velocityViolation=0.0;
	int pipeSetting[317];
	int fromNode[317], toNode[317];
	int nPipe=0;
	int pipeIndex[317];
	float sumDiameter=0.0;
	float maxDiameter=0.0;
	float X=0.0;
	float Xmax=0.0;

	// open the hydraulic solver
    // ENopen("MOD.inp","reportFile.rpt","");
	// ENresetreport();
	// ENsetreport("MESSAGES NO");

	// initialise variables
	// set pipe diameters and compute total cost
	for (i=0; i<317; i++)
	{
		pipeSetting[i]=floor(xreal[i]);
		pipeDiameter[i]=diameterList[pipeSetting[i]];
		ENsetlinkvalue(1+i, EN_DIAMETER, pipeDiameter[i]);
		ENgetlinkvalue(1+i, EN_LENGTH, &retrievedData);
		totalCost+=unitCost[pipeSetting[i]]*retrievedData;
	}
	// ENsaveinpfile("randomSolution.inp");
	// run the hydraulic simulation
	errorCode=ENsolveH();

	// retrieve static and simulated values of each node
	for (i=0; i<268; i++)
	{
		ENgetnodevalue(1+i, EN_ELEVATION, &retrievedData);
		minHead[i]=retrievedData+20.0;
		ENgetnodevalue(1+i, EN_DEMAND, &retrievedData);
		juncDemand[i]=retrievedData;
		ENgetnodevalue(1+i, EN_HEAD, &retrievedData);
		juncHead[i]=retrievedData;
		ENgetnodevalue(1+i, EN_PRESSURE, &retrievedData);
		if (retrievedData<20.0)
		{
			pressureViolation+=(20.0-retrievedData);
		}
		else if (retrievedData>maxPressure[i])
		{
			pressureViolation+=(retrievedData-maxPressure[i]);
		}
	}
	for (i=0; i<4; i++)
	{
		ENgetnodevalue(268+1+i, EN_DEMAND, &retrievedData);
		reservoirDischarge[i]=retrievedData;
		ENgetnodevalue(268+1+i, EN_HEAD, &retrievedData);
		reservoirHead[i]=retrievedData;	
	}
	// retrieve simulated values of each pipe
	for (i=0; i<317; i++)
	{
		ENgetlinkvalue(1+i, EN_VELOCITY, &retrievedData);
		if (retrievedData>2.0)
		{
			velocityViolation+=(retrievedData-2.0);
		}
	}

	// calculate the uniformity of each junction
	for (i=0; i<317; i++)
	{
		ENgetlinknodes(1+i, &fromNode[i], &toNode[i]);
	}
	for (i=1; i<269; i++)
	{
		nPipe=0;
		maxDiameter=0;
		sumDiameter=0;
		for (j=0; j<317; j++)
		{
			if (fromNode[j]==i)
			{
				ENgetlinkvalue(1+j, EN_DIAMETER, &retrievedData);
				if (retrievedData>0.01)
				{
					nPipe+=1;
					pipeIndex[nPipe-1]=1+j;
				}
			}
			if (toNode[j]==i)
			{
				ENgetlinkvalue(1+j, EN_DIAMETER, &retrievedData);
				if (retrievedData>0.01)
				{
					nPipe+=1;
					pipeIndex[nPipe-1]=1+j;
				}
			}
		}
		for (k=0; k<nPipe; k++)
		{
			ENgetlinkvalue(pipeIndex[k], EN_DIAMETER, &retrievedData);
			sumDiameter+=retrievedData;
			if (retrievedData>maxDiameter)
			{
				maxDiameter=retrievedData;
			}
		}
		juncU[i-1]=sumDiameter/(nPipe*maxDiameter);
	}
	
	// compute the network resilience
	for (i=0; i<268; i++)
	{
		X+=juncU[i]*juncDemand[i]*(juncHead[i]-minHead[i]);
		Xmax-=juncDemand[i]*minHead[i];
	}
	for (i=0; i<4; i++)
	{
		Xmax+=-1*reservoirDischarge[i]*reservoirHead[i];
	}
	networkResilience=X/Xmax;

	// close the hydraulic solver
	// ENclose();

	// report the objectives and constraint violation
    obj[0]=totalCost/1000000.0; // € => € MM
    obj[1]=-networkResilience; // for minimisation purpose
	constr[0]=-(pressureViolation+velocityViolation+errorCode);
    return;
}
#endif

#ifdef BIN //#11
char* inputFile = "BIN.inp";
int nvars = 454;

void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
	const float diameterList[10]={113.0, 126.6, 144.6, 162.8, 180.8, 226.2, 285.0, 361.8, 452.2, 581.8};
	const float costTable[10]={7.22, 9.1, 11.92, 14.84, 18.38, 28.6, 45.39, 76.32, 124.64, 215.85};
	float totalCost=0.0;
	float networkResilience=0.0;
	int errorCode=-1;
	float pressureViolation=0.0;
	float pipeLength=0.0;
	int i, j, k;
	float retrievedData=0.0;
	int pipeSetting[454];
	float pipeDiameter[454];
	int fromNode[454];
	int toNode[454];
	int nPipe=0;
	float maxDiameter=0.0;
	float sumDiameter=0.0;
	int pipeIndex[454];
	float juncU[443];
	float X=0.0;
	float Xmax=0.0;
	float juncDemand[443];
	float juncHead[443];
	float minHead[443];
	float reservoirDischarge[4];
	float reservoirHead[4];

	// open the hydraulic solver
    // ENopen("BIN.inp","reportFile.rpt","");
	// ENresetreport();
	// ENsetreport("MESSAGES NO");

	// initialise variables
	// set pipe diameters and compute total cost
	for (i=0; i<454; i++)
	{
		pipeSetting[i]=floor(xreal[i]);
		pipeDiameter[i]=diameterList[pipeSetting[i]];
		ENsetlinkvalue(1+i, EN_DIAMETER, pipeDiameter[i]);
		ENgetlinkvalue(1+i, EN_LENGTH, &pipeLength);
		totalCost+=costTable[pipeSetting[i]]*pipeLength;
	}
	
	// run the hydraulic simulation
	errorCode=ENsolveH();

	// retrieve static and simulated values of each node
	for (i=0; i<443; i++)
	{
		ENgetnodevalue(1+i, EN_HEAD, &juncHead[i]);
		ENgetnodevalue(1+i, EN_DEMAND, &juncDemand[i]);
		ENgetnodevalue(1+i, EN_ELEVATION, &retrievedData);
		minHead[i]=20.0+retrievedData;
		ENgetnodevalue(1+i, EN_PRESSURE, &retrievedData);
		if (retrievedData<20.0)
		{
			pressureViolation+=(20.0-retrievedData);
		}
	}
	for (i=0; i<4; i++)
	{
		ENgetnodevalue(443+1+i, EN_HEAD, &reservoirHead[i]);
		ENgetnodevalue(443+1+i, EN_DEMAND, &reservoirDischarge[i]);
	}
	
	// calculate the uniformity of each junction
	for (i=0; i<454; i++)
	{
		ENgetlinknodes(1+i, &fromNode[i], &toNode[i]);
	}
	for (i=1; i<444; i++)
	{
		nPipe=0;
		maxDiameter=0.0;
		sumDiameter=0.0;
		for (j=0; j<454; j++)
		{
			if (fromNode[j]==i)
			{
				ENgetlinkvalue(1+j, EN_DIAMETER, &retrievedData);
				if (retrievedData>0.01)
				{
					nPipe+=1;
					pipeIndex[nPipe-1]=1+j;
				}
			}
			if (toNode[j]==i)
			{
				ENgetlinkvalue(1+j, EN_DIAMETER, &retrievedData);
				if (retrievedData>0.01)
				{
					nPipe+=1;
					pipeIndex[nPipe-1]=1+j;
				}
			}
		}
		for (k=0; k<nPipe; k++)
		{
			ENgetlinkvalue(pipeIndex[k], EN_DIAMETER, &retrievedData);
			sumDiameter+=retrievedData;
			if (retrievedData>maxDiameter)
			{
				maxDiameter=retrievedData;
			}
		}
		juncU[i-1]=sumDiameter/(nPipe*maxDiameter);
	}

	// compute the network resilience
	for (i=0; i<443; i++)
	{
		X+=juncU[i]*juncDemand[i]*(juncHead[i]-minHead[i]);
		Xmax-=juncDemand[i]*minHead[i];
	}
	for (i=0; i<4; i++)
	{
		Xmax+=-1*reservoirDischarge[i]*reservoirHead[i];
	}
	networkResilience=X/Xmax;

	// close the hydraulic solver
	// ENclose();

	// report the objectives and constraint violation
	obj[0]=totalCost/1000000.0; // € => € MM
	obj[1]=-networkResilience; // for minimisation purpose
	constr[0]=-(pressureViolation+errorCode);
	return;
}
#endif

#ifdef EXN //#12
char* inputFile = "EXN.inp";
int nvars = 567;

void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
	const float diameterList[10]={110.0, 159.0, 200.0, 250.0, 300.0, 400.0, 500.0, 600.0, 750.0, 900.0};
	const float roughnessList[10]={0.03, 0.065, 0.1, 0.13, 0.17, 0.23, 0.3, 0.35, 0.43, 0.5};
	const float unitCostMinorRoad[10]={85.0, 95.0, 115.0, 150.0, 200.0, 250.0, 310.0, 370.0, 450.0, 580.0};
	const float unitCostMajorRoad[10]={100.0, 120.0, 140.0, 190.0, 240.0, 290.0, 340.0, 410.0, 500.0, 625.0};
	const float minPressure=20.0;
	float totalCost=0.0;
	float networkResilience=0.0;
	float pressureViolation=0.0;
	int errorCode=-1;
	int i=0;
	int j=0;
	int k=0;
	float retrievedData=0.0;
	float minHead[1886], juncDemand[1886], juncHead[1886], juncU[1886], reservoirDischarge[7], reservoirHead[7], pipeDiameter[567];
	int pipeSetting[567];
	int fromNode[3032], toNode[3032];
	int nPipe=0;
	int pipeIndex[3032];
	float sumDiameter=0.0;
	float maxDiameter=0.0;
	float X=0.0;
	float Xmax=0.0;
	const char majorRoadID[41][8]={"dup5016", "dup4899", "dup2791", "dup3172", "dup3121", "dup3154", "dup3147", "dup2759", "dup2821", "dup2752", "dup2769", "dup2681", "dup2803", "dup2784", "dup5085", "dup2579", "dup2651", "dup3055", "dup4057", "dup2790", "dup2619", "dup2087", "dup5066", "dup2063", "dup5239", "dup5117", "dup3513", "dup2493", "dup2297", "dup5063", "dup5165", "dup2369", "dup4072", "dup2065", "dup3917", "dup3937", "dup3823", "dup3792", "dup3834", "dup2963", "dup3690"};
	char pipeID[8]="";
	int whetherMajorRoad;

	// open the hydraulic solver
    // ENopen("EXN.inp","reportFile.rpt","");
	// ENresetreport();
	// ENsetreport("MESSAGES NO");

	// initialise variables
	// set pipe diameters
	for (i=0; i<567; i++)
	{
		pipeSetting[i]=floor(xreal[i]);
		if (pipeSetting[i]<10)
		{
			pipeDiameter[i]=diameterList[pipeSetting[i]];
			ENsetlinkvalue(2465+1+i, EN_DIAMETER, pipeDiameter[i]);
			ENsetlinkvalue(2465+1+i, EN_ROUGHNESS, roughnessList[pipeSetting[i]]);
			ENsetlinkvalue(2465+1+i, EN_INITSTATUS, 1);
		}
		else // do nothing option
		{
			ENsetlinkvalue(2465+1+i, EN_DIAMETER, 0.0001);
			ENsetlinkvalue(2465+1+i, EN_INITSTATUS, 0);
		}
	}
	
	// run the hydraulic simulation
	errorCode=ENsolveH();

	// retrieve static and simulated values of each node
	for (i=0; i<1886; i++) // Node 3003 to node 3007 should be treated as reservoirs, therefore, the upper bound of junction is (1891-5)=1886
	{
		ENgetnodevalue(1+i, EN_ELEVATION, &retrievedData);
		minHead[i]=retrievedData+minPressure;
		ENgetnodevalue(1+i, EN_DEMAND, &retrievedData);
		juncDemand[i]=retrievedData;
		ENgetnodevalue(1+i, EN_HEAD, &retrievedData);
		juncHead[i]=retrievedData;
		ENgetnodevalue(1+i, EN_PRESSURE, &retrievedData);
		if (juncDemand[i]>0.0 && retrievedData<minPressure && i != 0) // There must be something wrong with node 1107 as it is impossible to achieve a 20.0 m pressure at this location.
		{
			pressureViolation+=(minPressure-retrievedData);
		}
	}
	for (i=0; i<7; i++)
	{
		ENgetnodevalue(1886+1+i, EN_DEMAND, &retrievedData);
		reservoirDischarge[i]=retrievedData;
		ENgetnodevalue(1886+1+i, EN_HEAD, &retrievedData);
		reservoirHead[i]=retrievedData;
	}

	// calculate the total cost based on the road type
	for (i=0; i<567; i++)
	{
		ENgetlinkvalue(2465+1+i, EN_LENGTH, &retrievedData);
		ENgetlinkid(2465+1+i, pipeID);
		whetherMajorRoad=0;
		for (j=0; j<41; j++)
		{
			if (strcmp(pipeID, majorRoadID[j])==0)
			{
				whetherMajorRoad=1;
				break;
			}
		}
		if (whetherMajorRoad==1 && pipeSetting[i]<10)
		{
			totalCost+=unitCostMajorRoad[pipeSetting[i]]*retrievedData;
		}
		else if (whetherMajorRoad==0 && pipeSetting[i]<10)
		{
			totalCost+=unitCostMinorRoad[pipeSetting[i]]*retrievedData;
		}
	}

	// calculate the uniformity of each junction
	for (i=0; i<3032; i++)
	{
		ENgetlinknodes(1+i, &fromNode[i], &toNode[i]);
	}
	for (i=1; i<1887; i++)
	{
		nPipe=0;
		maxDiameter=0.0;
		sumDiameter=0.0;
		for (j=0; j<3032; j++)
		{
			if (fromNode[j]==i)
			{
				ENgetlinkvalue(1+j, EN_DIAMETER, &retrievedData);
				if (retrievedData>0.01)
				{
					nPipe+=1;
					pipeIndex[nPipe-1]=1+j;
				}
			}
			if (toNode[j]==i)
			{
				ENgetlinkvalue(1+j, EN_DIAMETER, &retrievedData);
				if (retrievedData>0.01)
				{
					nPipe+=1;
					pipeIndex[nPipe-1]=1+j;
				}
			}
		}
		for (k=0; k<nPipe; k++)
		{
			ENgetlinkvalue(pipeIndex[k], EN_DIAMETER, &retrievedData);
			sumDiameter+=retrievedData;
			if (retrievedData>maxDiameter)
			{
				maxDiameter=retrievedData;
			}
		}
		juncU[i-1]=sumDiameter/(nPipe*maxDiameter);
	}
	
	// compute the network resilience
	for (i=0; i<1886; i++)
	{
		X+=juncU[i]*juncDemand[i]*(juncHead[i]-minHead[i]);
		Xmax-=juncDemand[i]*minHead[i];
	}
	for (i=0; i<7; i++)
	{
		Xmax+=-1*reservoirDischarge[i]*reservoirHead[i];
	}
	networkResilience=X/Xmax;

	// close the hydraulic solver
	// ENclose();

	// report the objectives and constraint violation
    obj[0]=totalCost/1000000.0; // £ => £ MM
    obj[1]=-networkResilience; // for minimisation purpose
	constr[0]=-(pressureViolation+errorCode);
    return;
}
#endif