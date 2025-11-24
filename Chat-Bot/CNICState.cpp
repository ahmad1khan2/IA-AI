#include "CNICState.h"
#include <iostream>

using namespace std;

void CNICState::onEnter() {
    queryApplicationsByCNIC();
    cout << "Chatbot: CNIC query completed. Type A to explore Loan Options." << endl;
}

void CNICState::handleInput(const string& input) {
    // CNIC state is typically one-shot, but we can handle menu command
    if (input == "menu" || input == "back") {
        cout << "Returning to main menu..." << endl;
    } else {
        cout << "Chatbot: Type 'menu' to return to main menu." << endl;
    }
}