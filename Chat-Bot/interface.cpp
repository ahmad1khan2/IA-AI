#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// --- Global arrays and counters ---
string userInput[50], botReply[50];
int utteranceCount = 0;  // renamed from count

string homeArea[50], homeSize[50], homeInstallments[50], homePrice[50], homeDownPayment[50];
int homeCount = 0;

// --- Function to handle both files ---
void fileHandling() {
    // --- Read Utterances.txt ---
    ifstream file("Utterances.txt");
    if (!file) {
        cout << "Error opening Utterances.txt!" << endl;
        return;
    }

    string line;
    utteranceCount = 0;
    while (getline(file, line)) {
        int i = 0;
        string left = "", right = "";
        while (i < line.length() && line[i] != '#') {
            left += line[i];
            i++;
        }
        i++; // move past #
        while (i < line.length()) {
            right += line[i];
            i++;
        }
        userInput[utteranceCount] = left;
        botReply[utteranceCount] = right;
        utteranceCount++;
    }
    file.close();

    // --- Read Home.txt ---
    ifstream homeFile("Home.txt");
    if (!homeFile) {
        cout << "Error opening Home.txt!" << endl;
        return;
    }

    string homeLine;
    if (!getline(homeFile, homeLine)) {
        cout << "Home.txt is empty or unreadable!" << endl;
        return;
    }

    homeCount = 0;
    while (getline(homeFile, homeLine)) {
        string parts[5];
        int partIndex = 0;
        string currentPart = "";
        int i = 0;
        while (i < homeLine.length()) {
            if (homeLine[i] == '#') {
                parts[partIndex] = currentPart;
                currentPart = "";
                partIndex++;
            }
            else {
                currentPart += homeLine[i];
            }
            i++;
        }
        parts[partIndex] = currentPart;

        if (partIndex == 4) {
            homeArea[homeCount] = parts[0];
            homeSize[homeCount] = parts[1];
            homeInstallments[homeCount] = parts[2];
            homePrice[homeCount] = parts[3];
            homeDownPayment[homeCount] = parts[4];
            homeCount++;
        }
    }
    homeFile.close();
}

// --- Function to handle the chat ---
void mainChat() {
    cout << "Chatbot started. Type something (X to exit)" << endl;

    string input;
    string chatState = "main";
    int homeLoanAttempt = 0;

    while (true) {
        cout << "You: ";
        getline(cin, input);

        string inputForMatching = input;
        if (inputForMatching.length() == 1) {
            inputForMatching[0] = toupper(inputForMatching[0]);
        }
        else {
            for (int k = 0; k < inputForMatching.length(); k++) {
                inputForMatching[k] = tolower(inputForMatching[k]);
            }
        }

        if (inputForMatching == "X") {
            cout << "Chatbot: Goodbye!" << endl;
            break;
        }

        if (chatState == "main") {
            bool found = false;
            for (int i = 0; i < utteranceCount; i++) {
                string tempUser = userInput[i];
                if (tempUser.length() == 1) {
                    tempUser[0] = toupper(tempUser[0]);
                }
                else {
                    for (int j = 0; j < tempUser.length(); j++) {
                        tempUser[j] = tolower(tempUser[j]);
                    }
                }

                if (inputForMatching == tempUser) {
                    cout << "Chatbot: " << botReply[i] << endl;
                    found = true;

                    if (inputForMatching == "H") {
                        chatState = "home_loan_area";
                        homeLoanAttempt = 0;
                    }
                    break;
                }
            }

            if (!found) {
                for (int j = 0; j < utteranceCount; j++) {
                    if (userInput[j] == "*") {
                        cout << "Chatbot: " << botReply[j] << endl;
                        found = true;
                        break;
                    }
                }
                if (!found)
                    cout << "Chatbot: Sorry, I didn't understand that." << endl;
            }
        }
        else if (chatState == "home_loan_area") {
            bool isValidOption = (inputForMatching == "1" || inputForMatching == "2" || inputForMatching == "3" || inputForMatching == "4");

            if (!isValidOption) {
                cout << "Chatbot: That is not a valid option. Please select from 1, 2, 3, or 4. Press X to exit." << endl;
            }
            else {
                string targetArea = "Area " + inputForMatching;
                bool foundOptions = false;

                for (int i = 0; i < homeCount; i++) {
                    if (homeArea[i] == targetArea) {
                        if (!foundOptions) {
                            cout << "Chatbot: Here are the options for " << targetArea << ":" << endl;
                        }
                        cout << "  - Size: " << homeSize[i]
                            << ", Installments: " << homeInstallments[i]
                                << ", Price: " << homePrice[i]
                                    << ", Down Payment: " << homeDownPayment[i] << endl;
                                    foundOptions = true;
                    }
                }

                if (foundOptions) {
                    cout << "\nChatbot: Please press A if you want to apply for another loan. Press X to exit." << endl;
                    chatState = "main";
                }
                else {
                    homeLoanAttempt++;
                    if (homeLoanAttempt == 1) {
                        string availableAreas = "";
                        bool has1 = false, has2 = false, has3 = false, has4 = false;
                        for (int i = 0; i < homeCount; i++) {
                            if (homeArea[i] == "Area 1") has1 = true;
                            if (homeArea[i] == "Area 2") has2 = true;
                            if (homeArea[i] == "Area 3") has3 = true;
                            if (homeArea[i] == "Area 4") has4 = true;
                        }

                        if (has1) availableAreas += "1, ";
                        if (has2) availableAreas += "2, ";
                        if (has3) availableAreas += "3, ";
                        if (has4) availableAreas += "4, ";

                        if (availableAreas.length() > 0) {
                            availableAreas = availableAreas.substr(0, availableAreas.length() - 2);
                            cout << "Chatbot: Sorry, " << targetArea << " is not available. "
                                << "Available options are : " << availableAreas << ". "
                                << "Please select from these options or press X to exit." << endl;
                        }
                        else {
                            cout << "Chatbot: Sorry, it looks like no home loan options are available at this time. "
                                << "Please press A if you want to apply for another loan. Press X to exit." << endl;
                            chatState = "main";
                        }
                    }
                    else {
                        cout << "Chatbot: Sorry, that option is still not available. "
                            << "Please press A if you want to apply for another loan. Press X to exit." << endl;
                        chatState = "main";
                    }
                }
            }
        }
    }
}

// --- Main ---
int main() {
    fileHandling();
    mainChat();
    return 0;
}
