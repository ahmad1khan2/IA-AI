#pragma once
#ifndef PERSONAL_H
#define PERSONAL_H

#include <string>
#include <vector>
#include "loan.h"
#include "applicant.h"  // Add this include

using namespace std;

class PersonalLoanHandler {
public:
    PersonalLoanHandler();

    void handleInput(const std::string& input);
    string getCurrentState() const { return chatState; }

private:
    // State
    string chatState;
    int personalLoanAttempt;

    // Data
    vector<personalLoan> personalLoans;

    // Current options
    int currentOptionsCount;
    vector<int> currentOptionsLoanIndex;

    // State handlers
    void handleMainState(const string& input);
    void handleSelectState(const string& input);
    void handleApplyState(int selectedIndex, int userAmount);  // Add userAmount parameter

    // Helper functions
    void displayPersonalOptions();
    void displayInstallmentPlan(int selectedIndex);
    void displayInstallmentPlanWithMonths(int selectedIndex, int month);

    // Utilities
    string formatCurrency(int amount) const;
};

#endif