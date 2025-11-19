#pragma once
#ifndef CAR_H
#define CAR_H

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

    // Updated: take selected loan index
    void handleApplyState(int selectedIndex);

    // Display helpers
    void displayCarOptions();
    void displayInstallmentPlan(int selectedIndex);
    void displayInstallmentPlanWithMonths(int selectedIndex, int month);

    // Utilities
    std::string boolToYesNo(bool v) const;

    // Data
    std::vector<carLoan> carLoans;          // all car loan objects

    // Interaction state
    std::string chatState;                  // "main", "car_loan_select"
    int currentOptionsCount;                // number of options currently shown

    // Only store the INDEX into carLoans for current options
    std::vector<int> currentOptionsLoanIndex;
};
#endif
