#include "Sorter.h"
#include <algorithm>
#include <iostream>
using namespace std;

// Constructor initializes the nutrientIDs vector
//Sorter::Sorter(vector<Nutrient>& nutrientIDs) : nutrientIDs(nutrientIDs) {}
/*
// Constructor initializes the nutrientIDs vector with a single nutrientId
Sorter::Sorter(int nutrientId) : nutrientIDs({ nutrientId }) {}

// Compare function to compare Nutrient objects by nutrientId
bool Sorter::compare(const Nutrient& a, const Nutrient& b) {
    double sumA = 0;
    double sumB = 0;
    // loop through nutrientIDs vector
    for (int i = 0; i < nutrientIDs.size(); i++) {
        sumA += a.data[nutrientIDs[i]];
        sumB += b.data[nutrientIDs[i]];
    }
    return sumA < sumB;
}

// Merge sort implementation
void Sorter::mergeSortHelper(int left, int right) {
    if (left >= right) {
        return;
    }

    int mid = left + (right - left) / 2;
    mergeSortHelper(left, mid);
    mergeSortHelper(mid + 1, right);
    merge(left, mid, right);
}

void Sorter::merge(int left, int mid, int right) {
    vector<Nutrient> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;

    while (i <= mid && j <= right) {
        if (compareNutrient(m_nutrients[i], m_nutrients[j])) {
            temp[k++] = m_nutrients[i++];
        }
        else {
            temp[k++] = m_nutrients[j++];
        }
    }

    while (i <= mid) {
        temp[k++] = m_nutrients[i++];
    }

    while (j <= right) {
        temp[k++] = m_nutrients[j++];
    }

    for (int p = 0; p < k; ++p) {
        m_nutrients[left + p] = temp[p];
    }
}

// Public function to sort the m_nutrients vector using merge sort
void Sorter::mergeSort() {
    mergeSortHelper(0, m_nutrients.size() - 1);
}

*/