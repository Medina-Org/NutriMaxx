#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
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

void centerText(sf::Text& text, sf::RectangleShape& box) {
    sf::Vector2f boxPos = box.getPosition();
    sf::Vector2f textPos(boxPos.x + box.getSize().x / 2.f - text.getLocalBounds().width / 2.f,
        boxPos.y + box.getSize().y / 2.f - text.getLocalBounds().height / 1.5f);
    text.setPosition(textPos);
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



    ///////////////// GUI /////////////////
    bool selectedSort = false;
    bool selectedNutrient = false;
    bool commenceSort = false;

    // Create shell sort button
    sf::RectangleShape button1(sf::Vector2f(150.f, 50.f));
    button1.setPosition(25.f, 50.f);
    button1.setFillColor(sf::Color::Color(100, 100, 100, 255));
    sf::Text button1Text("Shell Sort", font, 20);
    button1Text.setFillColor(sf::Color::White);
    centerText(button1Text, button1);

    // create merge sort button
    sf::RectangleShape button2(sf::Vector2f(150.f, 50.f));
    button2.setPosition(200.f, 50.f);
    button2.setFillColor(sf::Color::Color(100, 100, 100, 255));
    sf::Text button2Text("Merge Sort", font, 20);
    button2Text.setFillColor(sf::Color::White);
    centerText(button2Text, button2);

    // create apply button
    sf::RectangleShape button3(sf::Vector2f(150.f, 50.f));
    button3.setPosition(125.f, 200.f);
    button3.setFillColor(sf::Color::Color(100, 100, 100, 255));
    sf::Text button3Text("Apply", font, 20);
    button3Text.setFillColor(sf::Color::White);
    centerText(button3Text, button3);

    // create time text
    sf::Text mergeTime("Time: --", font, 20);
    mergeTime.setFillColor(sf::Color::Black);
    sf::RectangleShape timeBox(sf::Vector2f(150.f, 50.f));
    timeBox.setPosition(25.f, 100.f);
    centerText(mergeTime, timeBox);
    sf::Text shellTime("Time: --", font, 20);
    shellTime.setFillColor(sf::Color::Black);
    timeBox.setPosition(200.f, 100.f);
    centerText(shellTime, timeBox);

    // create nutrient menu
    vector<sf::Text> textnutrientNames;
    vector<sf::RectangleShape> nameRects;
    for (int i = 0; i < nutrientNames.size(); ++i) {
        // create rectanglular frame for each nutrient name
        sf::RectangleShape rect(sf::Vector2f(300.f, 30.f));
        rect.setPosition(400.f, 50.f + (i * 30));
        rect.setFillColor(sf::Color::White);
        rect.setOutlineColor(sf::Color::Black);
        rect.setOutlineThickness(1.f);
        nameRects.push_back(rect);
        // convert the nutrientNames into a list of sf::Text objects
        sf::Text name;
        name.setFont(font);
        name.setCharacterSize(24);
        name.setFillColor(sf::Color::Black);
        name.setString(nutrientNames[i]);
        centerText(name, rect);
        textnutrientNames.push_back(name);
    }

    // helper rectangle to center text in a rectangle
    sf::RectangleShape temp(sf::Vector2f(300.f, 50.f));

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
        temp.setPosition(400.f, 350.f + (i * 50));
        centerText(nutrientVals, temp);
        sortNutrientVals.push_back(nutrientVals);
    }

    // create a subtitle
    sf::RectangleShape subtitleBox(sf::Vector2f(200.f, 50.f));
    subtitleBox.setPosition(450.f, 300.f);
    sf::Text subtitle;
    subtitle.setFont(font);
    subtitle.setCharacterSize(30);
    subtitle.setFillColor(sf::Color::Black);
    subtitle.setString(nutrientNames[1]);
    centerText(subtitle, subtitleBox);


    // create a scrollable view for the nutrient names rectangles
    sf::FloatRect viewBounds(400.f, 50.f, 400.f, 250.f);
    sf::View view(viewBounds);
    view.setViewport(sf::FloatRect(0.5f, 0.0625f, 0.5f, 0.416666f));

    // Start the main game loop
    while (window.isOpen()) {
        if (selectedNutrient && selectedSort) {
            button3.setFillColor(sf::Color::Color(244, 67, 54, 255));
        } else {
			button3.setFillColor(sf::Color::Color(100, 100, 100, 255));
		}

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
                            selectedSort = true;
                            cout << "Shell Sort" << endl;

                            /// TODO: Update sorting scheme to shell sort here

                        }
                        // select merge sort
                        else if (button2.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                            button1.setFillColor(sf::Color::Color(100, 100, 100, 255));
                            button2.setFillColor(sf::Color::Color(255, 165, 0, 255));
                            selectedSort = true;
                            cout << "Merge Sort" << endl;

                            /// TODO: Update sorting scheme to merge sort here

                        }
                        else if (button3.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                            commenceSort = true;
                            cout << "applying the sort" << endl;
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
                                            // loop through all name rects and set fill color to white
                                            for (int j = 0; j < nameRects.size(); j++) {
                                                nameRects[j].setFillColor(sf::Color::White);
                                            }
                                            nameRects[i].setFillColor(sf::Color::Color(200, 200, 200, 255));
                                            subtitle.setString(nutrientNames[i]);
                                            centerText(subtitle, subtitleBox);
                                            for (int j = 0; j < 5; ++j) {
                                                sortNutrientVals[j].setString(to_string(foodList.foodList[j].nutrients[i]));
                                            }
                                            selectedNutrient = true;
                                            cout << nutrientNames[i] << endl;

                                            /// TODO: nutrient to sort by to index i

                                        }
                                        else {
                                            nameRects[i].setFillColor(sf::Color::White);
                                            selectedNutrient = false;
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
        window.clear(sf::Color::Color(150, 150, 150, 255));
        // Draw everything to the screen
        window.setView(window.getDefaultView());
        window.draw(button1);
        window.draw(button1Text);
        window.draw(button2);
        window.draw(button2Text);
        window.draw(mergeTime);
        window.draw(shellTime);
        window.draw(subtitle);
        for (int i = 0; i < 5; ++i) {
            window.draw(sortRects[i]);
            window.draw(sortNames[i]);
            window.draw(sortNutrientVals[i]);
        }

        if (commenceSort) {
            button3.setFillColor(sf::Color::Color(100, 100, 100, 255));
            button3Text.setString("Loading...");
            centerText(button3Text, button3);
        }
        window.draw(button3);
        window.draw(button3Text);

        // Draw the scrollable rectangles and text
        window.setView(view);
        for (int i = 0; i < nutrientNames.size(); ++i) {
            window.draw(nameRects[i]);
            window.draw(textnutrientNames[i]);
        }
        window.display();

        if (commenceSort) {

            /// TODO: CALL SORT HERE
            double mergeTimeVal = 0.0;
            double shellTimeVal = 0.0;


            mergeTime.setString("Time:" + to_string(mergeTimeVal));
            shellTime.setString("Time:" + to_string(shellTimeVal));
            timeBox.setPosition(25.f, 100.f);
            centerText(mergeTime, timeBox);
            timeBox.setPosition(200.f, 100.f);
            centerText(shellTime, timeBox);
            commenceSort = false;
            selectedNutrient = false;
            selectedSort = false;
            button1.setFillColor(sf::Color::Color(100, 100, 100, 255));
            button2.setFillColor(sf::Color::Color(100, 100, 100, 255));
            for (int j = 0; j < nameRects.size(); j++) {
                nameRects[j].setFillColor(sf::Color::White);
            } 

            button3Text.setString("Apply");
            centerText(button3Text, button3);
        }
    }

    return 0;
}