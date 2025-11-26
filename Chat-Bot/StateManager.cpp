#include "StateManager.h"
#include "MainState.h"
#include "CarState.h"
#include "HomeState.h"
#include "ScooterState.h"
#include "PersonalState.h"
#include "CNICState.h"
#include <iostream>
#include <algorithm>

using namespace std;

// Constructor: Starts the bot in MainState
StateManager::StateManager() {
    currentState = make_unique<MainState>();
}

// Logic to detect global commands or pass input to current state
void StateManager::handleInput(const string& input) {
    // 1. Normalize input for checking global commands (H, C, S, etc.)
    string normalized = input;
    if (!normalized.empty()) {
        // Check first char safely
        normalized[0] = toupper(normalized[0]);
    }
    // transform entire string to upper for word matching (HOME, CAR)
    string upperFull = input;
    transform(upperFull.begin(), upperFull.end(), upperFull.begin(), ::toupper);

    // 2. Global Context Switching
    if (normalized == "H" || upperFull == "HOME") {
        changeState(make_unique<HomeState>());
        return;
    }
    else if (normalized == "C" || upperFull == "CAR") {
        changeState(make_unique<CarState>());
        return;
    }
    else if (normalized == "S" || upperFull == "SCOOTER") {
        changeState(make_unique<ScooterState>());
        return;
    }
    else if (normalized == "P" || upperFull == "PERSONAL") {
        changeState(make_unique<PersonalState>());
        return;
    }
    else if (normalized == "Q" || upperFull == "QUERY") {
        changeState(make_unique<CNICState>());
        return;
    }
    else if (upperFull == "MAIN" || upperFull == "MENU" || upperFull == "EXIT" || upperFull == "A") {
        returnToMain();
        return;
    }

    // 3. If no global command, pass input to the active state
    currentState->handleInput(input);
}

// Helper to swap the active state object
void StateManager::changeState(unique_ptr<ChatState> newState) {
    currentState = move(newState);
    currentState->onEnter(); // Trigger the "Welcome" message of the new state
}

// Helper to return to main menu
void StateManager::returnToMain() {
    changeState(make_unique<MainState>());
}