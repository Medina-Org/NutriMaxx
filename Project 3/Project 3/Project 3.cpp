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

    // Create the SFML window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");

    // Loading the font from a ttf file
    sf::Font font;
    if (!font.loadFromFile("./assets/arial.ttf"))
    {
        cerr << "Error loading font" << endl;
        return 1;
    }

    string line;

    // input 
    //string inputString;
    //cout << "Enter input string: ";
    //getline(cin, inputString);



    FoodList foodList = FoodList();

    // Creates Map of nutrient key from the first line in the csv file
    getline(file, line);
    vector<string> nutrientNames = split(line, ',');
    nutrientNames.erase(nutrientNames.begin(), nutrientNames.begin()+3);
    map<string, int> nutrientKey;
    int iter = 0;
    // iterates through nutrientNames vector and adds each nutrient to the map
    for (int i = 0; i < nutrientNames.size(); i++) {
        size_t last_period_pos = nutrientNames[i].find_last_of('.');
        nutrientNames[i] = nutrientNames[i].substr(last_period_pos + 1);
		nutrientKey[nutrientNames[i]] = iter;
        iter++;
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

    // close the csv file
    file.close();



    ///////////////// GRAPHICS /////////////////

    // Create shell sort button
    sf::RectangleShape button1(sf::Vector2f(150.f, 50.f));
    button1.setPosition(25.f, 50.f);
    button1.setFillColor(sf::Color::Color(100, 100, 100, 255));
    sf::Text button1Text("Shell Sort", font, 20);
    button1Text.setFillColor(sf::Color::White);
    sf::Vector2f button1Pos = button1.getPosition();
    sf::Vector2f button1TextPos(button1Pos.x + button1.getSize().x / 2.f - button1Text.getLocalBounds().width / 2.f,
        button1Pos.y + button1.getSize().y / 2.f - button1Text.getLocalBounds().height / 2.f);
    button1Text.setPosition(button1TextPos);

    // create merge sort button
    sf::RectangleShape button2(sf::Vector2f(150.f, 50.f));
    button2.setPosition(200.f, 50.f);
    button2.setFillColor(sf::Color::Color(100, 100, 100, 255));
    sf::Text button2Text("Merge Sort", font, 20);
    button2Text.setFillColor(sf::Color::White);
    sf::Vector2f button2Pos = button2.getPosition();
    sf::Vector2f button2TextPos(button2Pos.x + button2.getSize().x / 2.f - button2Text.getLocalBounds().width / 2.f,
        		button2Pos.y + button2.getSize().y / 2.f - button2Text.getLocalBounds().height / 2.f);
    button2Text.setPosition(button2TextPos);


    // convert the nutrientNames into a list of sf::Text objects
    vector<sf::Text> textnutrientNames;
    for (int i = 0; i < nutrientNames.size(); ++i) {
        sf::Text name;
        name.setFont(font);
        name.setCharacterSize(24);
        name.setFillColor(sf::Color::Black);
        name.setString(nutrientNames[i]);
        name.setPosition(400.f, 50.f + (i * 30));
        textnutrientNames.push_back(name);
    }

    // create rectanglular frame for each nutrient name
    vector<sf::RectangleShape> nameRects;
    for (int i = 0; i < nutrientNames.size(); ++i) {
        sf::RectangleShape rect(sf::Vector2f(300.f, 30.f));
        rect.setPosition(400.f, 50.f + (i * 30));
        rect.setFillColor(sf::Color::White);
        rect.setOutlineColor(sf::Color::Black);
        rect.setOutlineThickness(1.f);
        nameRects.push_back(rect);
    }

    // make ten textboxes in the bottom of the window
    vector<sf::RectangleShape> sortRects;
    vector<sf::Text> sortNames;
    vector<sf::Text> sortNutrientVals;
    for (int i = 0; i < 5; ++i) {
        // make a rectangle for each textbox
        sf::RectangleShape rect(sf::Vector2f(600.f, 50.f));
        rect.setPosition(100.f, 350.f + (i * 50));
        rect.setFillColor(sf::Color::White);
        rect.setOutlineColor(sf::Color::Black);
        rect.setOutlineThickness(1.f);
        sortRects.push_back(rect);
        // make the text
        sf::Text name;
        name.setFont(font);
        name.setCharacterSize(24);
        name.setFillColor(sf::Color::Black);
        name.setString(foodList.foodList[i].name);
        name.setPosition(100.f, 350.f + (i * 50));
        sortNames.push_back(name);
        // make the nutrient values
        sf::Text nutrientVals;
        nutrientVals.setFont(font);
        nutrientVals.setCharacterSize(24);
        nutrientVals.setFillColor(sf::Color::Black);
        nutrientVals.setString(to_string(foodList.foodList[i].nutrients[1]));
        nutrientVals.setPosition(500.f, 350.f + (i * 50));
        sortNutrientVals.push_back(nutrientVals);
    }

    // create a subtitle
    sf::Text subtitle;
    subtitle.setFont(font);
    subtitle.setCharacterSize(30);
    subtitle.setFillColor(sf::Color::Black);
    subtitle.setString(nutrientNames[1]);
    subtitle.setPosition(500.f, 300.f);


    // create a scrollable view for the nutrient names rectangles
    sf::FloatRect viewBounds(400.f, 50.f, 400.f, 250.f);
    sf::View view(viewBounds);
    view.setViewport(sf::FloatRect(0.5f, 0.0625f, 0.5f, 0.416666f));

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
                        // select shell sort
                        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                        if (button1.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                            button1.setFillColor(sf::Color::Color(255, 165, 0, 255));
                            button2.setFillColor(sf::Color::Color(100, 100, 100, 255));
                            cout << "Shell Sort" << endl;
                        }
                        // select merge sort
                        else if (button2.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                            button1.setFillColor(sf::Color::Color(100, 100, 100, 255));
                            button2.setFillColor(sf::Color::Color(255, 165, 0, 255));
                            cout << "Merge Sort" << endl;
                        }
                        else {
                            // select one of the nutrientNames
                            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                            // check if mouse is clicking inside view bounds
                            if (viewBounds.contains(mousePosition.x, mousePosition.y)) {
                                // get the current mouse position in the window
                                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                                // convert it to view coordinates
                                sf::Vector2f viewPos = window.mapPixelToCoords(pixelPos);
                                for (int i = 0; i < nameRects.size(); ++i) {
                                    if (nameRects[i].getGlobalBounds().contains(viewPos)) {
                                        if (nameRects[i].getFillColor() == sf::Color::White) {
                                            nameRects[i].setFillColor(sf::Color::Color(200, 200, 200, 255));
                                            subtitle.setString(nutrientNames[i]);
                                            for (int j = 0; j < 5; ++j) {
                                                sortNutrientVals[j].setString(to_string(foodList.foodList[j].nutrients[i]));
                                            }
                                            cout << nutrientNames[i] << endl;
                                        }
                                        else {
                                            nameRects[i].setFillColor(sf::Color::White);
                                            cout << "Removed " << nutrientNames[i] << endl;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    break;
                case sf::Event::MouseWheelScrolled:
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel && viewBounds.contains(mousePosition.x, mousePosition.y)) {
                        view.move(0.f, -event.mouseWheelScroll.delta * 10.f);
                    }
                    break;
            }
        }

        // Draw everything to the screen
        window.clear(sf::Color::Color(150, 150, 150, 255));
        window.setView(window.getDefaultView());
        window.draw(button1);
        window.draw(button1Text);
        window.draw(button2);
        window.draw(button2Text);
        window.draw(subtitle);
        for (int i = 0; i < 5; ++i) {
            window.draw(sortRects[i]);
            window.draw(sortNames[i]);
            window.draw(sortNutrientVals[i]);
        }
        // Draw the rectangles and text
        window.setView(view);
        for (int i = 0; i < nutrientNames.size(); ++i) {
            window.draw(nameRects[i]);
            window.draw(textnutrientNames[i]);
        }
        window.display();
    }

    return 0;
}