#pragma once
#ifndef CAR_H
#define CAR_H

#include <string>
#include <vector>
#include "loan.h"
#include "applicant.h"

using namespace std;

class CarLoanHandler {
public:
    CarLoanHandler();

    void handleInput(const std::string& input);
    string getCurrentState() const { return chatState; }

private:
    // State
    string chatState;
    int carLoanAttempt;

    // Data
    vector<carLoan> carLoans;

    // Current options
    int currentOptionsCount;
    vector<int> currentOptionsLoanIndex; // track indices of loans currently displayed

    // State handlers
    void handleMainState(const string& input);
    void handleSelectState(const string& input);
    void handleApplyState(int selectedIndex); // pass selected loan index

    // Helper functions
    void displayCarOptions();
    void displayInstallmentPlan(int selectedIndex);
    void displayInstallmentPlanWithMonths(int selectedIndex, int month);

    // Utilities
    string boolToYesNo(bool v) const;
};

#endif