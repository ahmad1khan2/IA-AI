#include "scooter.h"
#include "applicant.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

ScooterLoanHandler::ScooterLoanHandler()
    : chatState("main"), scooterLoanAttempt(0), currentOptionsCount(0) {
    scooterLoans = Parser::readScooter();
}

void ScooterLoanHandler::handleInput(const string& input) {
    string inputForMatching = input;
    transform(inputForMatching.begin(), inputForMatching.end(), inputForMatching.begin(), ::tolower);

    if (chatState == "main") handleMainState(inputForMatching);
    else if (chatState == "scooter_loan_make") handleMakeState(inputForMatching);
    else if (chatState == "scooter_loan_model") handleModelState(inputForMatching);
}

void ScooterLoanHandler::handleMainState(const string& input) {
    if (input == "s" || input == "show") {
        if (scooterLoans.empty()) {
            cout << "Chatbot: Sorry, no scooter loan options are currently available." << endl;
        }
        else {
            cout << "Chatbot: I am happy to help you with the scooter options." << endl;
            displayScooterMakes();
            chatState = "scooter_loan_make";
            scooterLoanAttempt = 0;
        }
    }
    else {
        cout << "Chatbot: Type 'show' to see scooter options or 'menu' to return to main menu." << endl;
    }
}

void ScooterLoanHandler::handleMakeState(const string& input) {
    if (input == "b") {
        cout << "Chatbot: Returning to main menu. Press S for scooter loan or X to exit." << endl;
        chatState = "main";
        return;
    }

    if (!isValidMakeOption(input)) {
        cout << "Chatbot: Invalid option. Please select a valid make number or B to go back." << endl;
        return;
    }

    int selectedMakeIndex = stoi(input) - 1;
    string selectedMake = getAvailableMakes()[selectedMakeIndex];
    displayScooterModels(selectedMake);
}

void ScooterLoanHandler::handleModelState(const string& input) {
    if (input == "b") {
        displayScooterMakes();
        chatState = "scooter_loan_make";
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
        int loanIndex = currentOptionsLoanIndices[selectedIndex];
        displayInstallmentPlan(selectedIndex);

        cout << "\nChatbot: Would you like to apply for this loan? (yes/no): ";
        string applyResponse;
        getline(cin, applyResponse);
        transform(applyResponse.begin(), applyResponse.end(), applyResponse.begin(), ::tolower);

        if (applyResponse == "yes" || applyResponse == "y") {
            handleApplyState(loanIndex);
        }
        else {
            cout << "Chatbot: Press S for another scooter loan or X to exit." << endl;
            chatState = "main";
        }
    }
    else {
        cout << "Chatbot: Invalid selection. Please type a number from 1 to "
            << currentOptionsCount << ", B to go back, or X to exit." << endl;
    }
}

void ScooterLoanHandler::handleApplyState(int loanIndex) {
    cout << "\n=== STARTING SCOOTER LOAN APPLICATION ===" << endl;

    Applicant applicant;
    applicant.collectData();

    if (!applicant.applicationId.empty()) {
        cout << "Chatbot: Application process completed. Press S for another Scooter loan or X to exit." << endl;
    }
    else {
        cout << "Press S to try again or X to exit." << endl;
    }

    // Ask for starting month
    int startMonth = 1;
    cout << "Chatbot: Enter starting month for installment plan (1-12): ";
    string monthInput;
    getline(cin, monthInput);

    try {
        int m = stoi(monthInput);
        if (m > 0) startMonth = m;
    }
    catch (...) {
        startMonth = 1;
    }

    displayInstallmentPlanWithMonths(loanIndex, startMonth);

    cout << "\nChatbot: Press S for another scooter loan or C for car loan or H for home loan or Q to check Application with CNIC or X to exit." << endl;
    chatState = "main";
}

void ScooterLoanHandler::displayScooterMakes() {
    auto availableMakes = getAvailableMakes();
    cout << "Chatbot: Available scooter makes:" << endl;
    for (size_t i = 0; i < availableMakes.size(); i++) {
        cout << "  " << (i + 1) << ". " << availableMakes[i] << endl;
    }
    cout << "Chatbot: Please select a make number or B to go back." << endl;
}

void ScooterLoanHandler::displayScooterModels(const string& selectedMake) {
    currentOptionsLoanIndices.clear();
    currentOptionsCount = 0;

    cout << "Chatbot: Options for " << selectedMake << ":" << endl;

    for (size_t i = 0; i < scooterLoans.size(); i++) {
        if (scooterLoans[i].getMake() == selectedMake) {
            currentOptionsLoanIndices.push_back(static_cast<int>(i));
            cout << "  " << (currentOptionsCount + 1) << ". " << scooterLoans[i].getModel()
                << ", Price: " << scooterLoans[i].getPrice()
                << ", Down Payment: " << scooterLoans[i].getDownPayment()
                << " (for " << scooterLoans[i].getInstallments() << " months)" << endl;
            currentOptionsCount++;
        }
    }

    if (currentOptionsCount > 0) {
        cout << "\nChatbot: Please select an option number to view the detailed plan." << endl;
        chatState = "scooter_loan_model";
    }
    else {
        cout << "Chatbot: No models found for selected make." << endl;
        displayScooterMakes();
    }
}

void ScooterLoanHandler::displayInstallmentPlan(int selectedIndex) {
    int loanIndex = currentOptionsLoanIndices[selectedIndex];
    cout << "\n=== DETAILED INSTALLMENT PLAN ===" << endl;
    scooterLoans[loanIndex].printInstallmentPlan();
}

void ScooterLoanHandler::displayInstallmentPlanWithMonths(int loanIndex, int startMonth) {
    scooterLoans[loanIndex].printInstallmentPlanStartingAt(startMonth);
}

vector<string> ScooterLoanHandler::getAvailableMakes() {
    vector<string> makes;
    for (const auto& loan : scooterLoans) {
        if (find(makes.begin(), makes.end(), loan.getMake()) == makes.end()) {
            makes.push_back(loan.getMake());
        }
    }
    return makes;
}

bool ScooterLoanHandler::isValidMakeOption(const string& input) {
    auto makes = getAvailableMakes();
    for (size_t i = 0; i < makes.size(); i++) {
        if (input == to_string(i + 1)) return true;
    }
    return false;
}