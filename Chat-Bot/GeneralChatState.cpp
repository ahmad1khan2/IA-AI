#include "GeneralChatState.h"
#include <iostream>

using namespace std;

void GeneralChatState::onEnter() {
    currentPrompt = "General Chat > ";
    cout << "Chatbot: Let's have a conversation! I can chat about various topics." << endl;
    cout << "Chatbot: Type 'exit' or 'menu' to return to the main menu anytime." << endl;
}

void GeneralChatState::handleInput(const string& input) {
    handler.handleInput(input);
    updatePrompt();
}

void GeneralChatState::updatePrompt() {
    string state = handler.getCurrentState();
    if (state == "chatting") {
        currentPrompt = "General Chat > ";
    }
    else if (state == "exiting") {
        currentPrompt = "Main Menu > ";
    }
}