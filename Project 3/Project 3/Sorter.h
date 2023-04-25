#pragma once
#include <vector>
#include <string>
using namespace std;

struct Nutrient {
        int nutrientId;
        vector<double> data;
    };
/*
class Sorter {
public:
    Sorter(vector<int>& nutrientIDs);
    Sorter(int nutrientIDs);

    void mergeSort();
    void shellSort();

    vector<int> nutrientIDs;
    // array of Nutrient objects
    Nutrient* dataset;

private:
    void mergeSortHelper(int left, int right);
    void merge(int left, int mid, int right);
    bool compare(const Nutrient& a, const Nutrient& b);
};
*/