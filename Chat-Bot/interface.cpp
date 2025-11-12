#include <iostream>
#include <fstream>
#include <string> 
using namespace std;

int main() {
    ifstream file("Utterances.txt");
    string userInput[50], botReply[50];
    int count = 0;
    string line;

    // --- Arrays for Home.txt data ---
    string homeArea[50], homeSize[50], homeInstallments[50], homePrice[50], homeDownPayment[50];
    int homeCount = 0;
    // --- End arrays ---

    if (!file) {
        cout << "Error opening Utterances.txt!" << endl;
        return 0;
    }

    // Read Utterances.txt line by line
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
        userInput[count] = left;
        botReply[count] = right;
        count++;
    }
    file.close();

    // --- Code to read Home.txt ---
    ifstream homeFile("Home.txt");
    if (!homeFile) {
        cout << "Error opening Home.txt!" << endl;
        return 0;
    }

    string homeLine;
    // Read and discard the header line
    if (!getline(homeFile, homeLine)) {
        cout << "Home.txt is empty or unreadable!" << endl;
        return 0;
    }

    // Read the data lines from Home.txt
    while (getline(homeFile, homeLine)) {
        string parts[5]; // To store the 5 columns
        int partIndex = 0;
        string currentPart = "";
        int i = 0;

        // Manual parsing, matching the original code's style
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
        parts[partIndex] = currentPart; // Add the last part

        // Assign parts to arrays if all 5 were found
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
    // --- End code for Home.txt ---


    cout << "Chatbot started. Type something (X to exit)" << endl;

    string input;
    string chatState = "main"; // --- state variable ---

    int homeLoanAttempt = 0; // Tracks failed attempts for a valid area


    while (true) {
        cout << "You: ";
        getline(cin, input);

        // Convert input to consistent format for matching
        string inputForMatching = input;
        if (inputForMatching.length() == 1) {
            inputForMatching[0] = toupper(inputForMatching[0]); // single letter choice to uppercase
        }
        else {
            for (int k = 0; k < inputForMatching.length(); k++) {
                inputForMatching[k] = tolower(inputForMatching[k]); // words to lowercase
            }
        }

        // --- Exit Check: At top to work in any state ---
        if (inputForMatching == "X") {
            cout << "Chatbot: Goodbye!" << endl;
            break;
        }

        // --- State-based logic ---
        if (chatState == "main") {
            bool found = false;
            for (int i = 0; i < count; i++) {
                // Convert file data to same format for consistent matching
                string tempUser = userInput[i];
                if (tempUser.length() == 1) {
                    tempUser[0] = toupper(tempUser[0]); // single letter to uppercase
                }
                else {
                    for (int j = 0; j < tempUser.length(); j++) {
                        tempUser[j] = tolower(tempUser[j]); // words to lowercase
                    }
                }

                if (inputForMatching == tempUser) {
                    cout << "Chatbot: " << botReply[i] << endl;
                    found = true;

                    // --- State Change Logic ---
                    if (inputForMatching == "H") {
                        chatState = "home_loan_area";
                        homeLoanAttempt = 0; // Reset counter
                    }
                    

                    break;
                }
            }

            if (!found) {
                // find the line that starts with '*'
                for (int j = 0; j < count; j++) {
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
            // --- logic for handling home loan area selection ---
           // First, check if the input is a valid *option* from the prompt
            bool isValidOption = (inputForMatching == "1" || inputForMatching == "2" || inputForMatching == "3" || inputForMatching == "4");

            if (!isValidOption) {
                // If not a valid option, ask them to try again
                // We DO NOT change the state
                cout << "Chatbot: That is not a valid option. Please select from 1, 2, 3, or 4. Press X to exit." << endl;
            }
            else {
                // It IS a valid option, so now we check the file
                string targetArea = "Area " + inputForMatching;
                bool foundOptions = false;

                // Loop through the loaded home loan data
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

                // ---  FAILURE LOGIC ---
                if (foundOptions) {
                    // SUCCESS: Options were found and printed
                    cout << "\nChatbot: Please press A if you want to apply for another loan. Press X to exit." << endl;
                    chatState = "main"; // Reset state
                }
                else {
                    // FAILURE: Valid option, but no data
                    homeLoanAttempt++; // Increment the attempt counter

                    if (homeLoanAttempt == 1) {
                        // FIRST FAILURE: Find and list available options

                        // Find which options ARE available
                        string availableAreas = "";
                        bool has1 = false, has2 = false, has3 = false, has4 = false;
                        for (int i = 0; i < homeCount; i++) {
                            if (homeArea[i] == "Area 1") has1 = true;
                            if (homeArea[i] == "Area 2") has2 = true;
                            if (homeArea[i] == "Area 3") has3 = true;
                            if (homeArea[i] == "Area 4") has4 = true;
                        }

                        // Build the reply string
                        if (has1) availableAreas += "1, ";
                        if (has2) availableAreas += "2, ";
                        if (has3) availableAreas += "3, ";
                        if (has4) availableAreas += "4, ";

                        if (availableAreas.length() > 0) {
                            // Remove the last ", "
                            availableAreas = availableAreas.substr(0, availableAreas.length() - 2);
                            cout << "Chatbot: Sorry, " << targetArea << " is not available. "
                                << "Available options are : " << availableAreas << ". "
                                << "Please select from these options or press X to exit." << endl;
                        }
                        else {
                            // Edge case: No options are available at all
                            cout << "Chatbot: Sorry, it looks like no home loan options are available at this time. "
                                << "Please press A if you want to apply for another loan. Press X to exit." << endl;
                            chatState = "main"; // Go back to main
                        }

                        // DO NOT change state. Let them try again.
                    }
                    else {
                        // SECOND FAILURE: Give up and go back to main
                        cout << "Chatbot: Sorry, that option is still not available. "
                            << "Please press A if you want to apply for another loan. Press X to exit." << endl;
                        chatState = "main"; // Reset state
                    }

                }

            }
        }
    }

    return 0;
}