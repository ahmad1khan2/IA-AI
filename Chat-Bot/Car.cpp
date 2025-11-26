#include "Car.h"
#include "applicant.h"
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
    else if (input == "menu") {
        cout << "Chatbot: Returning to main menu..." << endl;
        chatState = "main";
    }
    else {
        // Check if input is numeric (like "1234") and reject it
        bool isNumeric = !input.empty();
        for (char c : input) {
            if (!isdigit(c)) {
                isNumeric = false;
                break;
            }
        }

        if (isNumeric) {
            cout << "Chatbot: Invalid input. Please type 'show' to see car options or 'menu' to return to main menu." << endl;
        }
        else {
            cout << "Chatbot: Invalid command. Type 'show' to see car options or 'menu' to return to main menu." << endl;
        }
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
    if (input == "b" || input == "menu") {
        cout << "Chatbot: Returning to main menu. Press C for car loan or X to exit." << endl;
        chatState = "main";
        return;
    }

    // Check if input is numeric before trying to convert
    bool isNumeric = !input.empty();
    for (char c : input) {
        if (!isdigit(c)) {
            isNumeric = false;
            break;
        }
    }

    if (!isNumeric) {
        cout << "Chatbot: Invalid command. Please type a number from 1 to "
            << currentOptionsCount << ", or B to go back." << endl;
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
            << currentOptionsCount << ", or B to go back." << endl;
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
    try {
        applicant.collectData();
    }
    catch (const ReturnToMainMenuException& e) {
        // Re-throw to be caught by StateManager
        throw;
    }

    // Check if application was actually completed or just exited
    if (applicant.getStatus() == "Submitted") {
        cout << "Chatbot: Application process completed. Press C for another Car loan or X to exit." << endl;

        // Only ask for installment plan if application was completed
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
    }
    else if (applicant.applicationId.empty()) {
        // Application not found - no message needed (already shown in resume)
    }
    else {
        // Application was exited, not completed
        cout << "Chatbot: Application saved." << endl;
    }

    cout << "\nChatbot: Press C for another car loan or H for home loan or S for scooter loan or Q to check Application with CNIC or X to exit." << endl;
    chatState = "main";
}

string CarLoanHandler::boolToYesNo(bool v) const {
    return v ? "Yes" : "No";
}