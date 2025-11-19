#include "scooter.h"
#include "applicant.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

ScooterLoanHandler::ScooterLoanHandler()
    : chatState("main"), scooterLoanAttempt(0), currentOptionsCount(0) {
    scooterLoans = Parser::readScooter();
    cout << "Debug: Loaded " << scooterLoans.size() << " scooter loans" << endl;
}

void ScooterLoanHandler::runScooter() {
    cout << "Scooter Loan Chatbot started. Type something (X to exit)" << endl;
    string input;

    while (true) {
        cout << "You: ";
        getline(cin, input);
        string inputForMatching = input;
        if (inputForMatching.length() == 1)
            inputForMatching[0] = toupper(inputForMatching[0]);
        else
            transform(inputForMatching.begin(), inputForMatching.end(), inputForMatching.begin(), ::tolower);

        if (inputForMatching == "X") {
            cout << "Chatbot: Goodbye!" << endl;
            break;
        }

        if (chatState == "main") handleMainState(inputForMatching);
        else if (chatState == "scooter_loan_make") handleMakeState(inputForMatching);
        else if (chatState == "scooter_loan_model") handleModelState(inputForMatching);
    }
}

void ScooterLoanHandler::handleMainState(const string& input) {
    if (input == "S") {
        if (scooterLoans.empty())
            cout << "Chatbot: Sorry, no scooter loan options are currently available." << endl;
        else {
            displayScooterMakes();
            chatState = "scooter_loan_make";
            scooterLoanAttempt = 0;
        }
    }
    else cout << "Chatbot: Please press S for scooter loan or X to exit." << endl;
}

void ScooterLoanHandler::handleMakeState(const string& input) {
    if (input == "B") { chatState = "main"; return; }
    if (!isValidMakeOption(input)) { cout << "Invalid make option.\n"; return; }

    int selectedMakeIndex = stoi(input) - 1;
    string selectedMake = getAvailableMakes()[selectedMakeIndex];
    displayScooterModels(selectedMake);
}

void ScooterLoanHandler::handleModelState(const string& input) {
    if (input == "B") { displayScooterMakes(); chatState = "scooter_loan_make"; return; }

    int selectedIndex = stoi(input) - 1;
    if (selectedIndex >= 0 && selectedIndex < currentOptionsCount) {
        int loanIndex = currentOptionsLoanIndices[selectedIndex];
        displayInstallmentPlan(selectedIndex);

        cout << "\nChatbot: Would you like to apply for this loan? (yes/no): ";
        string applyResponse;
        getline(cin, applyResponse);
        transform(applyResponse.begin(), applyResponse.end(), applyResponse.begin(), ::tolower);

        if (applyResponse == "yes" || applyResponse == "y") handleApplyState(loanIndex);
        else { cout << "Chatbot: Press S for another scooter loan or X to exit.\n"; chatState = "main"; }
    }
    else cout << "Chatbot: Invalid selection.\n";
}

void ScooterLoanHandler::handleApplyState(int loanIndex) {
    cout << "\n=== STARTING SCOOTER LOAN APPLICATION ===\n";

    Applicant applicant;
    applicant.collectData();

    if (!applicant.applicationId.empty()) {
        cout << "Chatbot: Application process completed. Press S for another Scooter loan or X to exit." << endl;
    }
    else {
        cout << "Press S to try again or X to exit." << endl;
    }
    // Automatically ask for starting month after application
    cout << "\nChatbot: Enter starting month for installment plan (1-12): ";
    int startMonth;
    cin >> startMonth;
    cin.ignore();  // clear newline from input buffer

    displayInstallmentPlanWithMonths(loanIndex, startMonth);

    cout << "\nChatbot: Application process completed. Press S for another scooter loan or X to exit.\n";
    chatState = "main";
}


void ScooterLoanHandler::displayScooterMakes() {
    auto availableMakes = getAvailableMakes();
    cout << "Scooter makes:\n";
    for (size_t i = 0; i < availableMakes.size(); i++)
        cout << "  " << (i + 1) << ". " << availableMakes[i] << endl;
    cout << "Select a make or B to go back.\n";
}

void ScooterLoanHandler::displayScooterModels(const string& selectedMake) {
    currentOptionsLoanIndices.clear();
    currentOptionsCount = 0;

    for (size_t i = 0; i < scooterLoans.size(); i++) {
        if (scooterLoans[i].getMake() == selectedMake) {
            currentOptionsLoanIndices.push_back(static_cast<int>(i));
            cout << "  " << (currentOptionsCount + 1) << ". " << scooterLoans[i].getModel()
                << ", Price: $" << scooterLoans[i].getPrice() << endl;
            currentOptionsCount++;
        }
    }

    if (currentOptionsCount > 0) {
        cout << "Select a model or B to go back.\n";
        chatState = "scooter_loan_model";
    }
    else cout << "No models found for selected make.\n";
}

void ScooterLoanHandler::displayInstallmentPlan(int selectedIndex) {
    int loanIndex = currentOptionsLoanIndices[selectedIndex];
    cout << "\n=== DETAILED INSTALLMENT PLAN ===\n";
    scooterLoans[loanIndex].printInstallmentPlan();
}

void ScooterLoanHandler::displayInstallmentPlanWithMonths(int loanIndex, int startMonth) {
    scooterLoans[loanIndex].printInstallmentPlanStartingAt(startMonth);
}

vector<string> ScooterLoanHandler::getAvailableMakes() {
    vector<string> makes;
    for (const auto& loan : scooterLoans)
        if (find(makes.begin(), makes.end(), loan.getMake()) == makes.end())
            makes.push_back(loan.getMake());
    return makes;
}

bool ScooterLoanHandler::isValidMakeOption(const string& input) {
    auto makes = getAvailableMakes();
    for (size_t i = 0; i < makes.size(); i++)
        if (input == to_string(i + 1)) return true;
    return false;
}
