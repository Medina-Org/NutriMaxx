#pragma once
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
using namespace std;
using namespace chrono;

class FoodList
{
public:

	class Food
	{
	public:

		string category;
		string name;
		int id;
		double nutrients[35];	//	Array holding all nutrient values

		Food();
	};

	// Variables
	Food* foodList = new Food[7078];	//	Food array containing all the foods in the CSV file
	
	steady_clock::time_point start;		//	Timer variable holding start time
	steady_clock::time_point end;		//	Timer variable holding end time
	string shellSortTime = "0";				//	String holding shell sorting time
	string mergeSortTime = "0";				//	String holding mergeSorting time
	vector<string> nutrientNames;		// Vector holding names of nutrients corresponding to the nutrient index

	// Functions

	FoodList();
	~FoodList();

	// Merge Sorting
	void merge(Food* arr, int left, int mid, int right, int nutrientIndex);
	void mergeSort(Food* arr, int left, int right, int nutrientIndex);
	void mergeSortFoodList(int nutrientIndex);

	// Shell Sorting
	void shellSortFoodList(int nutrientIndex);

	//	Timer Functions
	void timeStart();	//	Starts timer
	void timeStopAndDisplay(string sortType);	//	Ends timer and displays time

	// Utility function for printing units for nutrient values
	string getUnit(int nutrientIndex);
};