#pragma once
#pragma once
#include "ChatState.h"
#include "Personal.h"

class PersonalState : public ChatState {
private:
    PersonalLoanHandler handler;
    std::string currentPrompt;

public:
    void handleInput(const std::string& input) override;
    void onEnter() override;
    std::string getPrompt() override { return currentPrompt; }

private:
    void updatePrompt();
};