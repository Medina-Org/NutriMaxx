#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include "FoodList.h"
using namespace std;

class Textbox {
public:
    Textbox() {}

    // constructor to create textbox
    Textbox(sf::Font& font, unsigned int fontSize, sf::Vector2f position, sf::Vector2f size) {
        text.setFont(font);
        text.setCharacterSize(fontSize);
        text.setFillColor(sf::Color::Black);
        text.setString("");

        box.setPosition(position);
        box.setSize(size);
        box.setFillColor(sf::Color::White);
        box.setOutlineColor(sf::Color::Black);
        box.setOutlineThickness(1.f);

        centerText();
    }

    // set text to be displayed
    void setText(const std::string& str) {
        text.setString(str);
        centerText();
    }

    // set text to be displayed but at a specific position, instead of the default center
    void setText(const std::string& str, sf::Vector2f position) {
		text.setString(str);
		text.setPosition(position);
	}

    // set textbox to a new position
    void setPosition(sf::Vector2f position) {
        box.setPosition(position);
        centerText();
    }

    // set color scheme of textbox (box color and text color)
    void setColorScheme(sf::Color boxColor, sf::Color textColor) {
        box.setFillColor(boxColor);
        text.setFillColor(textColor);
    }

    // set color scheme of textbox (box color, text color, outline color)
    void setColorScheme(sf::Color boxColor, sf::Color textColor, sf::Color outlineColor) {
        box.setFillColor(boxColor);
        box.setOutlineColor(outlineColor);
        text.setFillColor(textColor);
    }

    // function to draw textbox to window
    void drawToWindow(sf::RenderWindow& window) {
        window.draw(box);
        window.draw(text);
    }

    // function to check if the mouse is hovering over the textbox
    bool isMouseOver(sf::RenderWindow& window) {
		sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		sf::FloatRect bounds = box.getGlobalBounds();
		return bounds.contains(mousePos);
	}

private:
    sf::Text text; // text to display
    sf::RectangleShape box; // box to display text in

    // function to center text within textbox
    void centerText() {
        sf::Vector2f boxPos = box.getPosition();
        sf::Vector2f textPos(boxPos.x + box.getSize().x / 2.f - text.getLocalBounds().width / 2.f,
            boxPos.y + box.getSize().y / 2.f - text.getLocalBounds().height / 1.5f);
        text.setPosition(textPos);
    }
};

class GUI {
public:
    GUI() {}

    // initializer for GUI
    GUI(FoodList* foodList);

    // function to run the GUI
    void run();

private:
    int searchNutrientIndex; // index of nutrient to search for
    bool selectedSort; // true if a sort option is selected
    bool selectedNutrient; // true if a nutrient is selected
    bool commenceSort; // true if a sort is to be performed
    bool doMergeSort; // true if merge sort is to be performed
    FoodList* foodList; // pointer to food list
    sf::RenderWindow window; // window to render GUI
    sf::FloatRect viewBounds; // bounds of the view for the nutrient menu
    sf::View scrollView; // view to scroll through nutrient menu
    sf::Font font; // font for text
    vector<Textbox> nutrientMenu; // vector of nutrient menu option textboxes
    map<string, Textbox> optionsMap; // map of sort option textboxes
    map<string, Textbox> infoMap; // map of info textboxes (name of nutrient displayed and sort times) 
    vector<Textbox> foodNameTextboxes; // vector of displayed food name textboxes
    vector<Textbox> foodValTextboxes; // vector of displayed food nutrient value textboxes

    // function to render the GUI
    void render();
    // function to handle events from user interaction
    void eventHandler(sf::Event event);
    // function to update apply textbox and perform sort if necessary
    void update();
    // function to update printed foods
    void updateFoods();
};