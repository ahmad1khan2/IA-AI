#include "CarState.h"
#include <iostream>

using namespace std;

void CarState::onEnter() {
    currentPrompt = "Car loans > ";
    cout << "Chatbot: I am happy to help you with the car options. Please type 'show' to look at the availaible options or press 'menu' to explore other loans." << endl;
}

void CarState::handleInput(const string& input) {
    handler.handleInput(input);
    updatePrompt();
}

void CarState::updatePrompt() {
    string state = handler.getCurrentState();
    if (state == "main") {
        currentPrompt = "Car loans > ";
    } else if (state == "car_loan_select") {
        currentPrompt = "Select car > ";
    }
}