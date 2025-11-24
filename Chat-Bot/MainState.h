#pragma once
#ifndef MAINSTATE_H
#define MAINSTATE_H

#include "ChatState.h"
#include <vector>
#include <string>

class MainState : public ChatState {
private:
    std::vector<std::string> userInput;
    std::vector<std::string> botReply;

    // Private helper to load data
    void readUtterances();

public:
    MainState();

    // Override functions from ChatState
    void handleInput(const std::string& input) override;
    void onEnter() override;
    std::string getPrompt() override;
};

#endif // MAINSTATE_H