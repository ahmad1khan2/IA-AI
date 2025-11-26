#include "PersonalState.h"
#include <iostream>

using namespace std;

void PersonalState::onEnter() {
    currentPrompt = "Personal loans > ";
    cout << "Chatbot: I am happy to help you with personal loan options. Please type 'show' to look at the available options or press 'menu' to explore other loans." << endl;
}

void PersonalState::handleInput(const string& input) {
    handler.handleInput(input);
    updatePrompt();
}

void PersonalState::updatePrompt() {
    string state = handler.getCurrentState();
    if (state == "main") {
        currentPrompt = "Personal loans > ";
    }
    else if (state == "personal_loan_select") {
        currentPrompt = "Select loan type > ";
    }
}