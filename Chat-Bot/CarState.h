#pragma once
#include "ChatState.h"
#include "Car.h"

class CarState : public ChatState {
private:
    CarLoanHandler handler;
    std::string currentPrompt;

public:
    void handleInput(const std::string& input) override;
    void onEnter() override;
    std::string getPrompt() override { return currentPrompt; }

private:
    void updatePrompt();
};