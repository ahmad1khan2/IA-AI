#include "fileHandling.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <windows.h>  // Add Windows API header
#include "applicant.h"


using namespace std;

// Initialize static members
int Parser::utteranceCount = 0;
int Parser::homeCount = 0;
int Parser::scooterCount = 0;
int Parser::carCount = 0;
int Parser::personalCount = 0;

// Windows file locking helpers
static bool lockFile(HANDLE fileHandle) {
    OVERLAPPED overlapped = { 0 };
    return LockFileEx(fileHandle, LOCKFILE_EXCLUSIVE_LOCK, 0, MAXDWORD, MAXDWORD, &overlapped);
}

static bool unlockFile(HANDLE fileHandle) {
    OVERLAPPED overlapped = { 0 };
    return UnlockFileEx(fileHandle, 0, MAXDWORD, MAXDWORD, &overlapped);
}

// --- Helper function implementations ---

int Parser::stoiSafe(const string& str) {
    try {
        // Remove commas and spaces from the string to handle formatted numbers
        string cleanedStr;
        for (char c : str) {
            if (c != ',' && c != ' ') {
                cleanedStr += c;
            }
        }

        // Return 0 if the string is empty after cleaning
        if (cleanedStr.empty()) {
            return 0;
        }

        return stoi(cleanedStr);
    }
    catch (const std::invalid_argument& e) {
        // Return 0 if conversion fails
        return 0;
    }
    catch (const std::out_of_range& e) {
        // Return 0 if number is too large
        return 0;
    }
    catch (...) {
        return 0;
    }
}

double Parser::stodSafe(const string& str) {
    try {
        // Remove commas from the string before conversion
        string cleanedStr;
        for (char c : str) {
            if (c != ',' && c != ' ') {
                cleanedStr += c;
            }
        }

        if (cleanedStr.empty()) return 0.0;

        return stod(cleanedStr);
    }
    catch (...) {
        return 0.0;
    }
}

bool Parser::stobSafe(const string& str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);

    // Remove any potential whitespace
    lowerStr.erase(remove(lowerStr.begin(), lowerStr.end(), ' '), lowerStr.end());

    // Check against common boolean string representations
    return (lowerStr == "1" || lowerStr == "true" || lowerStr == "yes");
}

// --- Function implementations ---

void Parser::readUtterances(vector<string>& userInput, vector<string>& botReply) {
    ifstream file("Utterances.txt");
    if (!file) {
        cout << "Error opening Utterances.txt!" << endl;
        return;
    }

    string line;
    utteranceCount = 0;

    while (getline(file, line)) {
        // Skip empty lines
        if (line.empty()) continue;

        size_t delimiterPos = line.find('#');

        // If no delimiter is found, skip this line
        if (delimiterPos == string::npos) continue;

        string left = line.substr(0, delimiterPos);
        string right = line.substr(delimiterPos + 1);

        // Trim whitespace from both sides
        if (!left.empty()) {
            left.erase(0, left.find_first_not_of(" \t"));
            if (!left.empty()) left.erase(left.find_last_not_of(" \t") + 1);
        }

        if (!right.empty()) {
            right.erase(0, right.find_first_not_of(" \t"));
            if (!right.empty()) right.erase(right.find_last_not_of(" \t") + 1);
        }

        // Store parsed data into vectors
        userInput.push_back(left);
        botReply.push_back(right);

        utteranceCount++;
    }

    file.close();
}

vector<homeLoan> Parser::readHome() {
    vector<homeLoan> homeLoans;
    ifstream homeFile("Home.txt");
    if (!homeFile) {
        cout << "Error opening Home.txt!" << endl;
        return homeLoans;
    }

    string homeLine;
    // Read and discard the header line
    if (!getline(homeFile, homeLine)) {
        cout << "Home.txt is empty or unreadable!" << endl;
        return homeLoans;
    }

    homeCount = 0;
    while (getline(homeFile, homeLine)) {
        if (homeLine.empty()) continue;

        string parts[5];
        int partIndex = 0;
        string currentPart = "";

        // Split line by '#' delimiter
        for (char c : homeLine) {
            if (c == '#') {
                if (partIndex < 5) {
                    // Trim whitespace and store part
                    currentPart.erase(0, currentPart.find_first_not_of(" \t"));
                    if (!currentPart.empty()) currentPart.erase(currentPart.find_last_not_of(" \t") + 1);
                    parts[partIndex++] = currentPart;
                    currentPart = "";
                }
            }
            else {
                currentPart += c;
            }
        }

        // Process the final part
        if (!currentPart.empty() && partIndex < 5) {
            currentPart.erase(0, currentPart.find_first_not_of(" \t"));
            if (!currentPart.empty()) currentPart.erase(currentPart.find_last_not_of(" \t") + 1);
            parts[partIndex] = currentPart;
        }

        if (partIndex == 4) {
            // Extract the numeric area from strings like "Area 1" or "Area 10"
            int area = 0;
            if (parts[0].find("Area") != string::npos) {
                size_t pos = parts[0].find("Area");
                string numberStr;

                // Iterate through characters after "Area" to find digits
                for (size_t i = pos + 4; i < parts[0].length(); i++) {
                    if (isdigit(parts[0][i])) {
                        numberStr += parts[0][i];
                    }
                    else if (!numberStr.empty()) {
                        break; // Stop if we hit non-digits after finding a number
                    }
                }
                if (!numberStr.empty()) {
                    area = stoi(numberStr);
                }
            }
            else {
                // Fallback for standard number format
                area = stoiSafe(parts[0]);
            }

            string size = parts[1];
            int installments = stoiSafe(parts[2]);
            int price = stoiSafe(parts[3]);
            int down = stoiSafe(parts[4]);

            // Validate data integrity before creating object
            if (price <= 0 || installments <= 0) {
                continue;
            }

            homeLoan loan(area, size, installments, price, down);
            homeLoans.push_back(loan);
            homeCount++;
        }
    }
    homeFile.close();
    return homeLoans;
}

vector<scooterLoan> Parser::readScooter() {
    vector<scooterLoan> scooterLoans;
    ifstream scooterFile("Scooter.txt");
    if (!scooterFile) {
        cout << "Error opening Scooter.txt!" << endl;
        return scooterLoans;
    }

    string scooterLine;
    if (!getline(scooterFile, scooterLine)) {
        cout << "Scooter.txt is empty or unreadable!" << endl;
        return scooterLoans;
    }

    scooterCount = 0;
    while (getline(scooterFile, scooterLine)) {
        if (scooterLine.empty()) continue;

        string parts[8];
        int partIndex = 0;
        string currentPart = "";

        // Split line by '#' delimiter
        for (char c : scooterLine) {
            if (c == '#') {
                if (partIndex < 8) {
                    currentPart.erase(0, currentPart.find_first_not_of(" \t"));
                    if (!currentPart.empty()) currentPart.erase(currentPart.find_last_not_of(" \t") + 1);
                    parts[partIndex++] = currentPart;
                    currentPart = "";
                }
            }
            else {
                currentPart += c;
            }
        }

        if (!currentPart.empty() && partIndex < 8) {
            currentPart.erase(0, currentPart.find_first_not_of(" \t"));
            if (!currentPart.empty()) currentPart.erase(currentPart.find_last_not_of(" \t") + 1);
            parts[partIndex] = currentPart;
        }

        if (partIndex == 7) {
            string make = parts[0];
            string model = parts[1];
            int distance = stoiSafe(parts[2]);
            double chargingTime = stodSafe(parts[3]);
            int maxSpeed = stoiSafe(parts[4]);
            int installments = stoiSafe(parts[5]);
            int price = stoiSafe(parts[6]);
            int down = stoiSafe(parts[7]);

            // Validate that critical financial values are positive
            if (price <= 0 || installments <= 0) {
                continue;
            }

            scooterLoan loan(make, model, distance, chargingTime, maxSpeed,
                installments, price, down);
            scooterLoans.push_back(loan);
            scooterCount++;
        }
    }
    scooterFile.close();
    return scooterLoans;
}

vector<carLoan> Parser::readCar() {
    vector<carLoan> carLoans;
    ifstream carFile("Car.txt");
    if (!carFile) {
        cout << "Error opening Car.txt!" << endl;
        return carLoans;
    }

    string carLine;
    if (!getline(carFile, carLine)) {
        cout << "Car.txt is empty or unreadable!" << endl;
        return carLoans;
    }

    carCount = 0;
    while (getline(carFile, carLine)) {
        if (carLine.empty()) continue;

        string parts[8];
        int partIndex = 0;
        string currentPart = "";

        // Split line by '#' delimiter
        for (char c : carLine) {
            if (c == '#') {
                if (partIndex < 8) {
                    currentPart.erase(0, currentPart.find_first_not_of(" \t"));
                    if (!currentPart.empty()) currentPart.erase(currentPart.find_last_not_of(" \t") + 1);
                    parts[partIndex++] = currentPart;
                    currentPart = "";
                }
            }
            else {
                currentPart += c;
            }
        }

        if (!currentPart.empty() && partIndex < 8) {
            currentPart.erase(0, currentPart.find_first_not_of(" \t"));
            if (!currentPart.empty()) currentPart.erase(currentPart.find_last_not_of(" \t") + 1);
            parts[partIndex] = currentPart;
        }

        if (partIndex == 7) {
            string make = parts[0];
            string model = parts[1];
            int engine = stoiSafe(parts[2]);
            bool used = stobSafe(parts[3]);
            int year = stoiSafe(parts[4]);
            int installments = stoiSafe(parts[5]);
            int price = stoiSafe(parts[6]);
            int down = stoiSafe(parts[7]);

            // Validate that critical financial values are positive
            if (price <= 0 || installments <= 0) {
                continue;
            }

            carLoan loan(make, model, engine, used, year,
                installments, price, down);
            carLoans.push_back(loan);
            carCount++;
        }
    }
    carFile.close();
    return carLoans;
}

vector<personalLoan> Parser::readPersonal() {
    vector<personalLoan> personalLoans;
    ifstream personalFile("personal.txt");
    if (!personalFile) {
        cout << "Error opening personal.txt!" << endl;
        return personalLoans;
    }

    string personalLine;
    // Read and discard the header line
    if (!getline(personalFile, personalLine)) {
        cout << "personal.txt is empty or unreadable!" << endl;
        return personalLoans;
    }

    personalCount = 0;
    while (getline(personalFile, personalLine)) {
        if (personalLine.empty()) continue;

        string parts[6];
        int partIndex = 0;
        string currentPart = "";

        // Split line by '#' delimiter
        for (char c : personalLine) {
            if (c == '#') {
                if (partIndex < 6) {
                    // Trim whitespace and store part
                    currentPart.erase(0, currentPart.find_first_not_of(" \t"));
                    if (!currentPart.empty()) currentPart.erase(currentPart.find_last_not_of(" \t") + 1);
                    parts[partIndex++] = currentPart;
                    currentPart = "";
                }
            }
            else {
                currentPart += c;
            }
        }

        // Process the final part
        if (!currentPart.empty() && partIndex < 6) {
            currentPart.erase(0, currentPart.find_first_not_of(" \t"));
            if (!currentPart.empty()) currentPart.erase(currentPart.find_last_not_of(" \t") + 1);
            parts[partIndex] = currentPart;
        }

        if (partIndex == 5) {
            string loanType = parts[0];
            int maxAmount = stoiSafe(parts[1]);
            int installments = stoiSafe(parts[2]);

            // Process interest rate - remove percentage sign if present
            string interestStr = parts[3];
            if (!interestStr.empty() && interestStr.back() == '%') {
                interestStr.pop_back();
            }
            double interestRate = stodSafe(interestStr);

            int processingFee = stoiSafe(parts[4]);
            int downPayment = stoiSafe(parts[5]);

            // For personal loans, we set price to 0 initially since it will be entered by user
            // and validated to be less than maxAmount
            int price = 0;

            // Validate data integrity before creating object
            if (maxAmount <= 0 || installments <= 0) {
                continue;
            }

            personalLoan loan(loanType, maxAmount, interestRate, processingFee,
                installments, price, downPayment);
            personalLoans.push_back(loan);
            personalCount++;
        }
    }
    personalFile.close();
    return personalLoans;
}

vector<Loan*> Parser::readAllLoans() {
    vector<Loan*> allLoans;

    // Use try-catch to prevent memory leaks if an error occurs during reading
    try {
        // Read and store home loans
        vector<homeLoan> homeLoans = readHome();
        for (auto& loan : homeLoans) {
            allLoans.push_back(new homeLoan(loan));
        }

        // Read and store car loans
        vector<carLoan> carLoans = readCar();
        for (auto& loan : carLoans) {
            allLoans.push_back(new carLoan(loan));
        }

        // Read and store scooter loans
        vector<scooterLoan> scooterLoans = readScooter();
        for (auto& loan : scooterLoans) {
            allLoans.push_back(new scooterLoan(loan));
        }

        // Read and store personal loans
        vector<personalLoan> personalLoans = readPersonal();
        for (auto& loan : personalLoans) {
            allLoans.push_back(new personalLoan(loan));
        }
    }
    catch (...) {
        // If any exception happens, delete allocated memory and rethrow
        cleanupLoans(allLoans);
        throw;
    }

    return allLoans;
}


bool Parser::saveApplicationToFile(const string& fileData) {
    // Extract application ID from the file data
    size_t firstHash = fileData.find('#');
    string appId = (firstHash != string::npos) ? fileData.substr(0, firstHash) : "";

    if (appId.empty()) {
        return false;
    }

    // Open file with Windows HANDLE for locking
    HANDLE hFile = CreateFileA(
        "applications.txt",
        GENERIC_READ | GENERIC_WRITE,  // Need both read and write access
        0,  // No sharing while we have the lock
        NULL,
        OPEN_ALWAYS,  // Open existing or create new
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        return false;
    }

    // Acquire exclusive lock
    if (!lockFile(hFile)) {
        CloseHandle(hFile);
        return false;
    }

    // Read existing content
    stringstream fileContent;
    char buffer[4096];
    DWORD bytesRead;

    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    while (ReadFile(hFile, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        fileContent << buffer;
    }

    // Process content to remove duplicates
    vector<string> lines;
    string line;
    while (getline(fileContent, line)) {
        if (!line.empty()) {
            // Check if this line has the same application ID
            size_t lineFirstHash = line.find('#');
            string lineAppId = (lineFirstHash != string::npos) ? line.substr(0, lineFirstHash) : "";

            // Only keep lines that don't have the same application ID
            if (lineAppId != appId) {
                lines.push_back(line);
            }
        }
    }

    // Add the new/updated application data
    lines.push_back(fileData);

    // Truncate file and write everything back
    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    SetEndOfFile(hFile);  // Truncate the file

    // Write all lines back
    for (const auto& l : lines) {
        string toWrite = l + "\n";
        DWORD bytesWritten;
        if (!WriteFile(hFile, toWrite.c_str(), static_cast<DWORD>(toWrite.length()), &bytesWritten, NULL)) {
            unlockFile(hFile);
            CloseHandle(hFile);
            return false;
        }
    }

    // Release lock and close handle
    unlockFile(hFile);
    CloseHandle(hFile);

    return true;
}

string Parser::generateApplicationId() {
    int highestId = 1000; // Default starting ID

    // Open file with Windows HANDLE for locking
    HANDLE hFile = CreateFileA(
        "applications.txt",
        GENERIC_READ,
        0,  // No sharing while we have the lock
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        // File doesn't exist, return default starting ID
        return to_string(highestId + 1);
    }

    // Acquire exclusive lock
    if (!lockFile(hFile)) {
        CloseHandle(hFile);
        return to_string(highestId + 1);
    }

    // Read file content
    stringstream fileContent;
    char buffer[4096];
    DWORD bytesRead;

    while (ReadFile(hFile, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        fileContent << buffer;
    }

    // Release lock and close handle
    unlockFile(hFile);
    CloseHandle(hFile);

    // Parse the content from stringstream
    string line;
    while (getline(fileContent, line)) {
        if (!line.empty()) {
            size_t pos = line.find('#');
            if (pos != string::npos) {
                string idStr = line.substr(0, pos);
                try {
                    int currentId = stoi(idStr);
                    if (currentId > highestId) {
                        highestId = currentId;
                    }
                }
                catch (...) {
                    // Ignore invalid IDs
                }
            }
        }
    }

    // Return next ID
    return to_string(highestId + 1);
}

void Parser::countApplicationsByCNIC(const string& cnic) {
    // Open file with Windows HANDLE for locking
    HANDLE hFile = CreateFileA(
        "applications.txt",
        GENERIC_READ,
        0,  // No sharing while we have the lock
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        cout << "Error: applications.txt not found!" << endl;
        return;
    }

    // Acquire exclusive lock
    if (!lockFile(hFile)) {
        cout << "Error: cannot lock applications.txt!" << endl;
        CloseHandle(hFile);
        return;
    }

    // Read file content
    stringstream fileContent;
    char buffer[4096];
    DWORD bytesRead;

    while (ReadFile(hFile, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        fileContent << buffer;
    }

    // Release lock and close handle
    unlockFile(hFile);
    CloseHandle(hFile);

    // Parse the content from stringstream
    int submitted = 0;
    int approved = 0;
    int rejected = 0;
    string line;

    while (getline(fileContent, line)) {
        // Use vector instead of fixed array
        vector<string> fields;
        string field;
        stringstream ss(line);

        // Split by # delimiter safely
        while (getline(ss, field, '#')) {
            fields.push_back(field);
        }

        // Check if we have enough fields before accessing
        if (fields.size() >= 32) { // Need at least 32 fields for CNIC (index 6) and status (index 31)
            // CNIC is at index 6
            if (fields[6] == cnic) {
                submitted++;
                string status = fields[31]; // status field

                if (status == "approved" || status == "Approved")
                    approved++;
                else if (status == "rejected" || status == "Rejected")
                    rejected++;
            }
        }
        // Handle records with fewer fields but at least CNIC field
        else if (fields.size() >= 7) { // At least have CNIC at index 6
            if (fields[6] == cnic) {
                submitted++;
                // Can't determine approval status with insufficient fields
            }
        }
    }

    cout << "\n--- Application Count for CNIC: " << cnic << " ---\n";
    cout << "Submitted: " << submitted << endl;
    cout << "Approved:  " << approved << endl;
    cout << "Rejected:  " << rejected << endl;
}


bool Parser::isValidCNIC(const string& cnic) {
    if (cnic.length() != 13) return false;
    for (char c : cnic) {
        if (!isdigit(c)) return false;
    }
    return true;
}

void Parser::cleanupLoans(vector<Loan*>& loans) {
    for (Loan* loan : loans) {
        delete loan;
    }
    loans.clear();
}

bool Parser::loadApplication(const string& appId, const string& cnic, Applicant& applicant) {
    ifstream file("applications.txt");
    if (!file) {
        cout << "Error: applications.txt not found!" << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        vector<string> fields;
        string field;
        stringstream ss(line);

        // Split by # delimiter
        while (getline(ss, field, '#')) {
            fields.push_back(field);
        }

        // Check for 36 fields total
        if (fields.size() == 36 && fields[0] == appId && fields[6] == cnic) {
            string currentStatus = fields[35]; // Status is last field (index 35)

            // Don't allow loading of submitted applications
            if (currentStatus == "Submitted") {
                cout << "Error: This application is already submitted and cannot be modified." << endl;
                file.close();
                return false;
            }

            // Load ALL data into applicant object with CORRECT 36-field mapping:
            applicant.applicationId = fields[0];
            applicant.fullName = (fields[1] == "NOT_PROVIDED") ? "" : fields[1];
            applicant.fathersName = (fields[2] == "NOT_PROVIDED") ? "" : fields[2];
            applicant.postalAddress = (fields[3] == "NOT_PROVIDED") ? "" : fields[3];
            applicant.contactNumber = (fields[4] == "NOT_PROVIDED") ? "" : fields[4];
            applicant.email = (fields[5] == "NOT_PROVIDED") ? "" : fields[5];
            applicant.cnic = fields[6];
            applicant.cnicExpiry = (fields[7] == "NOT_PROVIDED") ? "" : fields[7];
            applicant.employmentStatus = (fields[8] == "NOT_PROVIDED") ? "" : fields[8];
            applicant.maritalStatus = (fields[9] == "NOT_PROVIDED") ? "" : fields[9];
            applicant.gender = (fields[10] == "NOT_PROVIDED") ? "" : fields[10];
            applicant.dependents = (fields[11] == "NOT_PROVIDED") ? "" : fields[11];
            applicant.annualIncome = (fields[12] == "0") ? "" : fields[12];
            applicant.avgElectricityBill = (fields[13] == "0") ? "" : fields[13];
            applicant.currentElectricityBill = (fields[14] == "0") ? "" : fields[14];

            // Load existing loans if any
            applicant.existingLoans.clear();
            if (fields[15] != "NO_LOAN") {
                applicant.addExistingLoan(fields[15], fields[16], fields[17], fields[18], fields[19], fields[20]);
            }

            // Load references (fields 21-30)
            applicant.referee1.name = (fields[21] == "NOT_PROVIDED") ? "" : fields[21];
            applicant.referee1.cnic = (fields[22] == "NOT_PROVIDED") ? "" : fields[22];
            applicant.referee1.issueDate = (fields[23] == "NOT_PROVIDED") ? "" : fields[23];
            applicant.referee1.phone = (fields[24] == "NOT_PROVIDED") ? "" : fields[24];
            applicant.referee1.email = (fields[25] == "NOT_PROVIDED") ? "" : fields[25];
            applicant.referee2.name = (fields[26] == "NOT_PROVIDED") ? "" : fields[26];
            applicant.referee2.cnic = (fields[27] == "NOT_PROVIDED") ? "" : fields[27];
            applicant.referee2.issueDate = (fields[28] == "NOT_PROVIDED") ? "" : fields[28];
            applicant.referee2.phone = (fields[29] == "NOT_PROVIDED") ? "" : fields[29];
            applicant.referee2.email = (fields[30] == "NOT_PROVIDED") ? "" : fields[30];

            // Load document paths (fields 31-34)
            applicant.cnicFrontPath = (fields[31] == "NOT_UPLOADED") ? "" : fields[31];
            applicant.cnicBackPath = (fields[32] == "NOT_UPLOADED") ? "" : fields[32];
            applicant.electricityBillPath = (fields[33] == "NOT_UPLOADED") ? "" : fields[33];
            applicant.salarySlipPath = (fields[34] == "NOT_UPLOADED") ? "" : fields[34];

            applicant.status = fields[35];

            file.close();
            return true;
        }
    }

    file.close();
    cout << "Error: Application not found with ID: " << appId << " and CNIC: " << cnic << endl;
    return false;
}

void Parser::loadHumanCorpus(vector<string>& human1Lines, vector<string>& human2Lines) {
    ifstream file("human_chat_corpus.txt");
    if (!file.is_open()) {
        cout << "Chatbot: Error loading chat corpus. General chat mode unavailable." << endl;
        return;
    }

    string line;
    int human1Count = 0;
    int human2Count = 0;

    while (getline(file, line)) {
        if (line.find("Human 1: ") == 0) {
            human1Lines.push_back(line.substr(9)); // Remove "Human 1: " prefix
            human1Count++;
        }
        else if (line.find("Human 2: ") == 0) {
            human2Lines.push_back(line.substr(9)); // Remove "Human 2: " prefix
            human2Count++;
        }
    }

    file.close();
    cout << "Chatbot: Loaded " << human1Count << " Human1 lines and " << human2Count << " Human2 lines." << endl;
}