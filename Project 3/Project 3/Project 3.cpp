#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <map>
#include "FoodList.h"
using namespace std;

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

int main() {
    
    // Open the CSV file for reading
    ifstream file("assets/food.csv");
    if (!file.is_open()) {
        cerr << "Failed to open file!" << endl;
        return 1;
    }

    string line;
    string inputString;
    cout << "Enter input string: ";
    getline(cin, inputString);
    map<string, int> nutrientKey;

    FoodList foodList = FoodList();

    // Creates Map of nutrient key from the first line in the csv file
    getline(file, line);
    vector<string> temp = split(line, ',');

    int i = 0;
    for (auto iter : temp) {
        nutrientKey[iter] = i;
        i++;
    }

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
        index++;
    }

    file.close();

    
    

    // Create the SFML window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");

    // Create two buttons
    sf::RectangleShape button1(sf::Vector2f(100.f, 50.f));
    button1.setPosition(50.f, 50.f);
    button1.setFillColor(sf::Color::Color(100, 100, 100, 255));
    
    sf::RectangleShape button2(sf::Vector2f(100.f, 50.f));
    button2.setPosition(50.f, 120.f);
    button2.setFillColor(sf::Color::Color(100, 100, 100, 255));

    // Create a scrollable list with multiple options
    vector<string> options = {"Nutrient 1", "Nutrient 2", "Nutrient 3", "Nutrient 4", "Nutrient 5", "Nutrient 6", "Nutrient 7", "Nutrient 8", "Nutrient 9"};
    sf::Font font;
    if (!font.loadFromFile("./assets/arial.ttf"))
    {
        cerr << "Error loading font" << endl;
        return 1;
    }

    // convert the options into a list of sf::Text objects
    vector<sf::Text> textOptions;
    for (int i = 0; i < options.size(); ++i) {
        sf::Text option;
        option.setFont(font);
        option.setCharacterSize(24);
        option.setFillColor(sf::Color::Black);
        option.setString(options[i]);
        option.setPosition(500.f, 50.f + (i * 30));
        textOptions.push_back(option);
    }

    // create rectanglular object for each option
    vector<sf::RectangleShape> optionRects;
    for (int i = 0; i < options.size(); ++i) {
        sf::RectangleShape rect(sf::Vector2f(200.f, 30.f));
        rect.setPosition(500.f, 50.f + (i * 30));
        rect.setFillColor(sf::Color::White);
        rect.setOutlineColor(sf::Color::Black);
        rect.setOutlineThickness(1.f);
        optionRects.push_back(rect);
    }

    // create default view
    //sf::View 

    // create a scrollable view for the options rectangles
    sf::View view(sf::FloatRect(500.f, 50.f, 500.f, 200.f));
    view.setViewport(sf::FloatRect(0.625f, 0.0625f, 0.625f, 0.3333f));

    //sf::RectangleShape scrollBar(sf::Vector2f(10.f, 100.f));
    //scrollBar.setPosition(700.f, 50.f);
    //scrollBar.setFillColor(sf::Color::Red);

    // Start the main game loop
    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch(event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                        if (button1.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                            cout << "Button 1 pressed" << endl;
                        }
                        else if (button2.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                            cout << "Button 2 pressed" << endl;
                        }
                    }
                    break;
                case sf::Event::MouseWheelScrolled:
                    if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                        view.move(0.f, -event.mouseWheelScroll.delta * 10.f);
                    }
                    break;
            }
        }

        // Draw everything to the screen
        window.clear(sf::Color::Black);
        window.setView(view);
        window.setView(window.getDefaultView());
        window.draw(button1);
        window.draw(button2);
        // Draw the rectangles and text
        for (int i = 0; i < options.size(); ++i) {
            window.draw(optionRects[i]);
            window.draw(textOptions[i]);
        }
        window.display();
    }

    return 0;
}