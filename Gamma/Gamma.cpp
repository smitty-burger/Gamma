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
//  	Salesman_Dist.txt
//	Salesman_Path.txt
//	Salesman_Learning_History.txt
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
	void evaluate(vector<poi> state);
	void check_path();
};

// Function Prototypes
void welcome();
void print(vector<policy> population, vector<poi> state, vector<double> history);
double find_best_for_it(vector<policy> population, vector<poi> state);
vector<poi> create_pois(int point_num);
vector<policy> create_pop(int pop_size, int point_num);
vector<policy> replicate(vector<policy> population);
vector<policy> bin_turney(vector<policy> population);

//===========================================================================
//===========================================================================					main
int main()
{
	//Initialize Random Number Generator
	srand((unsigned)time(NULL));

	welcome();
	int point_num = 10;		// Number of points of interest to visit
	int generations = 1000;	// Number of Generations
	int pop_size = 10;		// Half of total poulation size after replication
	vector<double> history;

	vector<poi> state;					// Stores each point of interest
	state = create_pois(point_num);		// Randomly create points of interest

	vector<policy> population;						// Store each policy
	population = create_pop(pop_size, point_num);	// Randomly create n-number of policies
	
	for (int i = 0; i < generations; i++)
	{
		population = replicate(population);			// Perform replication and mutation
		assert(pop_size * 2 == population.size());	// Check for proper poulation size

		for (int j = 0; j < pop_size * 2; j++)		// MR_3
		{
			population.at(j).evaluate(state);		// Evaluate fitness of policies
		}

		population = bin_turney(population);		// Run binary turnament to downselect
		assert(pop_size == population.size());		// Check for proper poulation size

		history.push_back(find_best_for_it(population, state));

	}

	print(population, state, history);

    return 0;
}

//===========================================================================					welcome
void welcome()
{
	cout << "Running..." << endl;
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
		A.init(max_s, min_s);		// Send dimensions of the State to the initialization function
		city.push_back(A);			// Add created location to the vector of cities
	}
	
	// Check to see that every pos in positions meets our expectations for initial dimensions
	for (int i = 0; i < point_num; i++) 
	{
		assert(city.at(i).x > min_s);
		assert(city.at(i).x < max_s);
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
vector<policy> replicate(vector<policy> population)		// MR_5
{
	vector<policy> n_population;
	n_population = population;
	int num = 10;
	int nu_2 = population.size();


	while (n_population.size() < 2 * population.size())
	{
		int choice = cl4ptp*(n_population.size());
		policy A = n_population.at(choice);
		A.path = A.mutate(A.path, (nu_2 / num));
		n_population.push_back(A);
	}
	
	assert(n_population.size() == 2*population.size());
	return n_population;
}

//===========================================================================					bin_turney
vector<policy> bin_turney(vector<policy> population)		// MR_4
{
	int end = population.size() / 2;
	for (int i = 0; i < end; i++)
	{
		int choice_1 = cl4ptp*(population.size());
		int choice_2 = cl4ptp*(population.size());
		double fit_1;
		double fit_2;
		if (choice_1 != 0 && choice_2 != 0 && choice_1 != choice_2)
		{
			assert(choice_1 > 0);
			int num = population.size();
			if (choice_1 >= num)
			{
				choice_1 = choice_1 - (choice_1 - num + 1);
			}
			assert(choice_1 < num);

			assert(choice_2 > 0);
			if (choice_2 >= num)
			{
				choice_2 = choice_2 - (choice_2 - num + 1);
			}
			assert(choice_2 < num);

			fit_1 = population.at(choice_1).fitness;
			fit_2 = population.at(choice_2).fitness;

			if (fit_1 > fit_2)
			{
				population.erase(population.begin() + choice_1);
			}
			else if (fit_1 < fit_2)
			{
				population.erase(population.begin() + choice_2);
			}
			else
			{
				population.erase(population.begin() + choice_2);
			}
		}
		else
		{
			i--;
		}
	}
	return population;
}

//===========================================================================					find_best_for_it
double find_best_for_it(vector<policy> population, vector<poi> state)
{
	// Find best route
	int end = population.size();
	double min_fit;
	min_fit = population.at(0).fitness;
	for (int i = 1; i < end; i++)
	{
		if (population.at(i).fitness < min_fit)
		{
			min_fit = population.at(i).fitness;
		}
	}

	return min_fit;
}

//===========================================================================					print
void print(vector<policy> population, vector<poi> state, vector<double> history)		// HR_1 & HR_2 & HR_3 & HR_4
{
	// Clear Console Screen
	system("CLS");

	// Now Writing Data To File
	cout << "Now Writing Data To File" << endl;

	// Create output file
	ofstream output_file;
	output_file.open("Salesman_Dist.txt");

	int end = population.size();
	for (int i = 0; i < end; i++)
	{
		output_file << i << '\t' << population.at(i).fitness << endl;
	}

	//Close output file
	output_file.close();



	// Create output file
	output_file.open("Salesman_Path.txt");

	// Find best route
	double min_fit;
	int pol_num;
	min_fit = population.at(0).fitness;
	pol_num = 0;
	for (int i = 1; i < end; i++)
	{
		if (population.at(i).fitness < min_fit)
		{
			min_fit = population.at(i).fitness;
			pol_num = i;
		}
	}

	end = state.size();
	for (int i = 0; i < end; i++)
	{
		int city_num;
		city_num = population.at(pol_num).path.at(i);
		output_file << state.at(city_num).x << '\t' << state.at(city_num).y << endl;
	}

	//Close output file
	output_file.close();



	// Create output file
	output_file.open("Salesman_Learning_History.txt");

	end = history.size();
	for (int i = 0; i < end; i++)
	{
		output_file << history[i] << endl;
	}

	//Close output file
	output_file.close();

	// Wait for
	Sleep(750);

	// Clear Console Screen
	system("CLS");

	//User console update
	cout << "Data Has Been Written To File" << endl;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++					poi::init
void poi::init(double max, double min)
{
	x = cl4ptp*(max - min) + min;
	y = cl4ptp*(max - min) + min;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++					policy::init
void policy::init(int point_num)		// MR_1
{
	deque<int> ordered_poi;
	for (int i = 0; i < point_num; i++)
	{
		ordered_poi.push_back(i);
	}

	path = mutate(ordered_poi, point_num/2);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++					policy::mutate
deque<int> policy::mutate(deque<int> order, int number)		// LR_4
{
	for (int i = 0; i < number; i++)
	{
		int choice = cl4ptp*(order.size());
		if (choice != 0)
		{
			assert(choice > 0);
			int num = order.size();
			if (choice >= num)
			{
				choice = choice - (choice - num + 1);
			}
			assert(choice < num);

			int point = order.at(choice);
			order.erase(order.begin() + choice);
			order.push_back(point);
		}
		else
		{
			i--;
		}
		assert(order.at(0) == 0);	//LR_5
	}
	return order;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++					policy::evaluate
void policy::evaluate(vector<poi> state)
{
	fitness = 0;
	int end = path.size();

	double del_x;
	double del_y;
	double dist;
	for (int i = 1; i < end; i++)			// LR_7 & LR_8 Using Pythagorean Theorem
	{
		del_x = state.at(path.at(i)).x - state.at(path.at(i - 1)).x;
		del_y = state.at(path.at(i)).y - state.at(path.at(i - 1)).y;
		dist = del_x*del_x + del_y*del_y;
		dist = sqrt(dist);			// LR_7 Using Pythagorean Theorem
		fitness = fitness + dist;	// LR_8 & MR_2
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++					policy::check_path
void policy::check_path()		// LR_6
{
	int end = path.size();
	for (int i = 0; i < end-1; i++)
	{
		for (int j = 0; j < end-1; i++)
		{
			if (j != i)
			{
				assert(path[i] != path[j]);
			}
		}
	}
}