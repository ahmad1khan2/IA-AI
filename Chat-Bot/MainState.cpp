#include "MainState.h"
#include "fileHandling.h" // Required for Parser::readUtterances
#include <iostream>
#include <algorithm>

using namespace std;

// Constructor
MainState::MainState() {
    readUtterances();
}

// Called when the user first enters this state
void MainState::onEnter() {
    cout << "Chatbot: Welcome! You can ask about home loans (H), car loans (C), "
        << "scooter loans (S), personal loans (P), or check applications (Q)." << endl;
}

// Returns the prompt string for the main menu
string MainState::getPrompt() {
    return "How can I help you today? ";
}

// Helper function to load utterances
void MainState::readUtterances() {
    // Calls the static method from your fileHandling logic
    Parser::readUtterances(userInput, botReply);
}

// Main logic to handle user input
void MainState::handleInput(const string& input) {
    string normalized = input;
    transform(normalized.begin(), normalized.end(), normalized.begin(), ::tolower);

    // 1. Check Utterances file matches
    bool found = false;
    for (size_t i = 0; i < userInput.size(); i++) {
        string tempUser = userInput[i];
        transform(tempUser.begin(), tempUser.end(), tempUser.begin(), ::tolower);

        if (normalized == tempUser) {
            cout << "Chatbot: " << botReply[i] << endl;
            found = true;
            break;
        }
    }

    // 2. Check for Wildcard (*) if no direct match found
    if (!found) {
        for (size_t j = 0; j < userInput.size(); j++) {
            if (userInput[j] == "*") {
                cout << "Chatbot: " << botReply[j] << endl;
                found = true;
                break;
            }
        }
    }

    // 3. Default fallback if nothing matches
    if (!found) {
        cout << "Chatbot: I can help with home loans (H), car loans (C), "
            << "scooter loans (S), personal loans (P), or application queries (Q). "
            << "Or type 'menu' to see options anytime." << endl;
    }
}