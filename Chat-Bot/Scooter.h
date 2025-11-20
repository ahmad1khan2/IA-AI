#pragma once
#ifndef SCOOTER_H
#define SCOOTER_H
#include "loan.h"
#include "fileHandling.h"
#include "applicant.h"
#include <string>
#include <vector>

using namespace std;

class ScooterLoanHandler {
public:
    ScooterLoanHandler();

    void handleInput(const std::string& input);
    string getCurrentState() const { return chatState; }

private:
    // State
    string chatState;
    int scooterLoanAttempt;

    // Data
    vector<scooterLoan> scooterLoans;

    // Current options
    int currentOptionsCount;
    vector<int> currentOptionsLoanIndices; // track indices of loans currently displayed

    // State handlers
    void handleMainState(const string& input);
    void handleMakeState(const string& input);
    void handleModelState(const string& input);
    void handleApplyState(int selectedIndex); // pass selected loan index

    // Helper functions
    void displayScooterMakes();
    void displayScooterModels(const string& selectedMake);
    void displayInstallmentPlan(int selectedIndex);
    void displayInstallmentPlanWithMonths(int loanIndex, int month);

    // Utilities
    vector<string> getAvailableMakes();
    bool isValidMakeOption(const string& input);
};
#endif