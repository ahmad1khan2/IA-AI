#include "Car.h"
#include "fileHandling.h"   // Parser::readCar / stoiSafe etc.
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

CarLoanHandler::CarLoanHandler()
    : chatState("main"), currentOptionsCount(0) {
    // Load car loans using parser
    carLoans = Parser::readCar();
    cout << "Debug: Loaded " << carLoans.size() << " car loans" << endl;
}

void CarLoanHandler::runCar() {
    cout << "Car Loan Chatbot started. Type something (X to exit)" << endl;
    string input;

    while (true) {
        cout << "You: ";
        getline(cin, input);

        string inputForMatching = input;
        if (inputForMatching.length() == 1) {
            inputForMatching[0] = toupper(inputForMatching[0]);
        } else {
            for (size_t k = 0; k < inputForMatching.length(); k++) {
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
        else if (chatState == "car_loan_select") {
            handleSelectState(inputForMatching);
        }
    }
}

void CarLoanHandler::handleMainState(const string& input) {
    if (input == "C") {
        if (carLoans.empty()) {
            cout << "Chatbot: Sorry, no car loan options are currently available. Please try another loan type or check back later." << endl;
        } else {
            cout << "Chatbot: You are applying for a car loan. Showing available car options." << endl;
            displayCarOptions();
            chatState = "car_loan_select";
        }
    } else {
        cout << "Chatbot: Please press C for car loan or X to exit." << endl;
    }
}

void CarLoanHandler::displayCarOptions() {
    currentOptionsCount = 0;
    currentOptionsMake.clear();
    currentOptionsModel.clear();
    currentOptionsEngine.clear();
    currentOptionsUsed.clear();
    currentOptionsYear.clear();
    currentOptionsPrice.clear();
    currentOptionsDownPayment.clear();
    currentOptionsMonths.clear();

    cout << "Chatbot: Here are available car loan options:" << endl;
    for (const auto& loan : carLoans) {
        currentOptionsMake.push_back(loan.getMake());
        currentOptionsModel.push_back(loan.getModel());
        currentOptionsEngine.push_back(to_string(loan.getEngine()));
        currentOptionsUsed.push_back(loan.isUsed() ? "Yes" : "No");
        currentOptionsYear.push_back(to_string(loan.getYear()));
        currentOptionsPrice.push_back(to_string(loan.getPrice()));
        currentOptionsDownPayment.push_back(to_string(loan.getDownPayment()));
        currentOptionsMonths.push_back(to_string(loan.getInstallments()));

        cout << "  " << (currentOptionsCount + 1) << ". "
             << loan.getMake() << " " << loan.getModel()
             << " (" << loan.getYear() << (loan.isUsed() ? ", used" : ", new") << ")"
             << " - Price: " << loan.getPrice()
             << ", Down: " << loan.getDownPayment()
             << " (for " << loan.getInstallments() << " months)" << endl;

        currentOptionsCount++;
    }

    if (currentOptionsCount > 0) {
        cout << "\nChatbot: Please select an option number to view the detailed installment plan." << endl;
        cout << "Chatbot: Press B to go back to the main menu." << endl;
    } else {
        cout << "Chatbot: No car options found." << endl;
        chatState = "main";
    }
}

void CarLoanHandler::handleSelectState(const string& input) {
    if (input == "B") {
        cout << "Chatbot: Returning to main menu. Please press C for car loan. Press X to exit." << endl;
        chatState = "main";
        return;
    }

    if (input == "A") {
        // apply flow
        handleApplyState();
        return;
    }

    int selectedIndex = -1;
    try {
        selectedIndex = stoi(input) - 1;
    } catch (...) { selectedIndex = -1; }

    if (selectedIndex >= 0 && selectedIndex < currentOptionsCount) {
        displayInstallmentPlan(selectedIndex);

        cout << "\nChatbot: Would you like to apply for this loan? (yes/no): ";
        string applyResponse;
        getline(cin, applyResponse);
        for (char& c : applyResponse) c = tolower(c);

        if (applyResponse == "yes" || applyResponse == "y") {
            handleApplyState();
        } else {
            cout << "Chatbot: Press C for another car loan or X to exit." << endl;
            chatState = "main";
        }
    } else {
        cout << "Chatbot: That is not a valid selection. Please type a number from 1 to "
             << currentOptionsCount << ", B to go back, or X to exit." << endl;
    }
}

void CarLoanHandler::displayInstallmentPlan(int selectedIndex) {
    // Find the matching carLoan object by comparing stored option attributes
    string make = currentOptionsMake[selectedIndex];
    string model = currentOptionsModel[selectedIndex];
    int price = parseFormattedNumber(currentOptionsPrice[selectedIndex]);
    int down = parseFormattedNumber(currentOptionsDownPayment[selectedIndex]);
    int months = parseFormattedNumber(currentOptionsMonths[selectedIndex]);

    for (const auto& loan : carLoans) {
        if (loan.getMake() == make &&
            loan.getModel() == model &&
            loan.getPrice() == price &&
            loan.getDownPayment() == down &&
            loan.getInstallments() == months) {

            cout << "\n=== DETAILED INSTALLMENT PLAN ===" << endl;
            loan.printInstallmentPlan();
            return;
        }
    }

    cout << "Chatbot: Unable to locate matching loan to display plan." << endl;
}

void CarLoanHandler::handleApplyState() {
    cout << "\n=== STARTING CAR LOAN APPLICATION PROCESS ===" << endl;
    Applicant applicant;
    applicant.collectData();
    if (!applicant.applicationId.empty()) {
        cout << "Chatbot: Application process completed. Press C for another Car loan or X to exit." << endl;
    }
    else {
        cout << "Press C to try again or X to exit." << endl;
    }
    chatState = "main";
}

int CarLoanHandler::parseFormattedNumber(const string& str) {
    // Local implementation: remove commas, spaces and non-digit characters, then stoi
    string cleaned;
    for (char c : str) {
        if (isdigit(static_cast<unsigned char>(c)) || (c == '-' && cleaned.empty())) {
            cleaned.push_back(c);
        }
    }
    if (cleaned.empty()) return 0;
    try {
        return stoi(cleaned);
    } catch (...) {
        return 0;
    }
}

string CarLoanHandler::boolToYesNo(bool v) const {
    return v ? "Yes" : "No";
}
