#pragma once
#include "ChatState.h"
#include "home.h"

class HomeState : public ChatState {
private:
    HomeLoanHandler handler;
    std::string currentPrompt;

public:
    void handleInput(const std::string& input) override;
    void onEnter() override;
    std::string getPrompt() override { return currentPrompt; }

private:
    void updatePrompt();
};