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

void openfile(ifstream &stream, string filename)
{
	stream.open(filename.c_str(), ios::in);
	if (!stream)
	{
		cerr << "Error in reading " << filename << ". Exiting..." << endl;
		winexit();
	}
	return;
}

void openfile(ofstream &stream, string filename)
{
	stream.open(filename.c_str(), ios::out);
	if (!stream)
	{
		cerr << "Error in reading " << filename << ". Exiting..." << endl;
		winexit();
	}
	return;
}

void winexit()
{

#ifdef WIN32
		system("PAUSE");
#endif
	exit(1);
	return;
}

void winexit(string message)
{
	cerr << message << endl;
#ifdef WIN32
		system("PAUSE");
#endif
	exit(1);
	return;
}

//void alpha_feasibility(double a, double b)
//{
//	if (a > b)
//	{
//		ifail = 1;
//		icount = 0;
//		cout << "There was an alpha/beta error.  Alpha cannot be greater than beta." << endl;
//		cout << endl << "Exiting ..." << endl;
//		exit(1);
//	}
//
//	return;
//
//}

//void debug_param_feasibility ()
//{
//	if (debug_sims > NumberSims)
//	{
//		cout << "There was an debug output parameter error.  You cannot show output for more simulations than you will run." << endl;
//		cout << endl << "Exiting ..." << endl;
//		exit(1);
//	}
//
//	if (debug_years > NumberYears)
//	{
//		cout << "There was a debug output parameter error.  You cannot show output for more years than the NumberYears you've specified." << endl;
//		cout << endl << "Exiting ..." << endl;
//		exit(1);
//	}
//
//	return;
//}


double average_array (double *data_set, const int length)
{
	double sum = 0.0;

	for (int i = 0; i < length; i++)
	{
		sum += data_set[i];
	}

	return (sum / length);

}

double average_array (vector<double> data, const int length)
{
	double sum = 0.0;
	for (int i = 0; i < length; i++)
	{
		sum += data[i];
	}

	return (sum/length);
}



double average_array_colwise (double **data, int rows, int cols, int col_of_interest)
{
	double dummy_sum = 0.0;
	double average = 0.0;

	for (int row_it = 0; row_it < rows; row_it++)
	{
		dummy_sum = dummy_sum + data[row_it][col_of_interest];
	}//end row loop

	average = dummy_sum / rows;

	//cout << "Call to average function: " << average << endl;

	return average;

}
double sum(double* data, int length)
{
	double value=0;

	for (int i = 0; i < length; i++)
	{
		value += data[i];
	}

	return value;
}

double sum(vector<double> data, int length)
{
	double value = 0;

	for (int i = 0; i < length; i++)
	{
		value+=data[i];
	}

	return value;
}

double min_array(double *data, const int length)
{
	//order(n) sorting routine

	double min = data[0]; //assume the first value is the minimum

	for (int i = 0; i < length; i++)
	{
		if (data[i] < min)
		{
			min = data[i]; //if we have a min, record it
		}
	}

	return min;
}

double max_array(double *data, const int length)
{
	//order(n) sorting routine

	double max = data[0]; //assume the first value is the maximum

	for (int i = 0; i < length; i++)
	{
		if (data[i] > max)
		{
			max = data[i]; //if we have a max, record it
		}
	}

	return max;
}

double max_array(vector<double> data, const int length)
{
	//order(n) sorting routine

	double max = data[0]; //assume the first value is the maximum

	for (int i = 0; i < length; i++)
	{
		if (data[i] > max)
		{
			max = data[i]; //if we have a max, record it
		}
	}

	return max;
}

int max_index_array(double *data, const int length)
{
	//order(n) sorting routine

	double max = data[0]; //assume the first value is the maximum
	int index_of_interest = 0;

	for (int i = 0; i < length; i++)
	{
		if (data[i] > max)
		{
			max = data[i]; //if we have a max, record it
			index_of_interest = i;
		}
	}

	return index_of_interest;
}

double std_dev(double *data, int length)
{
	//first calculate sum
	double sum = 0.0;

	for (int i = 0; i < length; i++)
	{
		sum += data[i];
	}

	double mean = sum / length;

	double squared_sum = 0.0;

	for (int i = 0; i < length; i++)
	{
		squared_sum += pow((data[i]-mean),2.0);
	}

	return sqrt( squared_sum / (length-1) );
}

void zeroes(double *data, const int length)
{
	for (int i = 0; i<length; i++)
	{
		data[i] = 0.0;
	}

	return;
}

void zeroes(int *data, const int length)
{
	for (int i = 0; i<length; i++)
	{
		data[i] = 0;
	}

	return;
}


void zeroes(double **data, const int rows, const int cols)
{
	for (int row_it = 0; row_it<rows; row_it++)
	{
		for (int col_it = 0; col_it<cols; col_it++)
		{
			data[row_it][col_it] = 0.0;
		}
	}
}


void zeroes(int **data, const int rows, const int cols)
{
	for (int row_it = 0; row_it<rows; row_it++)
	{
		for (int col_it = 0; col_it<cols; col_it++)
		{
			data[row_it][col_it] = 0;
		}
	}
}

void zeroes(int data[][12], const int rows, const int cols)
{
	for (int row_it = 0; row_it<rows; row_it++)
	{
		for (int col_it = 0; col_it<cols; col_it++)
		{
			data[row_it][col_it] = 0;
		}
	}
}

void zeroes(double data[][12], const int rows, const int cols)
{
	for (int row_it = 0; row_it<rows; row_it++)
	{
		for (int col_it = 0; col_it<cols; col_it++)
		{
			data[row_it][col_it] = 0;
		}
	}
}

double rounding(double input)
{
	double value;
	if (input - floor(input) < 0.5)
	{
		value = floor(input);
	}
	else value = ceil(input);

	return value;
}

double rounding(double input, double dec_places)
{
	//function to round 'input' by the number of decimal places specified, 'dec_places'.
	//Algorithm found at: http://cplusplus.com/forum/beginner/3600/
	return floor(input*pow(10.0,dec_places))/pow(10.0,dec_places);
}