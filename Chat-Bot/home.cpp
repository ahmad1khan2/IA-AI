#include "home.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>

using namespace std;

HomeLoanHandler::HomeLoanHandler()
    : chatState("main"), homeLoanAttempt(0), currentOptionsCount(0) {
    homeLoans = Parser::readHome();
    cout << "Debug: Loaded " << homeLoans.size() << " home loans" << endl; // Debug line
}

void HomeLoanHandler::runHome() {
    cout << "Home Loan Chatbot started. Type something (X to exit)" << endl;
    string input;

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
            handleMainState(inputForMatching);
        }
        else if (chatState == "home_loan_area") {
            handleAreaState(inputForMatching);
        }
        else if (chatState == "home_loan_select") {
            handleSelectState(inputForMatching);
        }
    }
}

void HomeLoanHandler::handleMainState(const string& input) {
    if (input == "H") {
        if (homeLoans.empty()) {
            cout << "Chatbot: Sorry, no home loan options are currently available. Please try another loan type or check back later." << endl;
        }
        else {
            cout << "Chatbot: You are applying for a home loan. Please select area. Options are 1, 2, 3, 4" << endl;
            chatState = "home_loan_area";
            homeLoanAttempt = 0;
        }
    }
    else {
        cout << "Chatbot: Please press H for home loan or X to exit." << endl;
    }
}

void HomeLoanHandler::handleAreaState(const string& input) {
    if (!isValidAreaOption(input)) {
        cout << "Chatbot: That is not a valid option. Please select from 1, 2, 3, or 4. Press X to exit." << endl;
        return;
    }

    string targetArea = "Area " + input;
    displayHomeLoanOptions(targetArea);
}

void HomeLoanHandler::handleSelectState(const string& input) {
    if (input == "B") {
        cout << "Chatbot: Returning to main menu. Please press H for home loan. Press X to exit." << endl;
        chatState = "main";
    }
    else if (input == "A") {
        // Apply for loan - start application process
        handleApplyState();
    }
    else {
        int selectedIndex = -1;
        try {
            selectedIndex = stoi(input) - 1;
        }
        catch (...) {}

        if (selectedIndex >= 0 && selectedIndex < currentOptionsCount) {
            displayInstallmentPlan(selectedIndex);

            // Ask if they want to apply
            cout << "\nChatbot: Would you like to apply for this loan? (yes/no): ";
            string applyResponse;
            getline(cin, applyResponse);

            for (char& c : applyResponse) {
                c = tolower(c);
            }

            if (applyResponse == "yes" || applyResponse == "y") {
                handleApplyState();
            }
            else {
                cout << "Chatbot: Press H for another home loan or X to exit." << endl;
                chatState = "main";
            }
        }
        else {
            cout << "Chatbot: That is not a valid selection. Please type a number from 1 to "
                << currentOptionsCount << ", B to go back, or X to exit." << endl;
        }
    }
}

void HomeLoanHandler::handleApplyState() {
    cout << "\n=== STARTING LOAN APPLICATION PROCESS ===" << endl;

    Applicant applicant;
    applicant.collectData();

    cout << "Chatbot: Application process completed. Press H for another home loan or X to exit." << endl;
    chatState = "main";
}

void HomeLoanHandler::displayHomeLoanOptions(const string& targetArea) {
    bool foundOptions = false;
    currentOptionsCount = 0;
    currentOptionsSize.clear();
    currentOptionsPrice.clear();
    currentOptionsDownPayment.clear();
    currentOptionsMonths.clear();

    // Convert target area to integer for comparison
    int targetAreaNum = stoi(targetArea.substr(5)); // Extract number from "Area X"

    for (const auto& loan : homeLoans) {
        if (loan.getArea() == targetAreaNum) {
            if (!foundOptions) {
                cout << "Chatbot: Here are the options for " << targetArea << ":" << endl;
                foundOptions = true;
            }

            currentOptionsSize.push_back(loan.getSize());
            currentOptionsPrice.push_back(to_string(loan.getPrice()));
            currentOptionsDownPayment.push_back(to_string(loan.getDownPayment()));
            currentOptionsMonths.push_back(to_string(loan.getInstallments()));

            cout << "  " << (currentOptionsCount + 1) << ". Size: " << loan.getSize()
                << ", Price: " << loan.getPrice()
                << ", Down Payment: " << loan.getDownPayment()
                << " (for " << loan.getInstallments() << " months)" << endl;

            currentOptionsCount++;
        }
    }

    if (foundOptions) {
        cout << "\nChatbot: Please select an option (e.g., 1, 2) to see the detailed plan." << endl;
        cout << "Chatbot: Press B to go back to the main menu." << endl;
        chatState = "home_loan_select";
    }
    else {
        homeLoanAttempt++;
        if (homeLoanAttempt == 1) {
            string availableAreas = getAvailableAreas();
            if (!availableAreas.empty()) {
                cout << "Chatbot: Sorry, " << targetArea << " is not available. "
                    << "Available options are: " << availableAreas << ". "
                    << "Please select from these options or press X to exit." << endl;
            }
            else {
                cout << "Chatbot: Sorry, it looks like no home loan options are available at this time. "
                    << "Please press H if you want to apply for home loan. Press X to exit." << endl;
                chatState = "main";
            }
        }
        else {
            cout << "Chatbot: Sorry, that option is still not available. "
                << "Please press H if you want to apply for home loan. Press X to exit." << endl;
            chatState = "main";
        }
    }
}

void HomeLoanHandler::displayInstallmentPlan(int selectedIndex) {
    // Find the matching homeLoan object using the area and stored vectors
     // We need to reconstruct which area we were searching for
    string targetArea = "Area "; // We'll need to track this from the previous state

    // Since we don't have the area stored, we'll search through all homeLoans
    // to find one that matches all the criteria from our stored vectors
    for (auto& loan : homeLoans) {
        string size = currentOptionsSize[selectedIndex];
        int price = stoi(currentOptionsPrice[selectedIndex]);
        int downPayment = stoi(currentOptionsDownPayment[selectedIndex]);
        int months = stoi(currentOptionsMonths[selectedIndex]);

        if (loan.getSize() == size &&
            loan.getPrice() == price &&
            loan.getDownPayment() == downPayment &&
            loan.getInstallments() == months) {

            // Use the existing method from Loan class to print installment plan
            cout << "\n=== DETAILED INSTALLMENT PLAN ===" << endl;
            loan.printInstallmentPlan();
            return;
        }
    }
}

int HomeLoanHandler::parseFormattedNumber(const string& str) {
    string cleanStr = "";
    for (char c : str) {
        if (c >= '0' && c <= '9') {
            cleanStr += c;
        }
    }
    try {
        return stoi(cleanStr);
    }
    catch (...) {
        return 0;
    }
}

bool HomeLoanHandler::isValidAreaOption(const string& input) {
    return (input == "1" || input == "2" || input == "3" || input == "4");
}

string HomeLoanHandler::getAvailableAreas() {
    string availableAreas = "";
    bool has1 = false, has2 = false, has3 = false, has4 = false;

    for (const auto& loan : homeLoans) {
        if (loan.getArea() == 1) has1 = true;
        if (loan.getArea() == 2) has2 = true;
        if (loan.getArea() == 3) has3 = true;
        if (loan.getArea() == 4) has4 = true;
    }

    if (has1) availableAreas += "1, ";
    if (has2) availableAreas += "2, ";
    if (has3) availableAreas += "3, ";
    if (has4) availableAreas += "4, ";

    if (availableAreas.length() > 0) {
        availableAreas = availableAreas.substr(0, availableAreas.length() - 2);
    }

    return availableAreas;
}