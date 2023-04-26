#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include "FoodList.h"
using namespace std;
using namespace chrono;

FoodList::FoodList()
{
}

FoodList::~FoodList()
{
    delete[] foodList;
}

FoodList::Food::Food()
{
}

// Merge function to merge two sub-arrays
void FoodList::merge(Food* arr, int left, int middle, int right, int nutrientIndex)
{
    int i, j, k;
    int n1 = middle - left + 1;
    int n2 = right - middle;

    // Create temporary arrays
    Food* leftArr = new Food[n1];
    Food* rightArr = new Food[n2];

    // Copy data to temporary arrays
    for (i = 0; i < n1; i++)
        leftArr[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        rightArr[j] = arr[middle + 1 + j];

    i = 0;
    j = 0;
    k = left;
    // Merge the temporary arrays back into arr[]
    while (i < n1 && j < n2)
    {
        // Compare nutrient values based on given nutrient index
        if (leftArr[i].nutrients[nutrientIndex] > rightArr[j].nutrients[nutrientIndex])
        {
            arr[k] = leftArr[i];
            i++;
        }
        else
        {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of leftArr[]
    while (i < n1)
    {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    // Copy the remaining elements of rightArr[]
    while (j < n2)
    {
        arr[k] = rightArr[j];
        j++;
        k++;
    }

    // Deallocate memory
    delete[] leftArr;
    delete[] rightArr;
}

// Merge Sort function
void FoodList::mergeSort(Food* arr, int left, int right, int nutrientIndex)
{
    if (left < right)
    {
        int middle = left + (right - left) / 2;

        // Sort left and right halves recursively
        mergeSort(arr, left, middle, nutrientIndex);
        mergeSort(arr, middle + 1, right, nutrientIndex);

        // Merge the sorted halves
        merge(arr, left, middle, right, nutrientIndex);
    }
}

// Function to sort foodList array in descending order based on given nutrient index
void FoodList::mergeSortFoodList(int nutrientIndex)
{
    timeStart();
    mergeSort(foodList, 0, 7077, nutrientIndex);
    timeStopAndDisplay("merge");
}

// Function to sort the foodList array in descending order according to given nutrient index
void FoodList::shellSortFoodList(int nutrientIndex) {
    timeStart();
    int n = 7078; // size of the foodList array

    // Calculate gap for shell sort
    for (int gap = n / 2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < n; i++)
        {
            Food temp = foodList[i];
            int j;
            for (j = i; j >= gap && foodList[j - gap].nutrients[nutrientIndex] < temp.nutrients[nutrientIndex]; j -= gap)
            {
                foodList[j] = foodList[j - gap];
            }
            foodList[j] = temp;
        }
    }
    timeStopAndDisplay("shell");
}

//	Starts the timer
void FoodList::timeStart() {
	//	Record start time
	start = steady_clock::now();

}

//	Ends the timer and displays the time it ran for
void FoodList::timeStopAndDisplay(string sortType){
	//	Record end time
	end = steady_clock::now();
	
	//	Calculate the duration
	duration<double> elapsed_seconds = duration_cast<duration<double>>(end - start);

	//	Saves the sort time of each sorting algorithm as string
	if (sortType == "shell") {
		shellSortTime = to_string(elapsed_seconds.count());
	}
	else if (sortType == "merge") {
		mergeSortTime = to_string(elapsed_seconds.count());
	}

	//	Output the elapsed time
	cout << "Elapsed time: " << elapsed_seconds.count() << " seconds" << endl; // DEBUG
}







