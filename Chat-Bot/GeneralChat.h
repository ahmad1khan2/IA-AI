#pragma once
#ifndef GENERALCHAT_H
#define GENERALCHAT_H

#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include "fileHandling.h"

using namespace std;

class GeneralChatHandler {
public:
    GeneralChatHandler();

    void handleInput(const string& input);
    string getCurrentState() const { return chatState; }

private:
    string chatState;

    // Corpus data
    vector<string> human1Lines;
    vector<string> human2Lines;

    // Tokenization and matching
    set<string> tokenize(const string& text);
    double calculateIoU(const set<string>& tokens1, const set<string>& tokens2);
    string findBestResponse(const string& userInput);

    
};

#endif