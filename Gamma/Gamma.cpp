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
vector<poi> create_pois(int point_num);
vector<policy> create_pop(int pop_size, int point_num);
vector<policy> replicate(vector<policy> population);

// Classes
class poi
{
public:
	double x;
	double y;
	void init(double max, double min);
};

class policy
{
public:
	deque<int> path;
	double fitness;
	void init(int point_num);
	deque<int> mutate(deque<int> order, int number);
};


//===========================================================================
//===========================================================================					main
int main()
{
	//Initialize Random Number Generator
	srand((unsigned)time(NULL));

	welcome();
	int point_num = 10;		// Number of points of interest to visit
	int generations = 10;	// Number of Generations
	int pop_size = 10;		// Half of total poulation size after replication

	vector<poi> state;		// Stores each point of interest
	state = create_pois(point_num);

	vector<policy> population;
	population = create_pop(pop_size, point_num);

	for (int i = 0; i < generations; i++)
	{
		population = replicate(population);
	}


    return 0;
}

//===========================================================================					welcome
void welcome()
{

}

//===========================================================================					create_pois
vector<poi> create_pois(int point_num)
{
	double min_s = 0;
	double max_s = 100;
	vector<poi> city;

	for (int i = 0; i < point_num; i++)
	{
		poi A;
		A.init(max_s, min_s);
		city.push_back(A);
	}
	
	// check to see that every pos in positions meets our expectations for initial dimensions
	double temp[2];
	for (int i = 0; i < point_num; i++) 
	{
		assert(city.at(i).x > min_s);
		assert(city.at(i).x < min_s);
		assert(city.at(i).y > min_s);
		assert(city.at(i).y < max_s);
	}
	return city;
}

//===========================================================================					create_pop
vector<policy> create_pop(int pop_size, int point_num)
{
	vector<policy> pop;

	for (int i = 0; i < pop_size; i++)
	{
		policy A;
		A.init(point_num);
		pop.push_back(A);
	}
	return pop;
}

//===========================================================================					replicate
vector<policy> replicate(vector<policy> population)
{
	vector<policy> n_population;
	n_population = population;

	while (n_population.size() < 2 * population.size())
	{
		int choice = cl4ptp*(n_population.size());
		policy A = n_population.at(choice);
		A.path = A.mutate(A.path, n_population.size / 10);
		n_population.push_back(A);
	}
	assert(n_population.size() == population.size());
	return n_population;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++					poi::init
void poi::init(double max, double min)
{
	x = cl4ptp*(max - min) + min;
	y = cl4ptp*(max - min) + min;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++					policy::init
void policy::init(int point_num)
{
	deque<int> ordered_poi;
	for (int i = 0; i < point_num; i++)
	{
		ordered_poi.push_back(i);
	}

	path = mutate(ordered_poi, point_num/2);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++					policy::mutate
deque<int> policy::mutate(deque<int> order, int number)
{
	for (int i = 0; i < number; i++)
	{
		int choice = cl4ptp*(order.size());
		if (choice != 0)
		{
			assert(choice > 0);
			assert(choice < order.size());

			int point = order.at(choice);
			order.erase(order.begin() + choice);
			order.push_back(point);
		}
		else
		{
			i--;
		}
		assert(order.at(0) = 0);
	}
	return order;
}