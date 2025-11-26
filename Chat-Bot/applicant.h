#ifndef APPLICANT_H
#define APPLICANT_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class ExistingLoan {
public:
    string loanStatus;
    string totalAmount;
    string amountReturned;
    string amountDue;
    string bankName;
    string loanCategory;

    ExistingLoan(string status = "", string total = "", string returned = "",
        string due = "", string bank = "", string category = "");
};

class Referee {
public:
    string name;
    string cnic;
    string issueDate;
    string phone;
    string email;

    Referee(string n = "", string c = "", string date = "", string p = "", string e = "");
};

class Applicant {
private: 
    bool isExiting = false;
public:
    string applicationId;
    string fullName;
    string fathersName;
    string postalAddress;
    string contactNumber;
    string email;
    string cnic;
    string cnicExpiry;
    string employmentStatus;
    string maritalStatus;
    string gender;
    string dependents;
    string annualIncome;
    string avgElectricityBill;
    string currentElectricityBill;
    vector<ExistingLoan> existingLoans;
    Referee referee1;
    Referee referee2;
    string status;

    Applicant();
    void collectData();
    void addExistingLoan(string status, string total, string returned, string due, string bank, string category);
    string toFileString();
    string cnicFrontPath;
    string cnicBackPath;
    string electricityBillPath;
    string salarySlipPath;
    void handleImageUpload();
    bool copyFile(const string& sourcePath, const string& destPath);
    // Multi-session methods
    void startNewApplication();
    void resumeApplication();
    void continueApplication();
    void collectPersonalInfo();
    void collectFinancialInfo();
    void collectReferences();
    void collectDocuments();
    void saveCheckpoint(const string& checkpointStatus);
    bool loadApplication(const string& appId, const string& cnicInput);
    void showCurrentData();

    // Validation methods
    bool validatePersonalInfo();
    bool validateFinancialInfo();
    bool validateReferences();
    //exit at any point
    void checkForExit(const string& input, const string& currentStatus);
    string getStatus() const { return status; }
private:

    void showApplicationSummary();
};

// ============================
// INPUT VALIDATION FUNCTIONS DECLARATIONS
// ============================

// Helper function to check if string contains only digits
bool containsOnlyDigits(const string& str);

// Helper function to check email format
bool isValidEmail(const string& email);

// Helper function to validate date format DD-MM-YYYY
bool isValidDate(const string& date);

// Phone number validation - 11 digits
bool isValidPhone(const string& phone);

// Numeric validation for income and bills
bool isValidNumeric(const string& num);

// Employment status validation
bool isValidEmploymentStatus(const string& status);

// Marital status validation  
bool isValidMaritalStatus(const string& status);

// Loan status validation
bool isValidLoanStatus(const string& status);

// Loan category validation
bool isValidLoanCategory(const string& category);

// Name validation (only letters and spaces)
bool isValidName(const string& name);

// Postal code validation - 5 digits
bool isValidPostalCode(const string& postalCode);


#endif