#pragma once
#ifndef GENERALCHATSTATE_H
#define GENERALCHATSTATE_H

#include "ChatState.h"
#include "GeneralChat.h"
#include <string>

class GeneralChatState : public ChatState {
private:
    GeneralChatHandler handler;
    std::string currentPrompt;

public:
    void handleInput(const std::string& input) override;
    void onEnter() override;
    std::string getPrompt() override { return currentPrompt; }

private:
    void updatePrompt();
};

#endif
