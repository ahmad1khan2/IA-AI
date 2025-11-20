#include "Car.h"
#include "fileHandling.h"   // Parser::readCar / stoiSafe etc.
#include "CarState.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

CarLoanHandler::CarLoanHandler()
    : chatState("main"), carLoanAttempt(0), currentOptionsCount(0)
{
    // Load car loans using parser
    carLoans = Parser::readCar();

}

void CarLoanHandler::handleInput(const string& input) {
    string inputForMatching = input;
    transform(inputForMatching.begin(), inputForMatching.end(),
        inputForMatching.begin(), ::tolower);

    if (chatState == "main") {
        handleMainState(inputForMatching);
    }
    else if (chatState == "car_loan_select") {
        handleSelectState(inputForMatching);
    }
}

void CarLoanHandler::handleMainState(const string& input) {
    if (input == "c" || input == "show") {
        if (carLoans.empty()) {
            cout << "Chatbot: Sorry, no car loan options are currently available." << endl;
        }
        else {
            cout << "Chatbot: Available car loan options:" << endl;
            displayCarOptions();
            chatState = "car_loan_select";
            carLoanAttempt = 0;
        }
    }
    else {
        cout << "Chatbot: Type 'show' to see car options or 'menu' to return to main menu." << endl;
    }
}

void CarLoanHandler::displayCarOptions() {
    currentOptionsCount = 0;
    currentOptionsLoanIndex.clear();

    for (int i = 0; i < carLoans.size(); i++) {
        const auto& loan = carLoans[i];
        currentOptionsLoanIndex.push_back(i);

        cout << "  " << (currentOptionsCount + 1) << ". "
            << loan.getMake() << " " << loan.getModel()
            << " (" << loan.getYear() << (loan.isUsed() ? ", used" : ", new") << ")"
            << ", Price: " << loan.getPrice()
            << ", Down Payment: " << loan.getDownPayment()
            << " (for " << loan.getInstallments() << " months)" << endl;

        currentOptionsCount++;
    }

    if (currentOptionsCount > 0) {
        cout << "\nChatbot: Please select an option number to view the detailed installment plan." << endl;
    }
    else {
        cout << "Chatbot: No car loan options found." << endl;
        chatState = "main";
    }
}

void CarLoanHandler::handleSelectState(const string& input) {
    if (input == "b") {
        cout << "Chatbot: Returning to main menu. Press C for car loan or X to exit." << endl;
        chatState = "main";
        return;
    }

    int selectedIndex = -1;
    try {
        selectedIndex = stoi(input) - 1;
    }
    catch (...) {
        selectedIndex = -1;
    }

    if (selectedIndex >= 0 && selectedIndex < currentOptionsCount) {
        // Display the regular installment plan first
        displayInstallmentPlan(selectedIndex);

        // Ask if user wants to apply
        cout << "\nChatbot: Would you like to apply for this loan? (yes/no): ";
        string applyResponse;
        getline(cin, applyResponse);
        transform(applyResponse.begin(), applyResponse.end(), applyResponse.begin(), ::tolower);

        if (applyResponse == "yes" || applyResponse == "y") {
            // Only call apply flow if user said yes
            handleApplyState(selectedIndex);
        }
        else {
            // If user says no, do NOT ask for starting month
            cout << "Chatbot: Press C for another car loan or X to exit." << endl;
            chatState = "main";
        }
    }
    else {
        cout << "Chatbot: Invalid selection. Please type a number from 1 to "
            << currentOptionsCount << ", B to go back, or X to exit." << endl;
    }
}

void CarLoanHandler::displayInstallmentPlan(int selectedIndex) {
    int realIndex = currentOptionsLoanIndex[selectedIndex];
    const auto& loan = carLoans[realIndex];

    cout << "\n=== DETAILED INSTALLMENT PLAN ===" << endl;
    loan.printInstallmentPlan();
}

void CarLoanHandler::displayInstallmentPlanWithMonths(int selectedIndex, int month) {
    int realIndex = currentOptionsLoanIndex[selectedIndex];
    const auto& loan = carLoans[realIndex];

    cout << "\n=== DETAILED INSTALLMENT PLAN (STARTING AT MONTH "
        << month << ") ===" << endl;

    loan.printInstallmentPlanStartingAt(month);
}

void CarLoanHandler::handleApplyState(int selectedIndex) {
    cout << "\n=== STARTING CAR LOAN APPLICATION PROCESS ===" << endl;

    Applicant applicant;
    applicant.collectData();

    if (!applicant.applicationId.empty()) {
        cout << "Chatbot: Application process completed. Press C for another Car loan or X to exit." << endl;
    }
    else {
        cout << "Press C to try again or X to exit." << endl;
    }

    // Ask for starting month to view installment plan
    int startMonth = 1;
    cout << "Chatbot: Enter starting month for installment plan (1-12): ";
    string monthInput;
    getline(cin, monthInput);

    try {
        int m = stoi(monthInput);
        if (m > 0) startMonth = m;
    }
    catch (...) {
        startMonth = 1; // fallback
    }

    // Show installment plan from chosen month
    displayInstallmentPlanWithMonths(selectedIndex, startMonth);

    cout << "\nChatbot: Press C for another car loan or H for home loan or S for scooter loan or X to exit." << endl;
    chatState = "main";
}

string CarLoanHandler::boolToYesNo(bool v) const {
    return v ? "Yes" : "No";
}