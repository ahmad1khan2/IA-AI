#pragma once
#ifndef HOME_H
#define HOME_H

#include <string>
#include <vector>
#include "loan.h"
#include "fileHandling.h"
#include "applicant.h"

using namespace std;

class HomeLoanHandler {
public:
    HomeLoanHandler();
    void runHome();

private:
    // State
    string chatState;
    int homeLoanAttempt;

    // Data
    vector<homeLoan> homeLoans;

    // Current options
    int currentOptionsCount;
    vector<int> currentOptionsLoanIndex; // track indices of loans currently displayed

    // State handlers
    void handleMainState(const string& input);
    void handleAreaState(const string& input);
    void handleSelectState(const string& input);
    void handleApplyState(int selectedIndex); // pass selected loan index

    // Helper functions
    void displayHomeLoanOptions(const string& targetArea);
    void displayInstallmentPlan(int selectedIndex);
    void displayInstallmentPlanWithMonths(int selectedIndex, int month);

    // Utilities
    int parseFormattedNumber(const string& str);
    bool isValidAreaOption(const string& input);
    string getAvailableAreas();
};

#endif
