//===========================================================================
//Gamma.cpp
//This program is designed to create a continuous grid world that an agent
//can move to all of the points of interest and will be able to optimize the path
//to be as short as possible.
//
//Input:
//	N/A
//			
//Output:
//  N/A
//
//===========================================================================

//#define NDEBUG
#define cl4ptp (double)rand()/RAND_MAX
#include "stdafx.h"
#include <conio.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cassert>
#include <random>
#include <vector>
#include <iomanip>
#include <deque>
#include <algorithm>
#include <numeric>
#include <functional>
#include <Windows.h>

using namespace std;
// Function Prototypes
void welcome();
vector<double[2]> create_pois(int point_num);

/*// Classes
class policy
{
	deque<int> path;
	double fitness;
};
*/

//===========================================================================
//===========================================================================					main
int main()
{
	//Initialize Random Number Generator
	srand((unsigned)time(NULL));

	welcome();
	int point_num = 10;		// Number of points of interest to visit
	vector<double[2]> positions;	// Storage for off point locations
	positions = create_pois(point_num);

    return 0;
}

//===========================================================================					welcome
void welcome()
{

}

//===========================================================================					create_pois
vector<double[2]> create_pois(int point_num)
{
	double min = 0;
	double max = 100;
	double pos[2];
	vector<double[2]> positions;

	for (int i = 0; i < point_num; i++)
	{
		pos[0] = cl4ptp*(max - min) + min;
		pos[1] = cl4ptp*(max - min) + min;
		positions.push_back(pos);
	}
	
	// check to see that every pos in positions meets our expectations for initial dimensions
	double temp[2];
	for (int i = 0; i < point_num; i++) 
	{
		temp[0] = positions[i][0];
		temp[1] = positions[i][1];
		assert(temp[0] > min);
		assert(temp[0] < max);
		assert(temp[1] > min);
		assert(temp[1] < max);
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++					pois::init
