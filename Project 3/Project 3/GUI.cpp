#include "GUI.h"
using namespace std;


GUI::GUI(FoodList* foodList) {

    // makes the window
	window.create(sf::VideoMode(800, 600), "Nutrition Sorter");

    // loads the font
    if (!font.loadFromFile("./assets/arial.ttf")) {
        cerr << "Error loading font" << endl;
    }

    // initializes all variables
    searchNutrientIndex = 0;
    selectedSort = false;
    selectedNutrient = false;
    commenceSort = false;
    doMergeSort = false;

    // initializes the food list 
    this->foodList = foodList;

    // initializes the scroll view
    viewBounds = sf::FloatRect(400.f, 50.f, 400.f, 250.f);
    scrollView = sf::View(viewBounds);
    scrollView.setViewport(sf::FloatRect(0.5f, 0.0625f, 0.5f, 0.416666f));

    // initializes the nutrient menu by looping through all nutrients using foodList's nutrientNames vector
    for (int i = 0; i < foodList->nutrientNames.size(); i++) {
        Textbox nutrientMenuEntry(font, 24, sf::Vector2f(400.f, 50.f + (i * 30)), sf::Vector2f(300.f, 30.f));
		nutrientMenuEntry.setColorScheme(sf::Color::White, sf::Color::Black);
        nutrientMenuEntry.setText(foodList->nutrientNames[i]);
		nutrientMenu.push_back(nutrientMenuEntry);
	}

    // initializes the options
    optionsMap["Shell"] = Textbox(font, 24, sf::Vector2f(25.f, 50.f), sf::Vector2f(150.f, 50.f));
    optionsMap["Shell"].setText("Shell Sort");
    optionsMap["Merge"] = Textbox(font, 24, sf::Vector2f(200.f, 50.f), sf::Vector2f(150.f, 50.f));
    optionsMap["Merge"].setText("Merge Sort");
    optionsMap["Apply"] = Textbox(font, 24, sf::Vector2f(125.f, 200.f), sf::Vector2f(150.f, 50.f));
    optionsMap["Apply"].setText("Apply");
    // set all options to white text on gray background
    for (auto it = optionsMap.begin(); it != optionsMap.end(); it++) {
		it->second.setColorScheme(sf::Color::Color(100, 100, 100, 255), sf::Color::White);
	}

    // initializes informational text
    infoMap["Shell Time"] = Textbox(font, 20, sf::Vector2f(25.f, 100.f), sf::Vector2f(150.f, 50.f));
    infoMap["Shell Time"].setText("Time: --");
    infoMap["Merge Time"] = Textbox(font, 20, sf::Vector2f(200.f, 100.f), sf::Vector2f(150.f, 50.f));
    infoMap["Merge Time"].setText("Time: --");
    infoMap["Subtitle"] = Textbox(font, 30, sf::Vector2f(450.f, 300.f), sf::Vector2f(200.f, 50.f));
    infoMap["Subtitle"].setText(foodList->nutrientNames[searchNutrientIndex]);
    // set all informational text to black text on transparent background
    for (auto it = infoMap.begin(); it != infoMap.end(); it++) {
        it->second.setColorScheme(sf::Color::Transparent, sf::Color::Black, sf::Color::Transparent);
    }

    // initializes the data display of the top 5 foods in foodlist at the moment
    for (int i = 0; i < 5; i++) {
        // Makes a new textbox for each food name
		Textbox food(font, 24, sf::Vector2f(100.f, 350.f + (i * 50)), sf::Vector2f(600.f, 50.f));
		food.setColorScheme(sf::Color::White, sf::Color::Black, sf::Color::White);
        food.setText(foodList->foodList[i].name, sf::Vector2f(100.f, 350.f + (i * 50)));
        foodNameTextboxes.push_back(food);

        // Makes a new textbox for each food's nutrient value
        Textbox foodVal(font, 24, sf::Vector2f(400.f, 350.f + (i * 50)), sf::Vector2f(300.f, 50.f));
        foodVal.setColorScheme(sf::Color::Transparent, sf::Color::Black, sf::Color::White);
        foodVal.setText(to_string(foodList->foodList[i].nutrients[searchNutrientIndex]) + foodList->getUnit(searchNutrientIndex));
        foodValTextboxes.push_back(foodVal);
	}
}

void GUI::run() {
    while (window.isOpen()) {
        update();
        // checks for events
        sf::Event event;
        while (window.pollEvent(event)) {
            eventHandler(event);
        }
        render();
	}
}

void GUI::eventHandler(sf::Event event) {
    // closes window if close button is pressed
    if (event.type == sf::Event::Closed) {
        window.close();
    }
    // checks for mouse clicks
    if (event.type == sf::Event::MouseButtonPressed) {
        // checks if mouse is clicked on shell sort option
        if (optionsMap["Shell"].isMouseOver(window)) {
            selectedSort = true;
            doMergeSort = false;
            optionsMap["Shell"].setColorScheme(sf::Color::Color(255, 165, 0, 255), sf::Color::White);
            optionsMap["Merge"].setColorScheme(sf::Color::Color(100, 100, 100, 255), sf::Color::White);
        }
        // checks if mouse is clicked on merge sort option
        else if (optionsMap["Merge"].isMouseOver(window)) {
            selectedSort = true;
            doMergeSort = true;
            optionsMap["Merge"].setColorScheme(sf::Color::Color(255, 165, 0, 255), sf::Color::White);
            optionsMap["Shell"].setColorScheme(sf::Color::Color(100, 100, 100, 255), sf::Color::White);
        }
        // checks if mouse is clicked on apply button
        else if (optionsMap["Apply"].isMouseOver(window)) {
            if (selectedSort && selectedNutrient) {
                commenceSort = true;
                optionsMap["Apply"].setColorScheme(sf::Color::Color(100, 100, 100, 255), sf::Color::White);
                optionsMap["Apply"].setText("Sorting...");
            }
        }
        // checks if mouse is clicked on a nutrient in the nutrient menu
        else {
            // check if mouse is inside the view
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            if (viewBounds.contains(mousePosition.x, mousePosition.y)) {
                // Convert mouse position to view coordinates
                sf::Vector2f viewPos = window.mapPixelToCoords(mousePosition);
                // set view to scrollView
                window.setView(scrollView);
                for (int i = 0; i < nutrientMenu.size(); i++) {
                    // reset color of all nutrients
                    nutrientMenu[i].setColorScheme(sf::Color::White, sf::Color::Black);
                    if (nutrientMenu[i].isMouseOver(window)) {
                        // unselect nutrient if it is already selected
                        if (selectedNutrient && i == searchNutrientIndex) {
                            selectedNutrient = false;
                        }
                        // otherwise update all relevant textboxes with new nutrient selection
                        else {
                            selectedNutrient = true;
                            searchNutrientIndex = i;
                            // set color of selected nutrient to light gray
                            nutrientMenu[i].setColorScheme(sf::Color::Color(200, 200, 200, 255), sf::Color::Black);

                            // update the top five foods with new nutrient selection
                            updateFoods();
                        }
                    }
                }
            }
        }
    }
    // checks for mouse scroll
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel && viewBounds.contains(mousePosition.x, mousePosition.y)) {
        scrollView.move(0.f, -event.mouseWheelScroll.delta * 10.f); // adjusts the view accordingly
    }
}

void GUI::render() {
    window.clear(sf::Color::Color(150, 150, 150, 255));
    window.setView(window.getDefaultView());

    // loop through option textboxes and draw to window
    for (auto it = optionsMap.begin(); it != optionsMap.end(); it++) {
        it->second.drawToWindow(window);
    }

    // loop through info textboxes and draw to window
    for (auto it = infoMap.begin(); it != infoMap.end(); it++) {
        it->second.drawToWindow(window);
    }

    // loop through food textboxes
    for (int i = 0; i < 5; i++) {
        foodNameTextboxes[i].drawToWindow(window);
        foodValTextboxes[i].drawToWindow(window);
    }

    // set view to scrollView
    window.setView(scrollView);

    // loop through nutrient menu textboxes 
    for (int i = 0; i < foodList->nutrientNames.size(); i++) {
        nutrientMenu[i].drawToWindow(window);
    }

    // back to default view 
    window.setView(window.getDefaultView());

    // finally display everything
    window.display(); 
}

void GUI::update() {
    // if apply was selected, sort the food list
    if (commenceSort) {
        // perform selected sort and display the time taken
        if (doMergeSort) {
			foodList->mergeSortFoodList(searchNutrientIndex);
            infoMap["Merge Time"].setText("Time: " + foodList->mergeSortTime + " s");
		}
        else {
			foodList->shellSortFoodList(searchNutrientIndex);
            infoMap["Shell Time"].setText("Time: " + foodList->shellSortTime + " s");
		}

        // update the printed foods
        updateFoods();

        // change apply button text to "Apply" and default color scheme
		optionsMap["Apply"].setText("Apply");
		optionsMap["Apply"].setColorScheme(sf::Color::Color(100, 100, 100, 255), sf::Color::White);

        // clear all selections 
		commenceSort = false;
        selectedSort = false;
        selectedNutrient = false;

        // reset color of all sort options
        optionsMap["Merge"].setColorScheme(sf::Color::Color(100, 100, 100, 255), sf::Color::White);
        optionsMap["Shell"].setColorScheme(sf::Color::Color(100, 100, 100, 255), sf::Color::White);

        // reset color of all nutrients
        for (int i = 0; i < nutrientMenu.size(); i++) {
            nutrientMenu[i].setColorScheme(sf::Color::White, sf::Color::Black);
        }
	} else {
        // highlight Apply button when user is allowed to select it
        if (selectedNutrient && selectedSort) {
            optionsMap["Apply"].setColorScheme(sf::Color::Color(244, 67, 54, 255), sf::Color::White);
        }
        else {
            optionsMap["Apply"].setColorScheme(sf::Color::Color(100, 100, 100, 255), sf::Color::White);
        }
	}
}

void GUI::updateFoods() {
    // update the top five foods by changing subtitle and the food names and nutrient values
	infoMap["Subtitle"].setText(foodList->nutrientNames[searchNutrientIndex]);
    for (int i = 0; i < 5; i++) {
        foodNameTextboxes[i].setText(foodList->foodList[i].name, sf::Vector2f(100.f, 350.f + (i * 50)));
		foodValTextboxes[i].setText(to_string(foodList->foodList[i].nutrients[searchNutrientIndex]) + foodList->getUnit(searchNutrientIndex));
	}
}