#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <SFML/Graphics.hpp>
#include <map>
#include "FoodList.h"
#include "GUI.h"
using namespace std;

// Function to split a string into tokens based on a delimiter
vector<string> split(const string& s, char delimiter);


int main() {
    
    // Open the CSV file for reading
    ifstream file("assets/food.csv");
    if (!file.is_open()) {
        cerr << "Failed to open file!" << endl;
        return 1;
    }


    string line;

    FoodList foodList = FoodList();

    // Sets Map of nutrient key using the first line in the csv file
    getline(file, line);
    vector<string> nutrientNames = split(line, ',');
    nutrientNames.erase(nutrientNames.begin(), nutrientNames.begin()+3);
    foodList.nutrientNames = nutrientNames;

    int index = 0;
    // Reads entire file
    while (getline(file, line)) {
        // Case 1: Line contains ""
        if (line.find("\"") != string::npos) {

            // Stores Food category, name, and id
            int parsePosition = line.find(",\"");
            foodList.foodList[index].category = line.substr(0, parsePosition);
            line = line.substr(parsePosition + 1);

            parsePosition = line.find("\",");
            foodList.foodList[index].name = line.substr(1, parsePosition - 1);
            line = line.substr(parsePosition + 2);

            parsePosition = line.find(",");
            foodList.foodList[index].id = stoi(line.substr(0, parsePosition));
            line = line.substr(parsePosition + 1);

            // Parses and stores the nutrients
            vector<string> nutrients = split(line, ',');

            int i = 0;
            for (auto iter : nutrients) {
                foodList.foodList[index].nutrients[i] = stod(iter);
                i++;
            }
        }
        // Case 2: Lines does not contain ""
        else {
            vector<string> parsedLine = split(line, ',');

            foodList.foodList[index].category = parsedLine[0];
            foodList.foodList[index].name = parsedLine[1];
            foodList.foodList[index].id = stoi(parsedLine[2]);

            for (int i = 0; i < parsedLine.size() - 3; i++) {
                foodList.foodList[index].nutrients[i] = stod(parsedLine[i + 3]);
                i++;
            }
        }

        // Limits name length to not overflow
        if (foodList.foodList[index].name.size() > 30) {
            foodList.foodList[index].name.resize(30);
        }
        index++;
    }

    // Close the csv file
    file.close();

    
    // pass foodlist pointer to GUI object
    GUI gui(&foodList);

    // run the GUI
    gui.run();

    return 0;
}

////////////////////////    Helper Function Definitions   /////////////////////////

// Function to split a string into tokens based on a delimiter
vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    istringstream ss(s);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}