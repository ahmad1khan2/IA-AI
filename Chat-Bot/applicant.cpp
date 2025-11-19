//#include "applicant.h"
//#include <fstream>
//#include <iostream>
//#include <string>  
//
//using namespace std;
//
//// ExistingLoan constructor 
//ExistingLoan::ExistingLoan(string status, string total, string returned,
//    string due, string bank, string category) {
//    loanStatus = status;
//    totalAmount = total;
//    amountReturned = returned;
//    amountDue = due;
//    bankName = bank;
//    loanCategory = category;
//}
//
//// Referee constructor 
//Referee::Referee(string n, string c, string date, string p, string e) {
//    name = n;
//    cnic = c;
//    issueDate = date;
//    phone = p;
//    email = e;
//}
//
//// Applicant constructor 
//Applicant::Applicant() {
//    status = "submitted";
//}
//
//// collectData 
//void Applicant::collectData() {
//    cout << "\n=== LOAN APPLICATION FORM ===" << endl;
//
//    // Personal Information
//   
//
//    // Full Name 
//    do {
//        cout << "Full Name: ";
//        getline(cin, fullName);
//        if (!isValidName(fullName)) {
//            cout << "Error: Name must contain only alphabets and spaces" << endl;
//        }
//    } while (!isValidName(fullName));
//
//    // Father's Name 
//    do {
//        cout << "Father's Name: ";
//        getline(cin, fathersName);
//        if (!isValidName(fathersName)) {
//            cout << "Error: Name must contain only alphabets and spaces" << endl;
//        }
//    } while (!isValidName(fathersName));
//
//    // Postal Address 
//    do {
//        cout << "Postal Address: ";
//        getline(cin, postalAddress);
//        if (!isValidPostalCode(postalAddress)) {
//            cout << "Error: Postal code must be 5 digits" << endl;
//        }
//    } while (!isValidPostalCode(postalAddress));
//
//    // Contact Number 
//    do {
//        cout << "Contact Number: ";
//        getline(cin, contactNumber);
//        if (!isValidPhone(contactNumber)) {
//            cout << "Error: Phone number must be 11 digits. Example: 03001234567" << endl;
//        }
//    } while (!isValidPhone(contactNumber));
//
//    // Email 
//    do {
//        cout << "Email Address: ";
//        getline(cin, email);
//        if (!isValidEmail(email)) {
//            cout << "Error: Please enter a valid email address. Example: user@example.com" << endl;
//        }
//    } while (!isValidEmail(email));
//
//    // CNIC 
//    do {
//        cout << "CNIC (without dashes): ";
//        getline(cin, cnic);
//        if (!isValidCNIC(cnic)) {
//            cout << "Error: CNIC must be 13 digits without dashes. Example: 1234567890123" << endl;
//        }
//    } while (!isValidCNIC(cnic));
//
//    // CNIC Expiry
//    do {
//        cout << "CNIC Expiry Date (DD-MM-YYYY): ";
//        getline(cin, cnicExpiry);
//        if (!isValidDate(cnicExpiry)) {
//            cout << "Error: Date must be in DD-MM-YYYY format. Example: 15-08-2030" << endl;
//        }
//    } while (!isValidDate(cnicExpiry));
//
//    // Employment Status
//    do {
//        cout << "Employment Status (Self-employed/salaried/retired/unemployed): ";
//        getline(cin, employmentStatus);
//        if (!isValidEmploymentStatus(employmentStatus)) {
//            cout << "Error: Please enter: Self-employed, salaried, retired, or unemployed" << endl;
//        }
//    } while (!isValidEmploymentStatus(employmentStatus));
//
//    // Marital Status
//    do {
//        cout << "Marital Status (Single/Married/Divorced/Widowed): ";
//        getline(cin, maritalStatus);
//        if (!isValidMaritalStatus(maritalStatus)) {
//            cout << "Error: Please enter: Single, Married, Divorced, or Widowed" << endl;
//        }
//    } while (!isValidMaritalStatus(maritalStatus));
//
//    cout << "Gender: ";
//    getline(cin, gender);
//
//    cout << "Number of Dependents: ";
//    getline(cin, dependents);
//
//    // Annual Income
//    do {
//        cout << "Annual Income (PKR without commas): ";
//        getline(cin, annualIncome);
//        if (!isValidNumeric(annualIncome)) {
//            cout << "Error: Income must contain only numbers without commas" << endl;
//        }
//    } while (!isValidNumeric(annualIncome));
//
//    // Monthly Average Electricity Bill 
//    do {
//        cout << "Monthly Average Electricity Bill (PKR): ";
//        getline(cin, avgElectricityBill);
//        if (!isValidNumeric(avgElectricityBill)) {
//            cout << "Error: Bill amount must contain only numbers without commas" << endl;
//        }
//    } while (!isValidNumeric(avgElectricityBill));
//
//    // Current Electricity Bill 
//    do {
//        cout << "Current Electricity Bill Amount (PKR): ";
//        getline(cin, currentElectricityBill);
//        if (!isValidNumeric(currentElectricityBill)) {
//            cout << "Error: Bill amount must contain only numbers without commas" << endl;
//        }
//    } while (!isValidNumeric(currentElectricityBill));
//
//    // Existing Loans
//    cout << "Do you have existing loans? (yes/no): ";
//    string hasLoans;
//    getline(cin, hasLoans);
//
//    if (hasLoans == "yes") {
//        string status, total, returned, due, bank, category;
//
//        // Loan Status
//        do {
//            cout << "Loan Status (Active/Inactive): ";
//            getline(cin, status);
//            if (!isValidLoanStatus(status)) {
//                cout << "Error: Please enter: Active or Inactive" << endl;
//            }
//        } while (!isValidLoanStatus(status));
//
//        // Total Loan Amount 
//        do {
//            cout << "Total Loan Amount: ";
//            getline(cin, total);
//            if (!isValidNumeric(total)) {
//                cout << "Error: Amount must contain only numbers without commas" << endl;
//            }
//        } while (!isValidNumeric(total));
//
//        // Amount Returned
//        do {
//            cout << "Amount Returned: ";
//            getline(cin, returned);
//            if (!isValidNumeric(returned)) {
//                cout << "Error: Amount must contain only numbers without commas" << endl;
//            }
//        } while (!isValidNumeric(returned));
//
//        // Amount Still Due
//        do {
//            cout << "Amount Still Due: ";
//            getline(cin, due);
//            if (!isValidNumeric(due)) {
//                cout << "Error: Amount must contain only numbers without commas" << endl;
//            }
//        } while (!isValidNumeric(due));
//
//        cout << "Bank Name: ";
//        getline(cin, bank);
//
//        // Loan Category
//        do {
//            cout << "Loan Category (Car/Home/Bike): ";
//            getline(cin, category);
//            if (!isValidLoanCategory(category)) {
//                cout << "Error: Please enter: Car, Home, or Bike" << endl;
//            }
//        } while (!isValidLoanCategory(category));
//
//        addExistingLoan(status, total, returned, due, bank, category);
//    }
//
//    // References
//    cout << "\n=== REFERENCE 1 ===" << endl;
//    cout << "Name: ";
//    getline(cin, referee1.name);
//
//    // Referee 1 CNIC 
//    do {
//        cout << "CNIC: ";
//        getline(cin, referee1.cnic);
//        if (!isValidCNIC(referee1.cnic)) {
//            cout << "Error: CNIC must be 13 digits without dashes. Example: 1234567890123" << endl;
//        }
//    } while (!isValidCNIC(referee1.cnic));
//
//    // Referee 1 Issue Date
//    do {
//        cout << "CNIC Issue Date (DD-MM-YYYY): ";
//        getline(cin, referee1.issueDate);
//        if (!isValidDate(referee1.issueDate)) {
//            cout << "Error: Date must be in DD-MM-YYYY format. Example: 15-08-2030" << endl;
//        }
//    } while (!isValidDate(referee1.issueDate));
//
//    // Referee 1 Phone
//    do {
//        cout << "Phone Number: ";
//        getline(cin, referee1.phone);
//        if (!isValidPhone(referee1.phone)) {
//            cout << "Error: Phone number must be 11 digits. Example: 03001234567" << endl;
//        }
//    } while (!isValidPhone(referee1.phone));
//
//    // Referee 1 Email
//    do {
//        cout << "Email: ";
//        getline(cin, referee1.email);
//        if (!isValidEmail(referee1.email)) {
//            cout << "Error: Please enter a valid email address. Example: user@example.com" << endl;
//        }
//    } while (!isValidEmail(referee1.email));
//
//    cout << "\n=== REFERENCE 2 ===" << endl;
//    cout << "Name: ";
//    getline(cin, referee2.name);
//
//    // Referee 2 CNIC
//    do {
//        cout << "CNIC: ";
//        getline(cin, referee2.cnic);
//        if (!isValidCNIC(referee2.cnic)) {
//            cout << "Error: CNIC must be 13 digits without dashes. Example: 1234567890123" << endl;
//        }
//    } while (!isValidCNIC(referee2.cnic));
//
//    // Referee 2 Issue Date
//    do {
//        cout << "CNIC Issue Date (DD-MM-YYYY): ";
//        getline(cin, referee2.issueDate);
//        if (!isValidDate(referee2.issueDate)) {
//            cout << "Error: Date must be in DD-MM-YYYY format. Example: 15-08-2030" << endl;
//        }
//    } while (!isValidDate(referee2.issueDate));
//
//    // Referee 2 Phone
//    do {
//        cout << "Phone Number: ";
//        getline(cin, referee2.phone);
//        if (!isValidPhone(referee2.phone)) {
//            cout << "Error: Phone number must be 11 digits. Example: 03001234567" << endl;
//        }
//    } while (!isValidPhone(referee2.phone));
//
//    // Referee 2 Email
//    do {
//        cout << "Email: ";
//        getline(cin, referee2.email);
//        if (!isValidEmail(referee2.email)) {
//            cout << "Error: Please enter a valid email address. Example: user@example.com" << endl;
//        }
//    } while (!isValidEmail(referee2.email));
//
//    //Confirmation and file saving
//    showApplicationSummary();
//
//    cout << "\nDo you want to submit this application? (yes/no): ";
//    string confirm;
//    getline(cin, confirm);
//
//    if (confirm == "yes") {
//        applicationId = generateApplicationId();
//        handleImageUpload();
//        if (saveToFile()) {
//            cout << "Application submitted successfully! Your Application ID: " << applicationId << endl;
//        }
//        else {
//            cout << "Error saving application. Please try again." << endl;
//        }
//    }
//    else {
//        cout << "Application cancelled. Returning to main menu." << endl;
//    }
//}
//
//// showApplicationSummary 
//void Applicant::showApplicationSummary() {
//    cout << "\n=== APPLICATION SUMMARY ===" << endl;
//    cout << "Name: " << fullName << endl;
//    cout << "CNIC: " << cnic << endl;
//    cout << "Email: " << email << endl;
//    cout << "Annual Income: " << annualIncome << endl;
//}
//
//// generateApplicationId
//string Applicant::generateApplicationId() {
//    static int counter = 1001;
//    string id = to_string(counter);
//    counter++;
//    return id;
//}
//
//// saveToFile
//bool Applicant::saveToFile() {
//    ofstream file("applications.txt", ios::app);
//    if (!file) {
//        file.open("applications.txt");
//        if (!file) {
//            return false;
//        }
//    }
//
//    string fileData = toFileString();
//    file << fileData << endl;
//    file.close();
//    return true;
//}
//
//// toFileString
//string Applicant::toFileString() {
//    string data = "";
//
//    // Personal Information
//    data += applicationId + "#" + fullName + "#" + fathersName + "#" +
//        postalAddress + "#" + contactNumber + "#" + email + "#" +
//        cnic + "#" + cnicExpiry + "#" + employmentStatus + "#" +
//        maritalStatus + "#" + gender + "#" + dependents + "#";
//
//    // Financial Information  
//    data += annualIncome + "#" + avgElectricityBill + "#" + currentElectricityBill + "#";
//
//    // Existing Loans 
//    if (existingLoans.empty()) {
//        data += "No#No#No#No#No#No#";  //"No" for all loan fields
//    }
//    else {
//        data += existingLoans[0].loanStatus + "#" + existingLoans[0].totalAmount + "#" +
//            existingLoans[0].amountReturned + "#" + existingLoans[0].amountDue + "#" +
//            existingLoans[0].bankName + "#" + existingLoans[0].loanCategory + "#";
//    }
//
//    // References
//    data += referee1.name + "#" + referee1.cnic + "#" + referee1.issueDate + "#" +
//        referee1.phone + "#" + referee1.email + "#" +
//        referee2.name + "#" + referee2.cnic + "#" + referee2.issueDate + "#" +
//        referee2.phone + "#" + referee2.email + "#";
//    // Image Paths
//    data += cnicFrontPath + "#" + cnicBackPath + "#" +
//        electricityBillPath + "#" + salarySlipPath + "#";
//
//    // Status
//    data += status;
//
//    return data;
//}
//
//// addExistingLoan
//void Applicant::addExistingLoan(string status, string total, string returned, string due, string bank, string category) {
//    ExistingLoan loan(status, total, returned, due, bank, category);
//    existingLoans.push_back(loan);
//}
//// handleImageUpload
//void Applicant::handleImageUpload() {
//    string imagePath;
//
//    cout << "\n=== IMAGE UPLOAD ===" << endl;
//
//    // Create ./data/xxxx folder using application ID (FIXED)
//    string folderPath = "./data/" + applicationId;
//#ifdef _WIN32  //windows format folder creation
//    system(("mkdir \"" + folderPath + "\"").c_str());
//#else //Linux/mac format folder creation
//    system(("mkdir -p \"" + folderPath + "\"").c_str());
//#endif
//
//    // CNIC Front
//    cout << "Enter path for CNIC Front image: ";
//    getline(cin, imagePath);
//    // Remove quotes if user entered them
//    if (!imagePath.empty() && imagePath.front() == '"' && imagePath.back() == '"') {
//        imagePath = imagePath.substr(1, imagePath.length() - 2);
//    }
//    cnicFrontPath = folderPath + "/cnic_front.jpg";
//    if (!copyFile(imagePath, cnicFrontPath)) {
//        cnicFrontPath = "NOT_UPLOADED";
//    }
//
//    // CNIC Back
//    cout << "Enter path for CNIC Back image: ";
//    getline(cin, imagePath);
//    if (!imagePath.empty() && imagePath.front() == '"' && imagePath.back() == '"') {
//        imagePath = imagePath.substr(1, imagePath.length() - 2);
//    }
//    cnicBackPath = folderPath + "/cnic_back.jpg";
//    if (!copyFile(imagePath, cnicBackPath)) {
//        cnicBackPath = "NOT_UPLOADED";
//    }
//
//    // Electricity Bill
//    cout << "Enter path for Electricity Bill image: ";
//    getline(cin, imagePath);
//    if (!imagePath.empty() && imagePath.front() == '"' && imagePath.back() == '"') {
//        imagePath = imagePath.substr(1, imagePath.length() - 2);
//    }
//    electricityBillPath = folderPath + "/electricity_bill.jpg";
//    if (!copyFile(imagePath, electricityBillPath)) {
//        electricityBillPath = "NOT_UPLOADED";
//    }
//
//    // Salary Slip
//    cout << "Enter path for Salary Slip/Bank Statement image: ";
//    getline(cin, imagePath);
//    if (!imagePath.empty() && imagePath.front() == '"' && imagePath.back() == '"') {
//        imagePath = imagePath.substr(1, imagePath.length() - 2);
//    }
//    salarySlipPath = folderPath + "/salary_slip.jpg";
//    if (!copyFile(imagePath, salarySlipPath)) {
//        salarySlipPath = "NOT_UPLOADED";
//    }
//
//    cout << "Image upload completed!" << endl;
//}
//// copyFile - HELPER FUNCTION
//bool Applicant::copyFile(const string& sourcePath, const string& destPath) {
//    ifstream source(sourcePath, ios::binary);
//    ofstream dest(destPath, ios::binary);
//
//    if (!source) {
//        cout << "Error: Cannot open source file: " << sourcePath << endl;
//        return false;
//    }
//    if (!dest) {
//        cout << "Error: Cannot create destination file: " << destPath << endl;
//        return false;
//    }
//
//    dest << source.rdbuf();
//
//    source.close();
//    dest.close();
//    return true;
//}
//
//// ============================
//// INPUT VALIDATION FUNCTIONS 
//// ============================
//
//// Helper function to check if string contains only digits
//bool containsOnlyDigits(const string& str) {
//    for (char c : str) {
//        if (!isdigit(c)) return false;
//    }
//    return true;
//}
//
//// Helper function to check email format
//bool isValidEmail(const string& email) {
//    bool hasAt = false;
//    bool hasDot = false;
//
//    for (char c : email) {
//        if (c == '@') hasAt = true;
//        if (c == '.') hasDot = true;
//    }
//
//    return hasAt && hasDot;
//}
//
//// Helper function to validate date format DD-MM-YYYY
//bool isValidDate(const string& date) {
//    if (date.length() != 10) return false;
//    if (date[2] != '-' || date[5] != '-') return false;
//
//    string day = date.substr(0, 2);
//    string month = date.substr(3, 2);
//    string year = date.substr(6, 4);
//
//    return containsOnlyDigits(day) && containsOnlyDigits(month) && containsOnlyDigits(year);
//}
//
//// CNIC validation - 13 digits without dashes
//bool isValidCNIC(const string& cnic) {
//    return cnic.length() == 13 && containsOnlyDigits(cnic);
//}
//
//// Phone number validation - 11 digits
//bool isValidPhone(const string& phone) {
//    return phone.length() == 11 && containsOnlyDigits(phone);
//}
//
//// Numeric validation for income and bills
//bool isValidNumeric(const string& num) {
//    return !num.empty() && containsOnlyDigits(num);
//}
//
//// Employment status validation
//bool isValidEmploymentStatus(const string& status) {
//    return status == "Self-employed" || status == "salaried" ||
//        status == "retired" || status == "unemployed";
//}
//
//// Marital status validation  
//bool isValidMaritalStatus(const string& status) {
//    return status == "Single" || status == "Married" ||
//        status == "Divorced" || status == "Widowed";
//}
//
//// Loan status validation
//bool isValidLoanStatus(const string& status) {
//    return status == "Active" || status == "Inactive";
//}
//
//// Loan category validation
//bool isValidLoanCategory(const string& category) {
//    return category == "Car" || category == "Home" || category == "Bike";
//}
//bool isValidName(const string& name) {
//    if (name.empty()) return false;
//    for (char c : name) {
//        if (!isalpha(c) && c != ' ') return false;
//    }
//    return true;
//}
//
//// Postal code validation - 5 digits
//bool isValidPostalCode(const string& postalCode) {
//    return postalCode.length() == 5 && containsOnlyDigits(postalCode);
//}
//
//// searching count through CNIC
//void countApplicationsByCNIC(string cnic) {
//    ifstream file("applications.txt");
//    if (!file) {
//        cout << "Error: applications.txt not found!" << endl;
//        return;
//    }
//
//    string line;
//    int submitted = 0;
//    int approved = 0;
//    int rejected = 0;
//
//    while (getline(file, line)) {
//       // Split by #
//        string fields[40];
//        int idx = 0;
//        string temp = "";
//
//        for (int i = 0; i < line.length(); i++) {
//            if (line[i] == '#') {
//                fields[idx++] = temp;
//                temp = "";
//            }
//            else {
//                temp += line[i];
//            }
//        }
//        fields[idx] = temp; // last one
//
//        // CNIC is in index 6
//        if (fields[6] == cnic) {
//            submitted++;
//
//            string status = fields[31];  // last field
//
//            if (status == "approved" || status == "Approved")
//                approved++;
//            else if (status == "rejected" || status == "Rejected")
//                rejected++;
//        }
//    }
//
//    cout << "\n--- Application Count for CNIC: " << cnic << " ---\n";
//    cout << "Submitted: " << submitted << endl;
//    cout << "Approved:  " << approved << endl;
//    cout << "Rejected:  " << rejected << endl;
//}