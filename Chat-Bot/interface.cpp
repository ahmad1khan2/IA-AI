#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "fileHandling.h"
#include "home.h"
#include "Car.h"
#include "scooter.h"
#include "cnic.h"

using namespace std;

// Global arrays for utterances
vector<string> userInput;
vector<string> botReply;

// Function to read utterances
void readUtterances() {
    Parser::readUtterances(userInput, botReply);
}

// Function to handle the main chat with utterances
void mainChat() {
    cout << "Chatbot started. Type something (X to exit)" << endl;
    string input;
    string chatState = "main";

    while (true) {
        cout << "You: ";
        getline(cin, input);

        string inputForMatching = input;
        if (inputForMatching.length() == 1) {
            inputForMatching[0] = toupper(inputForMatching[0]);
        }
        else {
            for (size_t k = 0; k < inputForMatching.length(); k++) {
                inputForMatching[k] = tolower(inputForMatching[k]);
            }
        }

        if (inputForMatching == "X") {
            cout << "Chatbot: Goodbye!" << endl;
            break;
        }

        if (chatState == "main") {
            bool found = false;

            // Check for loan-specific commands first
            if (inputForMatching == "H") {
                cout << "Chatbot: Starting Home Loan System..." << endl;
                HomeLoanHandler homeHandler;
                homeHandler.runHome();
                cout << "Chatbot: Welcome back to main chat!" << endl;
                found = true;
            }
            else if (inputForMatching == "C") {
                cout << "Chatbot: Starting Car Loan System..." << endl;
                CarLoanHandler carHandler;
                carHandler.runCar();
                cout << "Chatbot: Welcome back to main chat!" << endl;
                found = true;
            }
            else if (inputForMatching == "S") {
                cout << "Chatbot: Starting Scooter Loan System..." << endl;
                ScooterLoanHandler scooterHandler;
                scooterHandler.runScooter();
                cout << "Chatbot: Welcome back to main chat!" << endl;
                found = true;
            }
            else if (inputForMatching == "Q") {
                cout << "Chatbot: Starting CNIC Query System..." << endl;
                queryApplicationsByCNIC();
                cout << "Chatbot: Welcome back to main chat!" << endl;
                found = true;
            }

            // If not a loan command, check utterances
            if (!found) {
                for (size_t i = 0; i < userInput.size(); i++) {
                    string tempUser = userInput[i];
                    if (tempUser.length() == 1) {
                        tempUser[0] = toupper(tempUser[0]);
                    }
                    else {
                        for (size_t j = 0; j < tempUser.length(); j++) {
                            tempUser[j] = tolower(tempUser[j]);
                        }
                    }

                    if (inputForMatching == tempUser) {
                        cout << "Chatbot: " << botReply[i] << endl;
                        found = true;

                        // Handle state transitions for loan types
                        if (inputForMatching == "H") {
                            cout << "Chatbot: Starting Home Loan System..." << endl;
                            HomeLoanHandler homeHandler;
                            homeHandler.runHome();
                            cout << "Chatbot: Welcome back to main chat!" << endl;
                        }
                        else if (inputForMatching == "C") {
                            cout << "Chatbot: Starting Car Loan System..." << endl;
                            CarLoanHandler carHandler;
                            carHandler.runCar();
                            cout << "Chatbot: Welcome back to main chat!" << endl;
                        }
                        else if (inputForMatching == "S") {
                            cout << "Chatbot: Starting Scooter Loan System..." << endl;
                            ScooterLoanHandler scooterHandler;
                            scooterHandler.runScooter();
                            cout << "Chatbot: Welcome back to main chat!" << endl;
                        }
                        break;
                    }
                }
            }

            // If still not found, check for wildcard
            if (!found) {
                for (size_t j = 0; j < userInput.size(); j++) {
                    if (userInput[j] == "*") {
                        cout << "Chatbot: " << botReply[j] << endl;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    cout << "Chatbot: Sorry, I didn't understand that. Please try H for home loan, C for car loan, S for scooter loan, Q for CNIC query, or X to exit." << endl;
                }
            }
        }
    }
}

// Main function
int main() {
    // Read utterances first
    readUtterances();

    // Start the main chat
    mainChat();

    return 0;
}