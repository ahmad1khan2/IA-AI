#pragma once
#include <string>
#include <memory>
#include "ChatState.h"
#include "MainState.h"

using namespace std;

class StateManager {
private:
    unique_ptr<ChatState> currentState;

public:
    StateManager();
    void handleInput(const string& input);
    void changeState(unique_ptr<ChatState> newState);
    void returnToMain();
};