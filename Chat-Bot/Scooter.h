#ifndef SCOOTER_H
#define SCOOTER_H

#include "loan.h"
#include "fileHandling.h"
#include <string>
#include <vector>

using namespace std;

class ScooterLoanHandler {
private:
    string chatState;
    int scooterLoanAttempt;
    int currentOptionsCount;
    vector<scooterLoan> scooterLoans;

    // Current selection storage
    vector<string> currentOptionsMake;
    vector<string> currentOptionsModel;
    vector<int> currentOptionsDistance;
    vector<double> currentOptionsChargingTime;
    vector<int> currentOptionsMaxSpeed;
    vector<int> currentOptionsPrice;
    vector<int> currentOptionsDownPayment;
    vector<int> currentOptionsMonths;

    // Handler methods
    void handleMainState(const string& input);
    void handleMakeState(const string& input);
    void handleModelState(const string& input);
    void handleApplyState();

    // Display methods
    void displayScooterMakes();
    void displayScooterModels(const string& selectedMake);
    void displayInstallmentPlan(int selectedIndex);

    // Helper methods
    vector<string> getAvailableMakes();
    bool isValidMakeOption(const string& input);

public:
    ScooterLoanHandler();
    void runScooter();
};

#endif