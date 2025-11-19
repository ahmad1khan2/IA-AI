#include "home.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>

using namespace std;

HomeLoanHandler::HomeLoanHandler()
    : chatState("main"), homeLoanAttempt(0), currentOptionsCount(0) {
    homeLoans = Parser::readHome();
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
        cout << "Chatbot: You are applying for a home loan. Please select area. Options are 1, 2, 3, 4" << endl;
        chatState = "home_loan_area";
        homeLoanAttempt = 0;
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
                cout << "Chatbot: Returning to main menu. Press H for home loan. Press X to exit." << endl;
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

    cout << "Chatbot: Application process completed. Returning to main menu." << endl;
    chatState = "main";
}

void HomeLoanHandler::displayHomeLoanOptions(const string& targetArea) {
    bool foundOptions = false;
    currentOptionsCount = 0;
    currentOptionsSize.clear();
    currentOptionsPrice.clear();
    currentOptionsDownPayment.clear();
    currentOptionsMonths.clear();

    for (const auto& loan : homeLoans) {
        string areaStr = "Area " + to_string(loan.getArea());
        if (areaStr == targetArea) {
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
    // Get loan details and parse formatted numbers
    string size = currentOptionsSize[selectedIndex];
    int price = parseFormattedNumber(currentOptionsPrice[selectedIndex]);
    int downPayment = parseFormattedNumber(currentOptionsDownPayment[selectedIndex]);
    int months = stoi(currentOptionsMonths[selectedIndex]);

    // Calculate monthly installment
    double monthlyInstallment = (price - downPayment) / static_cast<double>(months);

    // Print complete tabular loan installment plan
    cout << "\n" << string(70, '=') << endl;
    cout << "COMPREHENSIVE LOAN INSTALLMENT PLAN" << endl;
    cout << string(70, '=') << endl;
    cout << "Property Size: " << size << endl;
    cout << "Total Price: $" << price << endl;
    cout << "Down Payment: $" << downPayment << endl;
    cout << "Loan Amount: $" << (price - downPayment) << endl;
    cout << "Loan Term: " << months << " months" << endl;
    cout << "Monthly Installment: $" << fixed << setprecision(2) << monthlyInstallment << endl;
    cout << string(70, '-') << endl;

    // Table header
    cout << setw(8) << "Month"
        << setw(15) << "Installment"
        << setw(20) << "Principal Paid"
        << setw(20) << "Remaining Balance" << endl;
    cout << string(70, '-') << endl;

    // Calculate and display each month's details
    double remainingBalance = price - downPayment;
    double totalPaid = downPayment;

    for (int month = 1; month <= months; month++) {
        double principalThisMonth = monthlyInstallment;
        remainingBalance -= principalThisMonth;
        totalPaid += principalThisMonth;

        // Handle rounding in the last month
        if (month == months) {
            principalThisMonth = remainingBalance + principalThisMonth;
            remainingBalance = 0;
        }

        cout << setw(8) << month
            << setw(15) << fixed << setprecision(2) << monthlyInstallment
            << setw(20) << fixed << setprecision(2) << principalThisMonth
            << setw(20) << fixed << setprecision(2) << (remainingBalance > 0 ? remainingBalance : 0)
            << endl;
    }

    cout << string(70, '=') << endl;
    cout << "Total Amount Paid: $" << fixed << setprecision(2) << totalPaid << endl;
    cout << "Total Interest: $" << fixed << setprecision(2) << (totalPaid - price) << endl;
    cout << string(70, '=') << endl;
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