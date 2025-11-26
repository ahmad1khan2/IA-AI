#include "applicant.h"
#include "fileHandling.h"
#include <fstream>
#include <iostream>
#include <string>  
#include <algorithm>

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
    status = "C0"; // Start with not started
    isExiting = false;  
}

// Multi-session application system
void Applicant::collectData() {
    cout << "\n=== LOAN APPLICATION SYSTEM ===" << endl;
    cout << "1. Start New Application" << endl;
    cout << "2. Resume Existing Application" << endl;
    cout << "Enter choice (1-2): ";

    string choice;
    getline(cin, choice);

    if (choice == "1") {
        // Start new application
        applicationId = Parser::generateApplicationId();
        status = "C0";

        try {
            startNewApplication();
        }
        catch (const runtime_error& e) {
            // Exception already handled in checkForExit
            return;
        }
    }
    else if (choice == "2") {
        // Resume existing application
        resumeApplication();
    }
    else {
        cout << "Invalid choice. Returning to main menu." << endl;
        return;
    }
}
void Applicant::startNewApplication() {
    cout << "\nStarting new application. Your Application ID: " << applicationId << endl;

    // Personal Information
    cout << "\n=== PERSONAL INFORMATION SECTION ===" << endl;
    collectPersonalInfo();
    saveCheckpoint("C1");

    // Financial Information
    cout << "\n=== FINANCIAL INFORMATION SECTION ===" << endl;
    collectFinancialInfo();
    saveCheckpoint("C2");

    // References
    cout << "\n=== REFERENCES SECTION ===" << endl;
    collectReferences();
    saveCheckpoint("C3");

    // Documents
    cout << "\n=== DOCUMENTS SECTION ===" << endl;
    collectDocuments();
    saveCheckpoint("Submitted");

    cout << "Application submitted successfully! Your Application ID: " << applicationId << endl;
}

void Applicant::resumeApplication() {
    string appId, cnicInput;

    cout << "\n=== RESUME APPLICATION ===" << endl;
    cout << "Enter Application ID: ";
    getline(cin, appId);
    cout << "Enter CNIC: ";
    getline(cin, cnicInput);

    if (loadApplication(appId, cnicInput)) {
        cout << "Application loaded successfully!" << endl;
        showCurrentData();
        continueApplication();
    }
    
}

void Applicant::continueApplication() {
    try {
        if (status == "C0" || status == "C1") {
            cout << "\n=== PERSONAL INFORMATION SECTION ===" << endl;
            if (status == "C1") {
                cout << "This section is already completed. Do you want to edit it? (yes/no): ";
                string edit;
                getline(cin, edit);
                if (edit == "yes" || edit == "y") {
                    collectPersonalInfo();
                }
            }
            else {
                collectPersonalInfo();
            }
            saveCheckpoint("C1");
        }

        if (status == "C1" || status == "C2") {
            cout << "\n=== FINANCIAL INFORMATION SECTION ===" << endl;
            if (status == "C2") {
                cout << "This section is already completed. Do you want to edit it? (yes/no): ";
                string edit;
                getline(cin, edit);
                if (edit == "yes" || edit == "y") {
                    collectFinancialInfo();
                }
            }
            else {
                collectFinancialInfo();
            }
            saveCheckpoint("C2");
        }

        if (status == "C2" || status == "C3") {
            cout << "\n=== REFERENCES SECTION ===" << endl;
            if (status == "C3") {
                cout << "This section is already completed. Do you want to edit it? (yes/no): ";
                string edit;
                getline(cin, edit);
                if (edit == "yes" || edit == "y") {
                    collectReferences();
                }
            }
            else {
                collectReferences();
            }
            saveCheckpoint("C3");
        }

        cout << "\n=== DOCUMENTS SECTION ===" << endl;
        collectDocuments();
        saveCheckpoint("Submitted");
        cout << "Application submitted successfully! Your Application ID: " << applicationId << endl;

    }
    catch (const runtime_error& e) {
        // Exit gracefully - already handled in checkForExit
        return;
    }
}

// collectPersonalInfo 
void Applicant::collectPersonalInfo() {
    cout << "\n=== PERSONAL INFORMATION ===" << endl;
    cout << "Type 'exit' at any time to save and leave application." << endl;

    // Full Name 
    do {
        cout << "Full Name: ";
        getline(cin, fullName);
        checkForExit(fullName, "C0");
        if (!isValidName(fullName)) {
            cout << "Error: Name must contain only alphabets and spaces" << endl;
        }
    } while (!isValidName(fullName));

    // Father's Name 
    do {
        cout << "Father's Name: ";
        getline(cin, fathersName);
        checkForExit(fathersName, "C0");
        if (!isValidName(fathersName)) {
            cout << "Error: Name must contain only alphabets and spaces" << endl;
        }
    } while (!isValidName(fathersName));

    // Postal Address 
    do {
        cout << "Postal Address: ";
        getline(cin, postalAddress);
        checkForExit(postalAddress, "C0");
        if (!isValidPostalCode(postalAddress)) {
            cout << "Error: Postal code must be 5 digits" << endl;
        }
    } while (!isValidPostalCode(postalAddress));

    // Contact Number 
    do {
        cout << "Contact Number: ";
        getline(cin, contactNumber);
        checkForExit(contactNumber, "C0");
        if (!isValidPhone(contactNumber)) {
            cout << "Error: Phone number must be 11 digits. Example: 03001234567" << endl;
        }
    } while (!isValidPhone(contactNumber));

    // Email 
    do {
        cout << "Email Address: ";
        getline(cin, email);
        checkForExit(email, "C0");
        if (!isValidEmail(email)) {
            cout << "Error: Please enter a valid email address. Example: user@example.com" << endl;
        }
    } while (!isValidEmail(email));

    // CNIC 
    do {
        cout << "CNIC (without dashes): ";
        getline(cin, cnic);
        checkForExit(cnic, "C0");
        if (!Parser::isValidCNIC(cnic)) {
            cout << "Error: CNIC must be 13 digits without dashes. Example: 1234567890123" << endl;
        }
    } while (!Parser::isValidCNIC(cnic));

    // CNIC Expiry
    do {
        cout << "CNIC Expiry Date (DD-MM-YYYY): ";
        getline(cin, cnicExpiry);
        checkForExit(cnicExpiry, "C0");
        if (!isValidDate(cnicExpiry)) {
            cout << "Error: Date must be in DD-MM-YYYY format. Example: 15-08-2030" << endl;
        }
    } while (!isValidDate(cnicExpiry));

    // Employment Status
    do {
        cout << "Employment Status:\n";
        cout << "1. Self-employed\n2. Salaried\n3. Retired\n4. Unemployed\n";
        cout << "Enter choice (1-4): ";
        getline(cin, employmentStatus);
        checkForExit(employmentStatus, "C0");

        if (employmentStatus == "1") employmentStatus = "Self-employed";
        else if (employmentStatus == "2") employmentStatus = "Salaried";
        else if (employmentStatus == "3") employmentStatus = "Retired";
        else if (employmentStatus == "4") employmentStatus = "Unemployed";
        else {
            cout << "Error: Please enter a number between 1-4" << endl;
            continue;
        }

        if (!isValidEmploymentStatus(employmentStatus)) {
            cout << "Error: Invalid employment status selection" << endl;
        }
    } while (!isValidEmploymentStatus(employmentStatus));

    // Marital Status
    do {
        cout << "Marital Status:\n";
        cout << "1. Single\n2. Married\n3. Divorced\n4. Widowed\n";
        cout << "Enter choice (1-4): ";
        getline(cin, maritalStatus);
        checkForExit(maritalStatus, "C0");

        if (maritalStatus == "1") maritalStatus = "Single";
        else if (maritalStatus == "2") maritalStatus = "Married";
        else if (maritalStatus == "3") maritalStatus = "Divorced";
        else if (maritalStatus == "4") maritalStatus = "Widowed";
        else {
            cout << "Error: Please enter a number between 1-4" << endl;
            continue;
        }

        if (!isValidMaritalStatus(maritalStatus)) {
            cout << "Error: Invalid marital status selection" << endl;
        }
    } while (!isValidMaritalStatus(maritalStatus));

    cout << "Gender: ";
    getline(cin, gender);
    checkForExit(gender, "C0");

    cout << "Number of Dependents: ";
    getline(cin, dependents);
    checkForExit(dependents, "C0");
}

// collectFinancialInfo 
void Applicant::collectFinancialInfo() {
    cout << "\n=== FINANCIAL INFORMATION ===" << endl;
    cout << "Type 'exit' at any time to save and leave application." << endl;

    // Annual Income
    do {
        cout << "Annual Income (PKR without commas): ";
        getline(cin, annualIncome);
        checkForExit(annualIncome, "C1");
        if (!isValidNumeric(annualIncome)) {
            cout << "Error: Income must contain only numbers without commas" << endl;
        }
    } while (!isValidNumeric(annualIncome));

    // Monthly Average Electricity Bill 
    do {
        cout << "Monthly Average Electricity Bill (PKR): ";
        getline(cin, avgElectricityBill);
        checkForExit(avgElectricityBill, "C1");
        if (!isValidNumeric(avgElectricityBill)) {
            cout << "Error: Bill amount must contain only numbers without commas" << endl;
        }
    } while (!isValidNumeric(avgElectricityBill));

    // Current Electricity Bill 
    do {
        cout << "Current Electricity Bill Amount (PKR): ";
        getline(cin, currentElectricityBill);
        checkForExit(currentElectricityBill, "C1");
        if (!isValidNumeric(currentElectricityBill)) {
            cout << "Error: Bill amount must contain only numbers without commas" << endl;
        }
    } while (!isValidNumeric(currentElectricityBill));

    // Existing Loans
    cout << "Do you have existing loans? (yes/no): ";
    string hasLoans;
    bool validLoanResponse = false;

    while (!validLoanResponse) {
        getline(cin, hasLoans);
        checkForExit(hasLoans, "C1");

        if (hasLoans == "yes") {
            validLoanResponse = true;
            string status, total, returned, due, bank, category;

            // Loan Status
            do {
                cout << "Loan Status:\n";
                cout << "1. Active\n2. Inactive\n";
                cout << "Enter choice (1-2): ";
                getline(cin, status);
                checkForExit(status, "C1");

                if (status == "1") status = "Active";
                else if (status == "2") status = "Inactive";
                else {
                    cout << "Error: Please enter 1 or 2" << endl;
                    continue;
                }

                if (!isValidLoanStatus(status)) {
                    cout << "Error: Invalid loan status selection" << endl;
                }
            } while (!isValidLoanStatus(status));

            // Total Loan Amount 
            do {
                cout << "Total Loan Amount: ";
                getline(cin, total);
                checkForExit(total, "C1");
                if (!isValidNumeric(total)) {
                    cout << "Error: Amount must contain only numbers without commas" << endl;
                }
            } while (!isValidNumeric(total));

            // Amount Returned
            do {
                cout << "Amount Returned: ";
                getline(cin, returned);
                checkForExit(returned, "C1");
                if (!isValidNumeric(returned)) {
                    cout << "Error: Amount must contain only numbers without commas" << endl;
                }
            } while (!isValidNumeric(returned));

            // Amount Still Due
            do {
                cout << "Amount Still Due: ";
                getline(cin, due);
                checkForExit(due, "C1");
                if (!isValidNumeric(due)) {
                    cout << "Error: Amount must contain only numbers without commas" << endl;
                }
            } while (!isValidNumeric(due));

            cout << "Bank Name: ";
            getline(cin, bank);
            checkForExit(bank, "C1");

            // Loan Category
            do {
                cout << "Loan Category:\n";
                cout << "1. Car\n2. Home\n3. Scooter\n";
                cout << "Enter choice (1-3): ";
                getline(cin, category);
                checkForExit(category, "C1");

                if (category == "1") category = "Car";
                else if (category == "2") category = "Home";
                else if (category == "3") category = "Scooter";
                else {
                    cout << "Error: Please enter a number between 1-3" << endl;
                    continue;
                }

                if (!isValidLoanCategory(category)) {
                    cout << "Error: Invalid loan category selection" << endl;
                }
            } while (!isValidLoanCategory(category));

            addExistingLoan(status, total, returned, due, bank, category);
        }
        else if (hasLoans == "no") {
            validLoanResponse = true;
            // Continue without adding loans
        }
        else {
            cout << "Invalid input. Please enter 'yes' or 'no': ";
        }
    }
}

// collectReferences 
void Applicant::collectReferences() {
    cout << "\n=== REFERENCES ===" << endl;
    cout << "Type 'exit' at any time to save and leave application." << endl;

    cout << "\n=== REFERENCE 1 ===" << endl;
    cout << "Name: ";
    getline(cin, referee1.name);
    checkForExit(referee1.name, "C2");

    // Referee 1 CNIC 
    do {
        cout << "CNIC (without dashes): ";
        getline(cin, referee1.cnic);
        checkForExit(referee1.cnic, "C2");
        if (!Parser::isValidCNIC(referee1.cnic)) {
            cout << "Error: CNIC must be 13 digits without dashes. Example: 1234567890123" << endl;
        }
    } while (!Parser::isValidCNIC(referee1.cnic));

    // Referee 1 Issue Date
    do {
        cout << "CNIC Issue Date (DD-MM-YYYY): ";
        getline(cin, referee1.issueDate);
        checkForExit(referee1.issueDate, "C2");
        if (!isValidDate(referee1.issueDate)) {
            cout << "Error: Date must be in DD-MM-YYYY format. Example: 15-08-2030" << endl;
        }
    } while (!isValidDate(referee1.issueDate));

    // Referee 1 Phone
    do {
        cout << "Phone Number: ";
        getline(cin, referee1.phone);
        checkForExit(referee1.phone, "C2");
        if (!isValidPhone(referee1.phone)) {
            cout << "Error: Phone number must be 11 digits. Example: 03001234567" << endl;
        }
    } while (!isValidPhone(referee1.phone));

    // Referee 1 Email
    do {
        cout << "Email: ";
        getline(cin, referee1.email);
        checkForExit(referee1.email, "C2");
        if (!isValidEmail(referee1.email)) {
            cout << "Error: Please enter a valid email address. Example: user@example.com" << endl;
        }
    } while (!isValidEmail(referee1.email));

    cout << "\n=== REFERENCE 2 ===" << endl;
    cout << "Name: ";
    getline(cin, referee2.name);
    checkForExit(referee2.name, "C2");

    // Referee 2 CNIC
    do {
        cout << "CNIC (without dashes): ";
        getline(cin, referee2.cnic);
        checkForExit(referee2.cnic, "C2");
        if (!Parser::isValidCNIC(referee2.cnic)) {
            cout << "Error: CNIC must be 13 digits without dashes. Example: 1234567890123" << endl;
        }
    } while (!Parser::isValidCNIC(referee2.cnic));

    // Referee 2 Issue Date
    do {
        cout << "CNIC Issue Date (DD-MM-YYYY): ";
        getline(cin, referee2.issueDate);
        checkForExit(referee2.issueDate, "C2");
        if (!isValidDate(referee2.issueDate)) {
            cout << "Error: Date must be in DD-MM-YYYY format. Example: 15-08-2030" << endl;
        }
    } while (!isValidDate(referee2.issueDate));

    // Referee 2 Phone
    do {
        cout << "Phone Number: ";
        getline(cin, referee2.phone);
        checkForExit(referee2.phone, "C2");
        if (!isValidPhone(referee2.phone)) {
            cout << "Error: Phone number must be 11 digits. Example: 03001234567" << endl;
        }
    } while (!isValidPhone(referee2.phone));

    // Referee 2 Email
    do {
        cout << "Email: ";
        getline(cin, referee2.email);
        checkForExit(referee2.email, "C2");
        if (!isValidEmail(referee2.email)) {
            cout << "Error: Please enter a valid email address. Example: user@example.com" << endl;
        }
    } while (!isValidEmail(referee2.email));
}

// collectDocuments 
void Applicant::collectDocuments() {
    handleImageUpload();
}

void Applicant::saveCheckpoint(const string& checkpointStatus) {
    status = checkpointStatus;
    if (Parser::saveApplicationToFile(toFileString())) {
        cout << "Progress saved successfully! Current status: " << status << endl;
    }
    else {
        cout << "Error saving progress." << endl;
    }
}

bool Applicant::loadApplication(const string& appId, const string& cnicInput) {
    return Parser::loadApplication(appId, cnicInput, *this);
}

void Applicant::showCurrentData() {
    cout << "\n=== CURRENT APPLICATION DATA ===" << endl;
    cout << "Application ID: " << applicationId << endl;
    cout << "Current Status: " << status << endl;

    if (status >= "C1") {
        cout << "Personal Info: " << fullName << ", " << cnic << endl;
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

// toFileString
string Applicant::toFileString() {
    string data = "";

    // Personal Information
    data += applicationId + "#" +
        (fullName.empty() ? "NOT_PROVIDED" : fullName) + "#" +
        (fathersName.empty() ? "NOT_PROVIDED" : fathersName) + "#" +
        (postalAddress.empty() ? "NOT_PROVIDED" : postalAddress) + "#" +
        (contactNumber.empty() ? "NOT_PROVIDED" : contactNumber) + "#" +
        (email.empty() ? "NOT_PROVIDED" : email) + "#" +
        (cnic.empty() ? "NOT_PROVIDED" : cnic) + "#" +
        (cnicExpiry.empty() ? "NOT_PROVIDED" : cnicExpiry) + "#" +
        (employmentStatus.empty() ? "NOT_PROVIDED" : employmentStatus) + "#" +
        (maritalStatus.empty() ? "NOT_PROVIDED" : maritalStatus) + "#" +
        (gender.empty() ? "NOT_PROVIDED" : gender) + "#" +
        (dependents.empty() ? "NOT_PROVIDED" : dependents) + "#";

    // Financial Information  
    data += (annualIncome.empty() ? "0" : annualIncome) + "#" +
        (avgElectricityBill.empty() ? "0" : avgElectricityBill) + "#" +
        (currentElectricityBill.empty() ? "0" : currentElectricityBill) + "#";

    // Existing Loans 
    if (existingLoans.empty()) {
        data += "NO_LOAN#NO_LOAN#NO_LOAN#NO_LOAN#NO_LOAN#NO_LOAN#";
    }
    else {
        data += existingLoans[0].loanStatus + "#" + existingLoans[0].totalAmount + "#" +
            existingLoans[0].amountReturned + "#" + existingLoans[0].amountDue + "#" +
            existingLoans[0].bankName + "#" + existingLoans[0].loanCategory + "#";
    }

    // References
    data += (referee1.name.empty() ? "NOT_PROVIDED" : referee1.name) + "#" +
        (referee1.cnic.empty() ? "NOT_PROVIDED" : referee1.cnic) + "#" +
        (referee1.issueDate.empty() ? "NOT_PROVIDED" : referee1.issueDate) + "#" +
        (referee1.phone.empty() ? "NOT_PROVIDED" : referee1.phone) + "#" +
        (referee1.email.empty() ? "NOT_PROVIDED" : referee1.email) + "#" +
        (referee2.name.empty() ? "NOT_PROVIDED" : referee2.name) + "#" +
        (referee2.cnic.empty() ? "NOT_PROVIDED" : referee2.cnic) + "#" +
        (referee2.issueDate.empty() ? "NOT_PROVIDED" : referee2.issueDate) + "#" +
        (referee2.phone.empty() ? "NOT_PROVIDED" : referee2.phone) + "#" +
        (referee2.email.empty() ? "NOT_PROVIDED" : referee2.email) + "#";

    // Image Paths (fields 27-30)
    data += (cnicFrontPath.empty() ? "NOT_UPLOADED" : cnicFrontPath) + "#" +
        (cnicBackPath.empty() ? "NOT_UPLOADED" : cnicBackPath) + "#" +
        (electricityBillPath.empty() ? "NOT_UPLOADED" : electricityBillPath) + "#" +
        (salarySlipPath.empty() ? "NOT_UPLOADED" : salarySlipPath) + "#";

    // Status (field 31)
    data += status;

    return data;
}

// addExistingLoan
void Applicant::addExistingLoan(string status, string total, string returned, string due, string bank, string category) {
    ExistingLoan loan(status, total, returned, due, bank, category);
    existingLoans.push_back(loan);
}

// handleImageUpload
void Applicant::handleImageUpload() {
    string imagePath;
    bool allUploaded = true;
    vector<string> failedUploads;

    cout << "\n=== IMAGE UPLOAD ===" << endl;
    cout << "Type 'exit' at any time to save and leave application." << endl;

    // CNIC Front
    cout << "Enter path for CNIC Front image: ";
    getline(cin, imagePath);
    checkForExit(imagePath, "C3");
    if (!imagePath.empty() && imagePath.front() == '"' && imagePath.back() == '"') {
        imagePath = imagePath.substr(1, imagePath.length() - 2);
    }
    if (copyFile(imagePath, "./temp_cnic_front.jpg")) {
        // Success
    }
    else {
        allUploaded = false;
        failedUploads.push_back("CNIC Front");
    }

    // CNIC Back
    cout << "Enter path for CNIC Back image: ";
    getline(cin, imagePath);
    checkForExit(imagePath, "C3");
    if (!imagePath.empty() && imagePath.front() == '"' && imagePath.back() == '"') {
        imagePath = imagePath.substr(1, imagePath.length() - 2);
    }
    if (copyFile(imagePath, "./temp_cnic_back.jpg")) {
        // Success
    }
    else {
        allUploaded = false;
        failedUploads.push_back("CNIC Back");
    }

    // Electricity Bill
    cout << "Enter path for Electricity Bill image: ";
    getline(cin, imagePath);
    checkForExit(imagePath, "C3");
    if (!imagePath.empty() && imagePath.front() == '"' && imagePath.back() == '"') {
        imagePath = imagePath.substr(1, imagePath.length() - 2);
    }
    if (copyFile(imagePath, "./temp_electricity_bill.jpg")) {
        // Success
    }
    else {
        allUploaded = false;
        failedUploads.push_back("Electricity Bill");
    }

    // Salary Slip
    cout << "Enter path for Salary Slip/Bank Statement image: ";
    getline(cin, imagePath);
    checkForExit(imagePath, "C3");
    if (!imagePath.empty() && imagePath.front() == '"' && imagePath.back() == '"') {
        imagePath = imagePath.substr(1, imagePath.length() - 2);
    }
    if (copyFile(imagePath, "./temp_salary_slip.jpg")) {
        // Success
    }
    else {
        allUploaded = false;
        failedUploads.push_back("Salary Slip");
    }

    // Only create folder and move files if ALL uploads were successful
    if (allUploaded) {
        // Create the folder
        string folderPath = "./data/" + applicationId;
#ifdef _WIN32
        system(("mkdir \"" + folderPath + "\"").c_str());
#else
        system(("mkdir -p \"" + folderPath + "\"").c_str());
#endif

        // Move temp files to final location
        cnicFrontPath = folderPath + "/cnic_front.jpg";
        cnicBackPath = folderPath + "/cnic_back.jpg";
        electricityBillPath = folderPath + "/electricity_bill.jpg";
        salarySlipPath = folderPath + "/salary_slip.jpg";

        copyFile("./temp_cnic_front.jpg", cnicFrontPath);
        copyFile("./temp_cnic_back.jpg", cnicBackPath);
        copyFile("./temp_electricity_bill.jpg", electricityBillPath);
        copyFile("./temp_salary_slip.jpg", salarySlipPath);

        // Clean up temp files
        remove("./temp_cnic_front.jpg");
        remove("./temp_cnic_back.jpg");
        remove("./temp_electricity_bill.jpg");
        remove("./temp_salary_slip.jpg");

        cout << "All images uploaded successfully!" << endl;
    }
    else {
        // Set paths to NOT_UPLOADED
        cnicFrontPath = "NOT_UPLOADED";
        cnicBackPath = "NOT_UPLOADED";
        electricityBillPath = "NOT_UPLOADED";
        salarySlipPath = "NOT_UPLOADED";

        // Clean up any temp files that were created
        remove("./temp_cnic_front.jpg");
        remove("./temp_cnic_back.jpg");
        remove("./temp_electricity_bill.jpg");
        remove("./temp_salary_slip.jpg");

        cout << "\nIMAGE UPLOAD FAILED!" << endl;
        cout << "The following documents were not uploaded correctly:" << endl;
        for (const auto& doc : failedUploads) {
            cout << "  - " << doc << endl;
        }
        cout << "\nYou can complete document upload when you resume the application." << endl;
    }
}

// copyFile - HELPER FUNCTION
bool Applicant::copyFile(const string& sourcePath, const string& destPath) {
    ifstream source(sourcePath, ios::binary);
    ofstream dest(destPath, ios::binary);

    if (!source) {
        cout << "Error: Cannot open source file: " << sourcePath << endl;
        return false;
    }
    if (!dest) {
        cout << "Error: Cannot create destination file: " << destPath << endl;
        return false;
    }

    dest << source.rdbuf();

    source.close();
    dest.close();
    return true;
}

// Helper function to check for exit command
void Applicant::checkForExit(const string& input, const string& currentStatus) {
    if (input == "exit" || input == "quit") {
        if (!isExiting) {
            isExiting = true;
            saveCheckpoint(currentStatus);
            cout << "You can resume later with AppID: " << applicationId << endl;
            cout << "Returning to main menu..." << endl;
            throw ReturnToMainMenuException();
        }
    }
}
// ============================
// INPUT VALIDATION FUNCTIONS 
// ============================

// Helper function to check if string contains only digits
bool containsOnlyDigits(const string& str) {
    for (char c : str) {
        if (!isdigit(c)) return false;
    }
    return true;
}

// Helper function to check email format
bool isValidEmail(const string& email) {
    bool hasAt = false;
    bool hasDot = false;

    for (char c : email) {
        if (c == '@') hasAt = true;
        if (c == '.') hasDot = true;
    }

    return hasAt && hasDot;
}

// Helper function to validate date format DD-MM-YYYY
bool isValidDate(const string& date) {
    if (date.length() != 10) return false;
    if (date[2] != '-' || date[5] != '-') return false;

    string day = date.substr(0, 2);
    string month = date.substr(3, 2);
    string year = date.substr(6, 4);

    return containsOnlyDigits(day) && containsOnlyDigits(month) && containsOnlyDigits(year);
}

// Phone number validation - 11 digits
bool isValidPhone(const string& phone) {
    return phone.length() == 11 && containsOnlyDigits(phone);
}

// Numeric validation for income and bills
bool isValidNumeric(const string& num) {
    return !num.empty() && containsOnlyDigits(num);
}

// Employment status validation - CASE INSENSITIVE
bool isValidEmploymentStatus(const string& status) {
    string lowerStatus = status;
    for (char& c : lowerStatus) c = tolower(c);
    return lowerStatus == "self-employed" || lowerStatus == "salaried" ||
        lowerStatus == "retired" || lowerStatus == "unemployed";
}

// Marital status validation - CASE INSENSITIVE  
bool isValidMaritalStatus(const string& status) {
    string lowerStatus = status;
    for (char& c : lowerStatus) c = tolower(c);
    return lowerStatus == "single" || lowerStatus == "married" ||
        lowerStatus == "divorced" || lowerStatus == "widowed";
}

// Loan status validation
bool isValidLoanStatus(const string& status) {
    string lowerStatus = status;
    transform(lowerStatus.begin(), lowerStatus.end(), lowerStatus.begin(), ::tolower);
    return lowerStatus == "active" || lowerStatus == "inactive";
}

// Loan category validation
bool isValidLoanCategory(const string& category) {
    string lowerCategory = category;
    transform(lowerCategory.begin(), lowerCategory.end(), lowerCategory.begin(), ::tolower);
    return lowerCategory == "car" || lowerCategory == "home" || lowerCategory == "scooter";
}

bool isValidName(const string& name) {
    if (name.empty()) return false;
    for (char c : name) {
        if (!isalpha(c) && c != ' ') return false;
    }
    return true;
}

// Postal code validation - 5 digits
bool isValidPostalCode(const string& postalCode) {
    return postalCode.length() == 5 && containsOnlyDigits(postalCode);
}

//Checkpoint Validations
bool Applicant::validatePersonalInfo() {
    return !fullName.empty() && !fathersName.empty() && !postalAddress.empty() &&
        !contactNumber.empty() && !email.empty() && !cnic.empty() &&
        !cnicExpiry.empty() && !employmentStatus.empty() && !maritalStatus.empty() &&
        !gender.empty() && !dependents.empty();
}

bool Applicant::validateFinancialInfo() {
    return !annualIncome.empty() && !avgElectricityBill.empty() && !currentElectricityBill.empty();
}

bool Applicant::validateReferences() {
    return !referee1.name.empty() && !referee1.cnic.empty() && !referee1.issueDate.empty() &&
        !referee1.phone.empty() && !referee1.email.empty() &&
        !referee2.name.empty() && !referee2.cnic.empty() && !referee2.issueDate.empty() &&
        !referee2.phone.empty() && !referee2.email.empty();
}