#pragma once
#include "ChatState.h"
#include "cnic.h"

class CNICState : public ChatState {
public:
    void handleInput(const std::string& input) override;
    void onEnter() override;
    std::string getPrompt() override { return "Enter CNIC (13 digits): "; }
};