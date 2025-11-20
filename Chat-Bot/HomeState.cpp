#include "HomeState.h"
#include <iostream>

using namespace std;

void HomeState::onEnter() {
    currentPrompt = "Home loans > ";
    cout << "Chatbot: I am happy to help you with the home options. Please type 'show' to look at the availaible options or press 'menu' to explore other loans." << endl;
}

void HomeState::handleInput(const string& input) {
    handler.handleInput(input);
    updatePrompt();
}

void HomeState::updatePrompt() {
    string state = handler.getCurrentState();
    if (state == "main") {
        currentPrompt = "Home loans > ";
    } else if (state == "home_loan_area") {
        currentPrompt = "Select area (1-4) > ";
    } else if (state == "home_loan_select") {
        currentPrompt = "Select home > ";
    }
}