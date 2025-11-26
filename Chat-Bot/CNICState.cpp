#include "CNICState.h"
#include <iostream>
#include <algorithm>

using namespace std;

void CNICState::onEnter() {
    cout << "=== CNIC APPLICATION QUERY ===" << endl;
    queryApplicationsByCNIC();
    cout << "\n=================================" << endl;
    cout << "Chatbot: Query completed. Type 'menu' to return to main menu." << endl;
    cout << "You can also type H for home loans, C for car loans, S for scooter loans," << endl;
    cout << "or G for general chat. Type X to exit." << endl;
}

void CNICState::handleInput(const string& input) {
    // CNIC state will handle global commands through StateManager
    // Just show a prompt that we're still in CNIC state
    cout << "Chatbot: Type 'menu' to return to main menu or use other commands." << endl;
}

