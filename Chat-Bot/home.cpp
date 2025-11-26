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
    
}

void HomeLoanHandler::handleInput(const string& input) {
    string inputForMatching = input;
    transform(inputForMatching.begin(), inputForMatching.end(), inputForMatching.begin(), ::tolower);

    if (chatState == "main") handleMainState(inputForMatching);
    else if (chatState == "home_loan_area") handleAreaState(inputForMatching);
    else if (chatState == "home_loan_select") handleSelectState(inputForMatching);
}

void HomeLoanHandler::handleMainState(const string& input) {
    if (input == "h" || input == "show") {
        if (homeLoans.empty()) {
            cout << "Chatbot: Sorry, no home loan options are currently available." << endl;
        }
        else {
            cout << "Chatbot: Please select area (1-4)" << endl;
            chatState = "home_loan_area";
            homeLoanAttempt = 0;
        }
    }
    else {
        cout << "Chatbot: Type 'show' to see home loan options or 'menu' to return to main menu." << endl;
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
        
        chatState = "home_loan_select";
    }
    else {
        homeLoanAttempt++;
        if (homeLoanAttempt == 1) {
            string available = getAvailableAreas();
            if (!available.empty()) {
                cout << "Chatbot: I am so sorry. This Area is not available. Available areas: " << available << endl;
            }
            else {
                cout << "Chatbot: No home loans available currently." << endl;
                chatState = "main";
            }
        }
        else {
            cout << "Chatbot:This area is still unavailable. Returning to main menu." << endl;
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
            cout << "Chatbot: Type H if you would like to continue exploring home loan options. Type A to explore other loan options or X to exit." << endl;
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

    // Check if application was actually completed or just exited
    if (applicant.getStatus() == "Submitted") {
        cout << "Chatbot: Application process completed. Press H for another home loan or X to exit." << endl;

        // Only ask for installment plan if application was completed
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
    }
    else if (applicant.applicationId.empty()) {
        // Application not found - no message needed (already shown in resume)
    }
    else {
        // Application was exited, not completed
        cout << "Chatbot: Application saved." << endl;
    }

    cout << "\nChatbot: Press H for another home loan or C for car loan or S for scooter loan or Q to check Application with CNIC or X to exit." << endl;
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


