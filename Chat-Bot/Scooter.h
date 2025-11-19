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
private:
    string chatState;
    int scooterLoanAttempt;
    int currentOptionsCount;
    vector<scooterLoan> scooterLoans;

    // Only store indices into scooterLoans
    vector<int> currentOptionsLoanIndices;

    // Handler methods
    void handleMainState(const string& input);
    void handleMakeState(const string& input);
    void handleModelState(const string& input);
    void handleApplyState(int loanIndex);

    // Display methods
    void displayScooterMakes();
    void displayScooterModels(const string& selectedMake);
    void displayInstallmentPlan(int selectedIndex);
    void displayInstallmentPlanWithMonths(int loanIndex, int startMonth);

    // Helper methods
    vector<string> getAvailableMakes();
    bool isValidMakeOption(const string& input);

public:
    ScooterLoanHandler();
    void runScooter();
};
#endif