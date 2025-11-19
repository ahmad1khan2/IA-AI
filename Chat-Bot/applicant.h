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
private:
    string generateApplicationId();
    bool saveToFile();
    void showApplicationSummary();
};

bool isValidCNIC(const std::string& cnic);
bool isValidDate(const std::string& date);
bool isValidEmail(const std::string& email);
bool isValidEmploymentStatus(const std::string& status);
bool isValidLoanCategory(const std::string& category);
bool isValidLoanStatus(const std::string& status);
bool isValidMaritalStatus(const std::string& status);
bool isValidName(const std::string& name);
bool isValidNumeric(const std::string& num);
bool isValidPhone(const std::string& phone);
bool isValidPostalCode(const std::string& postalCode);

#endif