#include "Personal.h"
#include "fileHandling.h"
#include "applicant.h"  // Add this include
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

PersonalLoanHandler::PersonalLoanHandler()
    : chatState("main"), personalLoanAttempt(0), currentOptionsCount(0)
{
    personalLoans = Parser::readPersonal();
}

void PersonalLoanHandler::handleInput(const string& input) {
    string inputForMatching = input;
    transform(inputForMatching.begin(), inputForMatching.end(),
        inputForMatching.begin(), ::tolower);

    if (chatState == "main") {
        handleMainState(inputForMatching);
    }
    else if (chatState == "personal_loan_select") {
        handleSelectState(inputForMatching);
    }
}

void PersonalLoanHandler::handleMainState(const string& input) {
    if (input == "p" || input == "show") {
        if (personalLoans.empty()) {
            cout << "Chatbot: Sorry, no personal loan options are currently available." << endl;
        }
        else {
            cout << "Chatbot: Available personal loan options:" << endl;
            displayPersonalOptions();
            chatState = "personal_loan_select";
        }
    }
    else {
        cout << "Chatbot: Type 'show' to see personal loan options or 'menu' to return to main menu." << endl;
    }
}

void PersonalLoanHandler::displayPersonalOptions() {
    currentOptionsCount = 0;
    currentOptionsLoanIndex.clear();

    for (int i = 0; i < personalLoans.size(); i++) {
        const auto& loan = personalLoans[i];
        currentOptionsLoanIndex.push_back(i);

        cout << "  " << (currentOptionsCount + 1) << ". "
            << loan.getLoanType()
            << ", Max Amount: " << formatCurrency(loan.getMaxAmount())
            << ", Interest Rate: " << fixed << setprecision(1) << loan.getInterestRate() << "%"
            << ", Processing Fee: " << formatCurrency(loan.getProcessingFee())
            << " (for " << loan.getInstallments() << " months)" << endl;

        currentOptionsCount++;
    }

    if (currentOptionsCount > 0) {
        cout << "\nChatbot: Please select an option number to view the detailed installment plan." << endl;
    }
    else {
        cout << "Chatbot: No personal loan options found." << endl;
        chatState = "main";
    }
}

void PersonalLoanHandler::handleSelectState(const string& input) {
    if (input == "b") {
        cout << "Chatbot: Returning to main menu. Press P for personal loan or X to exit." << endl;
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
        int realIndex = currentOptionsLoanIndex[selectedIndex];
        const personalLoan& selectedLoan = personalLoans[realIndex];

        // VALIDATION: Loan amount input
        int userAmount = 0;
        while (true) {
            cout << "Chatbot: Enter your desired loan amount (max: " << formatCurrency(selectedLoan.getMaxAmount()) << "): ";
            string amountInput;
            getline(cin, amountInput);

            try {
                // Remove commas and spaces from input
                amountInput.erase(remove(amountInput.begin(), amountInput.end(), ','), amountInput.end());
                amountInput.erase(remove(amountInput.begin(), amountInput.end(), ' '), amountInput.end());

                // Check if input contains only digits
                if (!all_of(amountInput.begin(), amountInput.end(), ::isdigit)) {
                    cout << "Error: Amount must contain only numeric values without commas." << endl;
                    continue;
                }

                userAmount = stoi(amountInput);

                if (userAmount <= 0) {
                    cout << "Error: Amount must be positive." << endl;
                }
                else if (userAmount < 10000) {
                    cout << "Error: Minimum loan amount is 10,000 PKR." << endl;
                }
                else if (userAmount > selectedLoan.getMaxAmount()) {
                    cout << "Error: Amount exceeds maximum limit of " << formatCurrency(selectedLoan.getMaxAmount()) << endl;
                }
                else {
                    break; // Valid amount
                }
            }
            catch (...) {
                cout << "Error: Please enter a valid numeric amount." << endl;
            }
        }

        // Create user-specific loan with their amount
        personalLoan userLoan(
            selectedLoan.getLoanType(),
            selectedLoan.getMaxAmount(),
            selectedLoan.getInterestRate(),
            selectedLoan.getProcessingFee(),
            selectedLoan.getInstallments(),
            userAmount,
            selectedLoan.getDownPayment()
        );

        // Display installment plan with user's amount
        cout << "\n=== DETAILED INSTALLMENT PLAN FOR " << formatCurrency(userAmount) << " ===" << endl;
        userLoan.printInstallmentPlan();

        // VALIDATION: Yes/No response
        string applyResponse;
        while (true) {
            cout << "\nChatbot: Would you like to apply for this loan? (yes/no): ";
            getline(cin, applyResponse);
            transform(applyResponse.begin(), applyResponse.end(), applyResponse.begin(), ::tolower);

            if (applyResponse == "yes" || applyResponse == "y" || applyResponse == "no" || applyResponse == "n") {
                break;
            }
            else {
                cout << "Error: Please enter 'yes' or 'no'." << endl;
            }
        }

        if (applyResponse == "yes" || applyResponse == "y") {
            handleApplyState(selectedIndex, userAmount);
        }
        else {
            cout << "Chatbot: Press P for another personal loan or X to exit." << endl;
            chatState = "main";
        }
    }
    else {
        cout << "Chatbot: Invalid selection. Please type a number from 1 to "
            << currentOptionsCount << ", B to go back, or X to exit." << endl;
    }
}

void PersonalLoanHandler::handleApplyState(int selectedIndex, int userAmount) {
    cout << "\n=== STARTING PERSONAL LOAN APPLICATION PROCESS ===" << endl;

    // Get the selected loan details
    int realIndex = currentOptionsLoanIndex[selectedIndex];
    const personalLoan& selectedLoan = personalLoans[realIndex];

    // Start the actual application process like other loan types
    Applicant applicant;
    applicant.collectData();

    // Check if application was actually completed or just exited
    if (applicant.getStatus() == "Submitted") {
        cout << "Chatbot: Application process completed. Press P for another Personal loan or X to exit." << endl;

        // Only ask for installment plan if application was completed
        int startMonth = 1;
        while (true) {
            cout << "Chatbot: Enter starting month for installment plan (1-12): ";
            string monthInput;
            getline(cin, monthInput);

            try {
                monthInput.erase(remove(monthInput.begin(), monthInput.end(), ' '), monthInput.end());

                if (!all_of(monthInput.begin(), monthInput.end(), ::isdigit)) {
                    cout << "Error: Month must be a number between 1 and 12." << endl;
                    continue;
                }

                int m = stoi(monthInput);
                if (m >= 1 && m <= 12) {
                    startMonth = m;
                    break;
                }
                else {
                    cout << "Error: Month must be between 1 and 12." << endl;
                }
            }
            catch (...) {
                cout << "Error: Please enter a valid month number (1-12)." << endl;
            }
        }

        // Create user-specific loan for month-based plan
        personalLoan userLoan(
            selectedLoan.getLoanType(),
            selectedLoan.getMaxAmount(),
            selectedLoan.getInterestRate(),
            selectedLoan.getProcessingFee(),
            selectedLoan.getInstallments(),
            userAmount,
            selectedLoan.getDownPayment()
        );

        // Show installment plan with months
        cout << "\n=== DETAILED INSTALLMENT PLAN FOR " << formatCurrency(userAmount) << " (STARTING AT MONTH " << startMonth << ") ===" << endl;
        userLoan.printInstallmentPlanStartingAt(startMonth);
    }
    else if (applicant.applicationId.empty()) {
        // Application not found - no message needed (already shown in resume)
    }
    else {
        // Application was exited, not completed
        cout << "Chatbot: Application saved." << endl;
    }

    cout << "\nChatbot: Press P for another personal loan or C for car loan or H for home loan or S for scooter loan or Q to check Application with CNIC or X to exit." << endl;
    chatState = "main";
}

void PersonalLoanHandler::displayInstallmentPlan(int selectedIndex) {
    int realIndex = currentOptionsLoanIndex[selectedIndex];
    const auto& loan = personalLoans[realIndex];
    cout << "\n=== DETAILED INSTALLMENT PLAN ===" << endl;
    loan.printInstallmentPlan();
}

void PersonalLoanHandler::displayInstallmentPlanWithMonths(int selectedIndex, int month) {
    int realIndex = currentOptionsLoanIndex[selectedIndex];
    const auto& loan = personalLoans[realIndex];
    cout << "\n=== DETAILED INSTALLMENT PLAN (STARTING AT MONTH " << month << ") ===" << endl;
    loan.printInstallmentPlanStartingAt(month);
}

string PersonalLoanHandler::formatCurrency(int amount) const {
    stringstream ss;
    ss.imbue(locale(""));
    ss << fixed << amount;
    return ss.str();
}