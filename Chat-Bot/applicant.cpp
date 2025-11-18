#include "applicant.h"
#include <fstream>
#include <iostream>
#include <string>  

using namespace std;

// ExistingLoan constructor 
ExistingLoan::ExistingLoan(string status, string total, string returned,
    string due, string bank, string category) {
    loanStatus = status;
    totalAmount = total;
    amountReturned = returned;
    amountDue = due;
    bankName = bank;
    loanCategory = category;
}

// Referee constructor 
Referee::Referee(string n, string c, string date, string p, string e) {
    name = n;
    cnic = c;
    issueDate = date;
    phone = p;
    email = e;
}

// Applicant constructor 
Applicant::Applicant() {
    status = "submitted";
}

// collectData 
void Applicant::collectData() {
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

    //Confirmation and file saving
    showApplicationSummary();

    cout << "\nDo you want to submit this application? (yes/no): ";
    string confirm;
    getline(cin, confirm);

    if (confirm == "yes") {
        applicationId = generateApplicationId();
        if (saveToFile()) {
            cout << "Application submitted successfully! Your Application ID: " << applicationId << endl;
        }
        else {
            cout << "Error saving application. Please try again." << endl;
        }
    }
    else {
        cout << "Application cancelled. Returning to main menu." << endl;
    }
}

// showApplicationSummary 
void Applicant::showApplicationSummary() {
    cout << "\n=== APPLICATION SUMMARY ===" << endl;
    cout << "Name: " << fullName << endl;
    cout << "CNIC: " << cnic << endl;
    cout << "Email: " << email << endl;
    cout << "Annual Income: " << annualIncome << endl;
}

// generateApplicationId
string Applicant::generateApplicationId() {
    static int counter = 1001;
    string id = to_string(counter);
    counter++;
    return id;
}

// saveToFile
bool Applicant::saveToFile() {
    ofstream file("applications.txt", ios::app);
    if (!file) {
        file.open("applications.txt");
        if (!file) {
            return false;
        }
    }

    string fileData = toFileString();
    file << fileData << endl;
    file.close();
    return true;
}

// toFileString
string Applicant::toFileString() {
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
        data += "No#No#No#No#No#No#";  //"No" for all loan fields
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

    // Status
    data += status;

    return data;
}

// addExistingLoan
void Applicant::addExistingLoan(string status, string total, string returned, string due, string bank, string category) {
    ExistingLoan loan(status, total, returned, due, bank, category);
    existingLoans.push_back(loan);
}

// searching count through CNIC
void countApplicationsByCNIC(string cnic) {
    ifstream file("applications.txt");
    if (!file) {
        cout << "Error: applications.txt not found!" << endl;
        return;
    }

    string line;
    int submitted = 0;
    int approved = 0;
    int rejected = 0;

    while (getline(file, line)) {
       // Split by #
        string fields[40];
        int idx = 0;
        string temp = "";

        for (int i = 0; i < line.length(); i++) {
            if (line[i] == '#') {
                fields[idx++] = temp;
                temp = "";
            }
            else {
                temp += line[i];
            }
        }
        fields[idx] = temp; // last one

        // CNIC is in index 6
        if (fields[6] == cnic) {
            submitted++;

            string status = fields[31];  // last field

            if (status == "approved" || status == "Approved")
                approved++;
            else if (status == "rejected" || status == "Rejected")
                rejected++;
        }
    }

    cout << "\n--- Application Count for CNIC: " << cnic << " ---\n";
    cout << "Submitted: " << submitted << endl;
    cout << "Approved:  " << approved << endl;
    cout << "Rejected:  " << rejected << endl;
}