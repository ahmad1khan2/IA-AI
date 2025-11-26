#include "GeneralChat.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <locale>

using namespace std;

GeneralChatHandler::GeneralChatHandler()
    : chatState("chatting")
{
    // Use Parser to load human corpus instead of local function
    Parser::loadHumanCorpus(human1Lines, human2Lines);
}


// Safe character checking function
bool isAlphaNumeric(char c) {
    unsigned char uc = static_cast<unsigned char>(c);
    return (uc >= 'a' && uc <= 'z') ||
        (uc >= 'A' && uc <= 'Z') ||
        (uc >= '0' && uc <= '9');
}

set<string> GeneralChatHandler::tokenize(const string& text) {
    set<string> tokens;
    stringstream ss(text);
    string token;

    while (ss >> token) {
        // Convert to lowercase and remove punctuation
        string cleanToken;
        for (char c : token) {
            if (isAlphaNumeric(c)) {
                cleanToken += tolower(c);
            }
        }
        if (!cleanToken.empty()) {
            tokens.insert(cleanToken);
        }
    }

    return tokens;
}

double GeneralChatHandler::calculateIoU(const set<string>& tokens1, const set<string>& tokens2) {
    if (tokens1.empty() && tokens2.empty()) return 1.0;
    if (tokens1.empty() || tokens2.empty()) return 0.0;

    set<string> intersection;
    set<string> unionSet;

    // Calculate intersection
    for (const auto& token : tokens1) {
        if (tokens2.find(token) != tokens2.end()) {
            intersection.insert(token);
        }
    }

    // Calculate union
    unionSet = tokens1;
    for (const auto& token : tokens2) {
        unionSet.insert(token);
    }

    if (unionSet.empty()) return 0.0;
    return static_cast<double>(intersection.size()) / unionSet.size();
}

string GeneralChatHandler::findBestResponse(const string& userInput) {
    if (human1Lines.empty() || human2Lines.empty()) {
        return "I'm having trouble accessing my chat data. Let's talk about loans instead!";
    }

    auto userTokens = tokenize(userInput);
    double bestIoU = 0.0;
    int bestIndex = -1;

    for (size_t i = 0; i < human1Lines.size(); i++) {
        auto corpusTokens = tokenize(human1Lines[i]);
        double iou = calculateIoU(userTokens, corpusTokens);

        if (iou > bestIoU) {
            bestIoU = iou;
            bestIndex = static_cast<int>(i);
        }
    }

    // Threshold for matching
    if (bestIoU > 0.1 && bestIndex >= 0) {
        // CRITICAL FIX: Check if bestIndex is within bounds of human2Lines
        if (bestIndex < static_cast<int>(human2Lines.size())) {
            return human2Lines[bestIndex];
        }
        else {
            // Index out of bounds - give generic response
            return "I don't have a good answer for that. Tell me more about what you're thinking!";
        }
    }
    else {
        return "That's interesting! Tell me more, or we can talk about loans if you prefer.";
    }
}

void GeneralChatHandler::handleInput(const string& input) {
    string normalized = input;
    transform(normalized.begin(), normalized.end(), normalized.begin(), ::tolower);

    if (normalized == "exit" || normalized == "menu") {
        cout << "Chatbot: Returning to main menu. You can ask about loans or type 'chat' to continue our conversation." << endl;
        chatState = "exiting";
        return;
    }

    string response = findBestResponse(input);
    cout << "Chatbot: " << response << endl;
}