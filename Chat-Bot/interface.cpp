
// interface.cpp
#include <iostream>
#include <string>
#include <memory>
#include "ChatState.h"
#include "StateManager.h"

using namespace std;

int main() {
    StateManager stateManager;

    cout << "Welcome to the Loan Application ChatBot" << endl;
    
    string input;
    while (true) {
        cout << "You: ";
        getline(cin, input);

        if (input == "X" || input == "x") {
            cout << "Chatbot: Goodbye!" << endl;
            break;
        }

        // Let the state manager handle the input
        stateManager.handleInput(input);
    }

    return 0;
}




