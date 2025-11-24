
#pragma once
#include <string>

class ChatState {
public:
    virtual ~ChatState() = default;
    virtual void handleInput(const std::string& input) = 0;
    virtual void onEnter() = 0;
    virtual std::string getPrompt() = 0;
};