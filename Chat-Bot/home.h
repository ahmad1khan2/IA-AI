#ifndef HOME_H
#define HOME_H

#include <string>
#include <vector>
#include "loan.h"
#include "fileHandling.h"
#include "applicant.h"

using namespace std;

class HomeLoanHandler {
private:
    string chatState;
    int homeLoanAttempt;
    vector<homeLoan> homeLoans;
    int currentOptionsCount;
    vector<string> currentOptionsSize;
    vector<string> currentOptionsPrice;
    vector<string> currentOptionsDownPayment;
    vector<string> currentOptionsMonths;

    // State handler functions
    void handleMainState(const string& input);
    void handleAreaState(const string& input);
    void handleSelectState(const string& input);
    void handleApplyState();

    // Helper functions
    void displayHomeLoanOptions(const string& targetArea);
    void displayInstallmentPlan(int selectedIndex);
    int parseFormattedNumber(const string& str);
    bool isValidAreaOption(const string& input);
    string getAvailableAreas();

public:
    HomeLoanHandler();
    void runHome();
};

#endif