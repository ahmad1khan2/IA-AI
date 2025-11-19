#include "scooter.h"
#include "applicant.h"
#include <iostream>
#include <iomanip>
#include <sstream>
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
        else if (chatState == "scooter_loan_make") {
            handleMakeState(inputForMatching);
        }
        else if (chatState == "scooter_loan_model") {
            handleModelState(inputForMatching);
        }
    }
}

void ScooterLoanHandler::handleMainState(const string& input) {
    if (input == "S") {
        if (scooterLoans.empty()) {
            cout << "Chatbot: Sorry, no scooter loan options are currently available. Please try another loan type or check back later." << endl;
        }
        else {
            displayScooterMakes();
            chatState = "scooter_loan_make";
            scooterLoanAttempt = 0;
        }
    }
    else {
        cout << "Chatbot: Please press S for scooter loan or X to exit." << endl;
    }
}

void ScooterLoanHandler::handleMakeState(const string& input) {
    if (input == "B") {
        cout << "Chatbot: Returning to main menu. Please press S for scooter loan. Press X to exit." << endl;
        chatState = "main";
        return;
    }

    if (!isValidMakeOption(input)) {
        cout << "Chatbot: That is not a valid option. Please select from the available makes. Press B to go back or X to exit." << endl;
        return;
    }

    int selectedMakeIndex = stoi(input) - 1;
    string selectedMake = getAvailableMakes()[selectedMakeIndex];
    displayScooterModels(selectedMake);
}

void ScooterLoanHandler::handleModelState(const string& input) {
    if (input == "B") {
        displayScooterMakes();
        chatState = "scooter_loan_make";
    }
    else {
        int selectedIndex = -1;
        try {
            selectedIndex = stoi(input) - 1;
        }
        catch (...) {}

        if (selectedIndex >= 0 && selectedIndex < currentOptionsCount) {
            displayInstallmentPlan(selectedIndex);

            // Ask if they want to apply - using direct input to avoid case conversion issues
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
                cout << "Chatbot: Press S for another scooter loan or X to exit." << endl;
                chatState = "main";
            }
        }
        else {
            cout << "Chatbot: That is not a valid selection. Please type a number from 1 to "
                << currentOptionsCount << ", B to go back, or X to exit." << endl;
        }
    }
}

void ScooterLoanHandler::handleApplyState() {
    cout << "\n=== STARTING SCOOTER LOAN APPLICATION PROCESS ===" << endl;

    Applicant applicant;
    applicant.collectData();

    cout << "Chatbot: Application process completed. Press S for another scooter loan or X to exit." << endl;
    chatState = "main";
}

void ScooterLoanHandler::displayScooterMakes() {
    vector<string> availableMakes = getAvailableMakes();

    cout << "Chatbot: Available scooter makes:" << endl;
    for (size_t i = 0; i < availableMakes.size(); i++) {
        cout << "  " << (i + 1) << ". " << availableMakes[i] << endl;
    }
    cout << "\nChatbot: Please select a make (e.g., 1, 2) or press B to go back." << endl;
}

void ScooterLoanHandler::displayScooterModels(const string& selectedMake) {
    bool foundOptions = false;
    currentOptionsCount = 0;
    currentOptionsMake.clear();
    currentOptionsModel.clear();
    currentOptionsDistance.clear();
    currentOptionsChargingTime.clear();
    currentOptionsMaxSpeed.clear();
    currentOptionsPrice.clear();
    currentOptionsDownPayment.clear();
    currentOptionsMonths.clear();

    for (const auto& loan : scooterLoans) {
        if (loan.getMake() == selectedMake) {
            if (!foundOptions) {
                cout << "Chatbot: Here are the models for " << selectedMake << ":" << endl;
                foundOptions = true;
            }

            currentOptionsMake.push_back(loan.getMake());
            currentOptionsModel.push_back(loan.getModel());
            currentOptionsDistance.push_back(loan.getDistancePerChargeKm());
            currentOptionsChargingTime.push_back(loan.getChargingTimeHrs());
            currentOptionsMaxSpeed.push_back(loan.getMaxSpeedKmH());
            currentOptionsPrice.push_back(loan.getPrice());
            currentOptionsDownPayment.push_back(loan.getDownPayment());
            currentOptionsMonths.push_back(loan.getInstallments());

            cout << "  " << (currentOptionsCount + 1) << ". " << loan.getModel()
                << ", Range: " << loan.getDistancePerChargeKm() << "km"
                << ", Charging: " << loan.getChargingTimeHrs() << "hrs"
                << ", Max Speed: " << loan.getMaxSpeedKmH() << "km/h"
                << ", Price: $" << loan.getPrice() << endl;

            currentOptionsCount++;
        }
    }

    if (foundOptions) {
        cout << "\nChatbot: Please select a model (e.g., 1, 2) to see the detailed plan." << endl;
        cout << "Chatbot: Press B to go back to make selection." << endl;
        chatState = "scooter_loan_model";
    }
    else {
        cout << "Chatbot: Sorry, no models found for the selected make. Please try again." << endl;
    }
}

void ScooterLoanHandler::displayInstallmentPlan(int selectedIndex) {
    // Create a temporary scooterLoan object to use its print functionality
    scooterLoan selectedLoan(
        currentOptionsMake[selectedIndex],
        currentOptionsModel[selectedIndex],
        currentOptionsDistance[selectedIndex],
        currentOptionsChargingTime[selectedIndex],
        currentOptionsMaxSpeed[selectedIndex],
        currentOptionsMonths[selectedIndex],
        currentOptionsPrice[selectedIndex],
        currentOptionsDownPayment[selectedIndex]
    );

    cout << "\n=== DETAILED INSTALLMENT PLAN ===" << endl;
    cout << "Chatbot: Here's the detailed installment plan for "
        << currentOptionsMake[selectedIndex] << " " << currentOptionsModel[selectedIndex] << ":\n";
    selectedLoan.printInstallmentPlan();
}

vector<string> ScooterLoanHandler::getAvailableMakes() {
    vector<string> availableMakes;
    for (const auto& loan : scooterLoans) {
        if (find(availableMakes.begin(), availableMakes.end(), loan.getMake()) == availableMakes.end()) {
            availableMakes.push_back(loan.getMake());
        }
    }
    return availableMakes;
}

bool ScooterLoanHandler::isValidMakeOption(const string& input) {
    vector<string> availableMakes = getAvailableMakes();
    for (size_t i = 0; i < availableMakes.size(); i++) {
        if (input == to_string(i + 1)) {
            return true;
        }
    }
    return false;
}