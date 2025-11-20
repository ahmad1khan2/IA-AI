#pragma once
#include "ChatState.h"
#include "scooter.h"

class ScooterState : public ChatState {
private:
    ScooterLoanHandler handler;
    std::string currentPrompt;

public:
    void handleInput(const std::string& input) override;
    void onEnter() override;
    std::string getPrompt() override { return currentPrompt; }

private:
    void updatePrompt();
};