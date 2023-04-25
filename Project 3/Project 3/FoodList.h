#pragma once
#include <string>
#include <iostream>
using namespace std;

class FoodList
{
public:

	class Food
	{
	public:

		string category;
		string name;
		int id;
		double nutrients[35];

		Food();
	};

	// Variables
	Food* foodList = new Food[7078];

	// Functions

	FoodList();
	~FoodList();

	// Sorting

};