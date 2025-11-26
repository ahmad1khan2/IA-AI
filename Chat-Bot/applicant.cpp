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
    status = "submitted";
}

// collectData 
void Applicant::collectData() {
    cout << "\n=== LOAN APPLICATION FORM ===" << endl;

    // Personal Information
   

    // Full Name 
    do {
        cout << "Full Name: ";
        getline(cin, fullName);
        if (!isValidName(fullName)) {
            cout << "Error: Name must contain only alphabets and spaces" << endl;
        }
    } while (!isValidName(fullName));

    // Father's Name 
    do {
        cout << "Father's Name: ";
        getline(cin, fathersName);
        if (!isValidName(fathersName)) {
            cout << "Error: Name must contain only alphabets and spaces" << endl;
        }
    } while (!isValidName(fathersName));

    // Postal Address 
    do {
        cout << "Postal Address: ";
        getline(cin, postalAddress);
        if (!isValidPostalCode(postalAddress)) {
            cout << "Error: Postal code must be 5 digits" << endl;
        }
    } while (!isValidPostalCode(postalAddress));

    // Contact Number 
    do {
        cout << "Contact Number: ";
        getline(cin, contactNumber);
        if (!isValidPhone(contactNumber)) {
            cout << "Error: Phone number must be 11 digits. Example: 03001234567" << endl;
        }
    } while (!isValidPhone(contactNumber));

    // Email 
    do {
        cout << "Email Address: ";
        getline(cin, email);
        if (!isValidEmail(email)) {
            cout << "Error: Please enter a valid email address. Example: user@example.com" << endl;
        }
    } while (!isValidEmail(email));

    // CNIC 
    do {
        cout << "CNIC (without dashes): ";
        getline(cin, cnic);
        if (!Parser::isValidCNIC(cnic)) {  
            cout << "Error: CNIC must be 13 digits without dashes. Example: 1234567890123" << endl;
        }
    } while (!Parser::isValidCNIC(cnic));  


    // CNIC Expiry
    do {
        cout << "CNIC Expiry Date (DD-MM-YYYY): ";
        getline(cin, cnicExpiry);
        if (!isValidDate(cnicExpiry)) {
            cout << "Error: Date must be in DD-MM-YYYY format. Example: 15-08-2030" << endl;
        }
    } while (!isValidDate(cnicExpiry));

    // Employment Status
    do {
        cout << "Employment Status (Self-employed/salaried/retired/unemployed): ";
        getline(cin, employmentStatus);
        if (!isValidEmploymentStatus(employmentStatus)) {
            cout << "Error: Please enter: Self-employed, salaried, retired, or unemployed" << endl;
        }
    } while (!isValidEmploymentStatus(employmentStatus));

    // Marital Status
    do {
        cout << "Marital Status (Single/Married/Divorced/Widowed): ";
        getline(cin, maritalStatus);
        if (!isValidMaritalStatus(maritalStatus)) {
            cout << "Error: Please enter: Single, Married, Divorced, or Widowed" << endl;
        }
    } while (!isValidMaritalStatus(maritalStatus));

    cout << "Gender: ";
    getline(cin, gender);

    cout << "Number of Dependents: ";
    getline(cin, dependents);

    // Annual Income
    do {
        cout << "Annual Income (PKR without commas): ";
        getline(cin, annualIncome);
        if (!isValidNumeric(annualIncome)) {
            cout << "Error: Income must contain only numbers without commas" << endl;
        }
    } while (!isValidNumeric(annualIncome));

    // Monthly Average Electricity Bill 
    do {
        cout << "Monthly Average Electricity Bill (PKR): ";
        getline(cin, avgElectricityBill);
        if (!isValidNumeric(avgElectricityBill)) {
            cout << "Error: Bill amount must contain only numbers without commas" << endl;
        }
    } while (!isValidNumeric(avgElectricityBill));

    // Current Electricity Bill 
    do {
        cout << "Current Electricity Bill Amount (PKR): ";
        getline(cin, currentElectricityBill);
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

        if (hasLoans == "yes") {
            validLoanResponse = true;
            string status, total, returned, due, bank, category;

            // Loan Status
            do {
                cout << "Loan Status (Active/Inactive): ";
                getline(cin, status);
                if (!isValidLoanStatus(status)) {
                    cout << "Error: Please enter: Active or Inactive" << endl;
                }
            } while (!isValidLoanStatus(status));

            // Total Loan Amount 
            do {
                cout << "Total Loan Amount: ";
                getline(cin, total);
                if (!isValidNumeric(total)) {
                    cout << "Error: Amount must contain only numbers without commas" << endl;
                }
            } while (!isValidNumeric(total));

            // Amount Returned
            do {
                cout << "Amount Returned: ";
                getline(cin, returned);
                if (!isValidNumeric(returned)) {
                    cout << "Error: Amount must contain only numbers without commas" << endl;
                }
            } while (!isValidNumeric(returned));

            // Amount Still Due
            do {
                cout << "Amount Still Due: ";
                getline(cin, due);
                if (!isValidNumeric(due)) {
                    cout << "Error: Amount must contain only numbers without commas" << endl;
                }
            } while (!isValidNumeric(due));

            cout << "Bank Name: ";
            getline(cin, bank);

            // Loan Category
            do {
                cout << "Loan Category (Car/Home/Scooter): ";
                getline(cin, category);
                if (!isValidLoanCategory(category)) {
                    cout << "Error: Please enter: Car, Home, or Scooter" << endl;
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

    // References
    cout << "\n=== REFERENCE 1 ===" << endl;
    cout << "Name: ";
    getline(cin, referee1.name);

    // Referee 1 CNIC 
    do {
        cout << "CNIC (without dashes): ";
        getline(cin, cnic);
        if (!Parser::isValidCNIC(cnic)) {
            cout << "Error: CNIC must be 13 digits without dashes. Example: 1234567890123" << endl;
        }
    } while (!Parser::isValidCNIC(cnic));


    // Referee 1 Issue Date
    do {
        cout << "CNIC Issue Date (DD-MM-YYYY): ";
        getline(cin, referee1.issueDate);
        if (!isValidDate(referee1.issueDate)) {
            cout << "Error: Date must be in DD-MM-YYYY format. Example: 15-08-2030" << endl;
        }
    } while (!isValidDate(referee1.issueDate));

    // Referee 1 Phone
    do {
        cout << "Phone Number: ";
        getline(cin, referee1.phone);
        if (!isValidPhone(referee1.phone)) {
            cout << "Error: Phone number must be 11 digits. Example: 03001234567" << endl;
        }
    } while (!isValidPhone(referee1.phone));

    // Referee 1 Email
    do {
        cout << "Email: ";
        getline(cin, referee1.email);
        if (!isValidEmail(referee1.email)) {
            cout << "Error: Please enter a valid email address. Example: user@example.com" << endl;
        }
    } while (!isValidEmail(referee1.email));

    cout << "\n=== REFERENCE 2 ===" << endl;
    cout << "Name: ";
    getline(cin, referee2.name);

    // Referee 2 CNIC
    do {
        cout << "CNIC (without dashes): ";
        getline(cin, cnic);
        if (!Parser::isValidCNIC(cnic)) {
            cout << "Error: CNIC must be 13 digits without dashes. Example: 1234567890123" << endl;
        }
    } while (!Parser::isValidCNIC(cnic));

    // Referee 2 Issue Date
    do {
        cout << "CNIC Issue Date (DD-MM-YYYY): ";
        getline(cin, referee2.issueDate);
        if (!isValidDate(referee2.issueDate)) {
            cout << "Error: Date must be in DD-MM-YYYY format. Example: 15-08-2030" << endl;
        }
    } while (!isValidDate(referee2.issueDate));

    // Referee 2 Phone
    do {
        cout << "Phone Number: ";
        getline(cin, referee2.phone);
        if (!isValidPhone(referee2.phone)) {
            cout << "Error: Phone number must be 11 digits. Example: 03001234567" << endl;
        }
    } while (!isValidPhone(referee2.phone));

    // Referee 2 Email
    do {
        cout << "Email: ";
        getline(cin, referee2.email);
        if (!isValidEmail(referee2.email)) {
            cout << "Error: Please enter a valid email address. Example: user@example.com" << endl;
        }
    } while (!isValidEmail(referee2.email));

    //Confirmation and file saving
    showApplicationSummary();

    cout << "\nDo you want to submit this application? (yes/no): ";
    string confirm;
    bool validConfirm = false;

    while (!validConfirm) {
        getline(cin, confirm);

        if (confirm == "yes") {
            validConfirm = true;
            applicationId = Parser::generateApplicationId();  // Changed to use Parser
            handleImageUpload();

            // Check if ALL images were uploaded successfully before saving
            bool allImagesUploaded = (cnicFrontPath != "NOT_UPLOADED" &&
                cnicBackPath != "NOT_UPLOADED" &&
                electricityBillPath != "NOT_UPLOADED" &&
                salarySlipPath != "NOT_UPLOADED");

            if (allImagesUploaded) {
                if (Parser::saveApplicationToFile(toFileString())) {  // Changed to use Parser
                    cout << "Application submitted successfully! Your Application ID: " << applicationId << endl;
                }
                else {
                    cout << "Error saving application. Please try again." << endl;
                }
            }
            else {
                // Reset application ID since it wasn't successfully submitted
                applicationId = "";
            }
        }
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
    // Image Paths
    data += cnicFrontPath + "#" + cnicBackPath + "#" +
        electricityBillPath + "#" + salarySlipPath + "#";

    // Status
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

    // Track upload results first without creating folder
    string cnicFrontResult, cnicBackResult, electricityBillResult, salarySlipResult;

    // CNIC Front
    cout << "Enter path for CNIC Front image: ";
    getline(cin, imagePath);
    if (!imagePath.empty() && imagePath.front() == '"' && imagePath.back() == '"') {
        imagePath = imagePath.substr(1, imagePath.length() - 2);
    }
    if (copyFile(imagePath, "./temp_cnic_front.jpg")) {
        cnicFrontResult = "UPLOADED";
    }
    else {
        cnicFrontResult = "NOT_UPLOADED";
        allUploaded = false;
        failedUploads.push_back("CNIC Front");
    }

    // CNIC Back
    cout << "Enter path for CNIC Back image: ";
    getline(cin, imagePath);
    if (!imagePath.empty() && imagePath.front() == '"' && imagePath.back() == '"') {
        imagePath = imagePath.substr(1, imagePath.length() - 2);
    }
    if (copyFile(imagePath, "./temp_cnic_back.jpg")) {
        cnicBackResult = "UPLOADED";
    }
    else {
        cnicBackResult = "NOT_UPLOADED";
        allUploaded = false;
        failedUploads.push_back("CNIC Back");
    }

    // Electricity Bill
    cout << "Enter path for Electricity Bill image: ";
    getline(cin, imagePath);
    if (!imagePath.empty() && imagePath.front() == '"' && imagePath.back() == '"') {
        imagePath = imagePath.substr(1, imagePath.length() - 2);
    }
    if (copyFile(imagePath, "./temp_electricity_bill.jpg")) {
        electricityBillResult = "UPLOADED";
    }
    else {
        electricityBillResult = "NOT_UPLOADED";
        allUploaded = false;
        failedUploads.push_back("Electricity Bill");
    }

    // Salary Slip
    cout << "Enter path for Salary Slip/Bank Statement image: ";
    getline(cin, imagePath);
    if (!imagePath.empty() && imagePath.front() == '"' && imagePath.back() == '"') {
        imagePath = imagePath.substr(1, imagePath.length() - 2);
    }
    if (copyFile(imagePath, "./temp_salary_slip.jpg")) {
        salarySlipResult = "UPLOADED";
    }
    else {
        salarySlipResult = "NOT_UPLOADED";
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

        cout << "All images uploaded successfully! Application submitted." << endl;
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
        cout << "\nPlease submit your application again or return to main menu." << endl;
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
bool isValidLoanStatus(const string & status) {
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
