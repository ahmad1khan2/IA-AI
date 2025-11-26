#pragma once
#ifndef GENERALCHAT_H
#define GENERALCHAT_H

#include <string>
#include <vector>
#include <set>
#include <algorithm>

class GeneralChatHandler {
public:
    GeneralChatHandler();

    void handleInput(const std::string& input);
    std::string getCurrentState() const { return chatState; }

private:
    std::string chatState;

    // Corpus data
    std::vector<std::string> human1Lines;
    std::vector<std::string> human2Lines;

    // Tokenization and matching
    std::set<std::string> tokenize(const std::string& text);
    double calculateIoU(const std::set<std::string>& tokens1, const std::set<std::string>& tokens2);
    std::string findBestResponse(const std::string& userInput);

    void loadHumanCorpus();
};

#endif