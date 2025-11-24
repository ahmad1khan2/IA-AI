#include "ScooterState.h"
#include <iostream>

using namespace std;

void ScooterState::onEnter() {
    currentPrompt = "Scooter loans > ";
    cout << "Chatbot: I am happy to help you with the scooter options. Please type 'show' to look at the availaible options or press 'menu' to explore other loans." << endl;
}

void ScooterState::handleInput(const string& input) {
    handler.handleInput(input);
    updatePrompt();
}

void ScooterState::updatePrompt() {
    string state = handler.getCurrentState();
    if (state == "main") {
        currentPrompt = "Scooter loans > ";
    } else if (state == "scooter_loan_make") {
        currentPrompt = "Select make > ";
    } else if (state == "scooter_loan_model") {
        currentPrompt = "Select model > ";
    }
}