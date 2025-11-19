#include "home.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>

using namespace std;

HomeLoanHandler::HomeLoanHandler()
    : chatState("main"), homeLoanAttempt(0), currentOptionsCount(0)
{
    homeLoans = Parser::readHome();
    cout << "Debug: Loaded " << homeLoans.size() << " home loans" << endl;
}

void HomeLoanHandler::runHome() {
    cout << "Home Loan Chatbot started. Type something (X to exit)" << endl;
    string input;

    while (true) {
        cout << "You: ";
        getline(cin, input);

        string inputForMatching = input;
        transform(inputForMatching.begin(), inputForMatching.end(), inputForMatching.begin(), ::tolower);

        if (inputForMatching == "x") {
            cout << "Chatbot: Goodbye!" << endl;
            break;
        }

        if (chatState == "main") handleMainState(inputForMatching);
        else if (chatState == "home_loan_area") handleAreaState(inputForMatching);
        else if (chatState == "home_loan_select") handleSelectState(inputForMatching);
    }
}

void HomeLoanHandler::handleMainState(const string& input) {
    if (input == "h") {
        if (homeLoans.empty()) {
            cout << "Chatbot: Sorry, no home loan options are currently available." << endl;
        }
        else {
            cout << "Chatbot: You are applying for a home loan. Please select area (1-4)" << endl;
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
        cout << "Chatbot: Invalid option. Please select 1-4 or X to exit." << endl;
        return;
    }

    string targetArea = "Area " + input;
    displayHomeLoanOptions(targetArea);
}

void HomeLoanHandler::displayHomeLoanOptions(const string& targetArea) {
    currentOptionsCount = 0;
    currentOptionsLoanIndex.clear();
    bool foundOptions = false;

    int targetAreaNum = stoi(targetArea.substr(5));

    for (int i = 0; i < homeLoans.size(); ++i) {
        const auto& loan = homeLoans[i];
        if (loan.getArea() == targetAreaNum) {
            if (!foundOptions) {
                cout << "Chatbot: Options for " << targetArea << ":" << endl;
                foundOptions = true;
            }

            currentOptionsLoanIndex.push_back(i);

            cout << "  " << (currentOptionsCount + 1) << ". Size: " << loan.getSize()
                << ", Price: " << loan.getPrice()
                << ", Down Payment: " << loan.getDownPayment()
                << " (for " << loan.getInstallments() << " months)" << endl;

            currentOptionsCount++;
        }
    }

    if (foundOptions) {
        cout << "\nChatbot: Please select an option number to view the detailed plan." << endl;
        cout << "Chatbot: Press B to go back to main menu." << endl;
        chatState = "home_loan_select";
    }
    else {
        homeLoanAttempt++;
        if (homeLoanAttempt == 1) {
            string available = getAvailableAreas();
            if (!available.empty()) {
                cout << "Chatbot: Area not available. Available: " << available << endl;
            }
            else {
                cout << "Chatbot: No home loans available currently." << endl;
                chatState = "main";
            }
        }
        else {
            cout << "Chatbot: Area still unavailable. Returning to main menu." << endl;
            chatState = "main";
        }
    }
}

void HomeLoanHandler::handleSelectState(const string& input) {
    if (input == "b") {
        cout << "Chatbot: Returning to main menu. Press H for home loan or X to exit." << endl;
        chatState = "main";
        return;
    }

    int selectedIndex = -1;
    try { selectedIndex = stoi(input) - 1; }
    catch (...) {}

    if (selectedIndex >= 0 && selectedIndex < currentOptionsCount) {
        displayInstallmentPlan(selectedIndex);

        // Ask if user wants to apply
        cout << "\nChatbot: Would you like to apply for this loan? (yes/no): ";
        string applyResponse;
        getline(cin, applyResponse);
        transform(applyResponse.begin(), applyResponse.end(), applyResponse.begin(), ::tolower);

        if (applyResponse == "yes" || applyResponse == "y") {
            handleApplyState(selectedIndex);
        }
        else {
            cout << "Chatbot: Press H for another home loan or X to exit." << endl;
            chatState = "main";
        }
    }
    else {
        cout << "Chatbot: Invalid selection. Type 1-" << currentOptionsCount
            << ", B to go back, or X to exit." << endl;
    }
}

void HomeLoanHandler::handleApplyState(int selectedIndex) {
    cout << "\n=== STARTING HOME LOAN APPLICATION ===" << endl;
    Applicant applicant;
    applicant.collectData();

    if (!applicant.applicationId.empty()) {
        cout << "Chatbot: Application process completed. Press H for another home loan or X to exit." << endl;
    }
    else {
        cout << "Press H to try again or X to exit." << endl;
    }
    // Ask for starting month
    int startMonth = 1;
    cout << "Chatbot: Enter starting month for installment plan (1-12):";
    string monthInput;
    getline(cin, monthInput);
    try {
        int m = stoi(monthInput);
        if (m > 0) startMonth = m;
    }
    catch (...) { startMonth = 1; }

    displayInstallmentPlanWithMonths(selectedIndex, startMonth);

    cout << "\nChatbot: Press H for another home loan or X to exit." << endl;
    chatState = "main";
}

void HomeLoanHandler::displayInstallmentPlan(int selectedIndex) {
    int realIndex = currentOptionsLoanIndex[selectedIndex];
    homeLoans[realIndex].printInstallmentPlan();
}

void HomeLoanHandler::displayInstallmentPlanWithMonths(int selectedIndex, int month) {
    int realIndex = currentOptionsLoanIndex[selectedIndex];
    homeLoans[realIndex].printInstallmentPlanStartingAt(month);
}

int HomeLoanHandler::parseFormattedNumber(const string& str) {
    string clean;
    for (char c : str) if (isdigit(static_cast<unsigned char>(c))) clean += c;
    try { return stoi(clean); }
    catch (...) { return 0; }
}

bool HomeLoanHandler::isValidAreaOption(const string& input) {
    return (input == "1" || input == "2" || input == "3" || input == "4");
}

string HomeLoanHandler::getAvailableAreas() {
    string available;
    bool has[5] = { false,false,false,false,false }; // 1-4

    for (auto& loan : homeLoans) has[loan.getArea()] = true;

    for (int i = 1; i <= 4; ++i) if (has[i]) available += to_string(i) + ", ";
    if (!available.empty()) available = available.substr(0, available.length() - 2);

    return available;
}
