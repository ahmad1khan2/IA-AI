#include "MainState.h"
#include "fileHandling.h" // Required for Parser::readUtterances
#include <iostream>
#include <algorithm>

// Constructor
MainState::MainState() {
    readUtterances();
}

// Called when the user first enters this state
void MainState::onEnter() {
    std::cout << "Chatbot: Welcome! You can ask about home loans (H), car loans (C), "
        << "scooter loans (S), check applications (Q) or just chat (G)." << std::endl;
}

// Returns the prompt string for the main menu
std::string MainState::getPrompt() {
    return "How can I help you today? ";
}

// Helper function to load utterances
void MainState::readUtterances() {
    // Calls the static method from your fileHandling logic
    Parser::readUtterances(userInput, botReply);
}

// Main logic to handle user input
void MainState::handleInput(const std::string& input) {
    std::string normalized = input;
    std::transform(normalized.begin(), normalized.end(), normalized.begin(), ::tolower);

    // 1. Check Utterances file matches
    bool found = false;
    for (size_t i = 0; i < userInput.size(); i++) {
        std::string tempUser = userInput[i];
        std::transform(tempUser.begin(), tempUser.end(), tempUser.begin(), ::tolower);

        if (normalized == tempUser) {
            std::cout << "Chatbot: " << botReply[i] << std::endl;
            found = true;
            break;
        }
    }

    // 2. Check for Wildcard (*) if no direct match found
    if (!found) {
        for (size_t j = 0; j < userInput.size(); j++) {
            if (userInput[j] == "*") {
                std::cout << "Chatbot: " << botReply[j] << std::endl;
                found = true;
                break;
            }
        }
    }

    // 3. Default fallback if nothing matches
    if (!found) {
        std::cout << "Chatbot: I can help with home loans (H), car loans (C), "
            << "scooter loans (S), or application queries (Q). "
            << "Or type 'menu' to see options anytime." << std::endl;
    }
}