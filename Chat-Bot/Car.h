#pragma once

#include <string>
#include <vector>
#include "loan.h"
#include "applicant.h"

class CarLoanHandler {
public:
    CarLoanHandler();

    // Start interactive car-loan chatbot
    void runCar();

private:
    // State handlers
    void handleMainState(const std::string& input);
    void handleSelectState(const std::string& input);
    void handleApplyState();

    // UI / display helpers
    void displayCarOptions();
    void displayInstallmentPlan(int selectedIndex);

    // Utilities
    int parseFormattedNumber(const std::string& str);
    std::string boolToYesNo(bool v) const;

    // Data
    std::vector<carLoan> carLoans;

    // Interaction state
    std::string chatState; // "main" or "car_loan_select"
    int currentOptionsCount;

    // Vectors storing the currently shown option attributes (strings for easy comparison)
    std::vector<std::string> currentOptionsMake;
    std::vector<std::string> currentOptionsModel;
    std::vector<std::string> currentOptionsEngine;
    std::vector<std::string> currentOptionsUsed;
    std::vector<std::string> currentOptionsYear;
    std::vector<std::string> currentOptionsPrice;
    std::vector<std::string> currentOptionsDownPayment;
    std::vector<std::string> currentOptionsMonths;
};
