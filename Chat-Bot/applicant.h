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
        string due = "", string bank = "", string category = "") {
        loanStatus = status;
        totalAmount = total;
        amountReturned = returned;
        amountDue = due;
        bankName = bank;
        loanCategory = category;
    }
};

class Referee {
public:
    string name;
    string cnic;
    string issueDate;
    string phone;
    string email;

    Referee(string n = "", string c = "", string date = "", string p = "", string e = "") {
        name = n;
        cnic = c;
        issueDate = date;
        phone = p;
        email = e;
    }
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

    Applicant() {
        status = "submitted";
    }

    // Method to collect all data - you call this from main
    void collectData() {
        cout << "\n=== LOAN APPLICATION FORM ===" << endl;

        // Personal Information
        cout << "Full Name: ";
        getline(cin, fullName);

        cout << "Father's Name: ";
        getline(cin, fathersName);

        cout << "Postal Address: ";
        getline(cin, postalAddress);

        cout << "Contact Number: ";
        getline(cin, contactNumber);

        cout << "Email Address: ";
        getline(cin, email);

        cout << "CNIC (without dashes): ";
        getline(cin, cnic);

        cout << "CNIC Expiry Date (DD-MM-YYYY): ";
        getline(cin, cnicExpiry);

        // Employment & Status
        cout << "Employment Status (Self-employed/salaried/retired/unemployed): ";
        getline(cin, employmentStatus);

        cout << "Marital Status (Single/Married/Divorced/Widowed): ";
        getline(cin, maritalStatus);

        cout << "Gender: ";
        getline(cin, gender);

        cout << "Number of Dependents: ";
        getline(cin, dependents);

        // Financial Information
        cout << "Annual Income (PKR without commas): ";
        getline(cin, annualIncome);

        cout << "Monthly Average Electricity Bill (PKR): ";
        getline(cin, avgElectricityBill);

        cout << "Current Electricity Bill Amount (PKR): ";
        getline(cin, currentElectricityBill);

        // Existing Loans
        cout << "Do you have existing loans? (yes/no): ";
        string hasLoans;
        getline(cin, hasLoans);

        if (hasLoans == "yes") {
            string status, total, returned, due, bank, category;

            cout << "Loan Status (Active/Inactive): ";
            getline(cin, status);

            cout << "Total Loan Amount: ";
            getline(cin, total);

            cout << "Amount Returned: ";
            getline(cin, returned);

            cout << "Amount Still Due: ";
            getline(cin, due);

            cout << "Bank Name: ";
            getline(cin, bank);

            cout << "Loan Category (Car/Home/Bike): ";
            getline(cin, category);

            addExistingLoan(status, total, returned, due, bank, category);
        }

        // References
        cout << "\n=== REFERENCE 1 ===" << endl;
        cout << "Name: ";
        getline(cin, referee1.name);

        cout << "CNIC: ";
        getline(cin, referee1.cnic);

        cout << "CNIC Issue Date (DD-MM-YYYY): ";
        getline(cin, referee1.issueDate);

        cout << "Phone Number: ";
        getline(cin, referee1.phone);

        cout << "Email: ";
        getline(cin, referee1.email);

        cout << "\n=== REFERENCE 2 ===" << endl;
        cout << "Name: ";
        getline(cin, referee2.name);

        cout << "CNIC: ";
        getline(cin, referee2.cnic);

        cout << "CNIC Issue Date (DD-MM-YYYY): ";
        getline(cin, referee2.issueDate);

        cout << "Phone Number: ";
        getline(cin, referee2.phone);

        cout << "Email: ";
        getline(cin, referee2.email);
    }

    void addExistingLoan(string status, string total, string returned, string due, string bank, string category) {
        ExistingLoan loan(status, total, returned, due, bank, category);
        existingLoans.push_back(loan);
    }

    string toFileString() {
        string data = "";

        // Personal Information
        data += applicationId + "#" + fullName + "#" + fathersName + "#" +
            postalAddress + "#" + contactNumber + "#" + email + "#" +
            cnic + "#" + cnicExpiry + "#" + employmentStatus + "#" +
            maritalStatus + "#" + gender + "#" + dependents + "#";

        // Financial Information  
        data += annualIncome + "#" + avgElectricityBill + "#" + currentElectricityBill + "#";

        // Existing Loans 
        if (existingLoans.empty()) {
            data += "No#No#No#No#No#No#";  // Clean "No" for all loan fields
        }
        else {
            data += existingLoans[0].loanStatus + "#" + existingLoans[0].totalAmount + "#" +
                existingLoans[0].amountReturned + "#" + existingLoans[0].amountDue + "#" +
                existingLoans[0].bankName + "#" + existingLoans[0].loanCategory + "#";
        }

        // References
        data += referee1.name + "#" + referee1.cnic + "#" + referee1.issueDate + "#" +
            referee1.phone + "#" + referee1.email + "#" +
            referee2.name + "#" + referee2.cnic + "#" + referee2.issueDate + "#" +
            referee2.phone + "#" + referee2.email + "#";

        //// Image Paths (placeholders)
        //data += cnicFrontPath + "#" + cnicBackPath + "#" +
        //    electricityBillPath + "#" + salarySlipPath + "#";

        // Status
        data += status;

        return data;
    }
};

#endif