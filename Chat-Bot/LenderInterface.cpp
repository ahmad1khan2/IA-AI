#include "applicant.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <memory>
#include <windows.h>  // Add Windows API header
using namespace std;

// Helper: split a string by a delimiter
static vector<string> splitTokens(const string& s, char delim = '#') {
    vector<string> out;
    string token;
    stringstream ss(s);
    while (getline(ss, token, delim)) {
        out.push_back(token);
    }
    return out;
}

// Helper function for Windows file locking
static bool lockFile(HANDLE fileHandle) {
    OVERLAPPED overlapped = { 0 };
    return LockFileEx(fileHandle, LOCKFILE_EXCLUSIVE_LOCK, 0, MAXDWORD, MAXDWORD, &overlapped);
}

static bool unlockFile(HANDLE fileHandle) {
    OVERLAPPED overlapped = { 0 };
    return UnlockFileEx(fileHandle, 0, MAXDWORD, MAXDWORD, &overlapped);
}

// Read all applications from file into unique_ptr<Applicant> objects
vector<unique_ptr<Applicant>> readApplications(const string& path) {
    vector<unique_ptr<Applicant>> apps;

    // Open file with Windows HANDLE for locking
    HANDLE hFile = CreateFileA(
        path.c_str(),
        GENERIC_READ,
        0,  // No sharing while we have the lock
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        cerr << "Error: cannot open " << path << " for reading. Error: " << GetLastError() << '\n';
        return apps;
    }

    // Acquire exclusive lock
    if (!lockFile(hFile)) {
        cerr << "Error: cannot lock " << path << " for reading. Error: " << GetLastError() << '\n';
        CloseHandle(hFile);
        return apps;
    }

    // Now read the file content
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
        if (line.empty()) continue;
        vector<string> t = splitTokens(line, '#');

        // Updated: Need 36 fields for full data including document paths
        if (t.size() < 36) {
            // tolerate shorter lines but skip invalid
            cerr << "Warning: skipping malformed line (fields=" << t.size() << "):\n  " << line << '\n';
            continue;
        }

        auto a = make_unique<Applicant>();
        size_t idx = 0;

        // Basic information (fields 0-14)
        a->applicationId = t[idx++];            // 0
        a->fullName = t[idx++];                 // 1
        a->fathersName = t[idx++];              // 2
        a->postalAddress = t[idx++];            // 3
        a->contactNumber = t[idx++];            // 4
        a->email = t[idx++];                    // 5
        a->cnic = t[idx++];                     // 6
        a->cnicExpiry = t[idx++];               // 7
        a->employmentStatus = t[idx++];         // 8
        a->maritalStatus = t[idx++];            // 9
        a->gender = t[idx++];                   // 10
        a->dependents = t[idx++];               // 11
        a->annualIncome = t[idx++];             // 12
        a->avgElectricityBill = t[idx++];       // 13
        a->currentElectricityBill = t[idx++];   // 14

        // Existing loans (fields 15-20) - always 6 fields
        a->existingLoans.clear();
        if (t[15] != "NO_LOAN" && t[15] != "No") {
            a->addExistingLoan(t[15], t[16], t[17], t[18], t[19], t[20]);
        }
        idx += 6; // Move past the loan fields

        // Referees (fields 21-30) - 10 fields total (5 each)
        a->referee1.name = t[idx++];            // 21
        a->referee1.cnic = t[idx++];            // 22
        a->referee1.issueDate = t[idx++];       // 23
        a->referee1.phone = t[idx++];           // 24
        a->referee1.email = t[idx++];           // 25

        a->referee2.name = t[idx++];            // 26
        a->referee2.cnic = t[idx++];            // 27
        a->referee2.issueDate = t[idx++];       // 28
        a->referee2.phone = t[idx++];           // 29
        a->referee2.email = t[idx++];           // 30

        // NEW: Document paths (fields 31-34) - 4 fields
        a->cnicFrontPath = t[idx++];            // 31
        a->cnicBackPath = t[idx++];             // 32
        a->electricityBillPath = t[idx++];      // 33
        a->salarySlipPath = t[idx++];           // 34

        // Status (field 35)
        a->status = t[idx++];                   // 35

        apps.push_back(std::move(a));
    }

    return apps;
}

// Convert an Applicant back to file-format line
string applicantToLine(const Applicant& a) {
    stringstream ss;
    ss << a.applicationId << '#'
        << a.fullName << '#'
        << a.fathersName << '#'
        << a.postalAddress << '#'
        << a.contactNumber << '#'
        << a.email << '#'
        << a.cnic << '#'
        << a.cnicExpiry << '#'
        << a.employmentStatus << '#'
        << a.maritalStatus << '#'
        << a.gender << '#'
        << a.dependents << '#'
        << a.annualIncome << '#'
        << a.avgElectricityBill << '#'
        << a.currentElectricityBill << '#';

    // existing loans (each contributes 6 fields)
    for (const auto& L : a.existingLoans) {
        ss << L.loanStatus << '#'
            << L.totalAmount << '#'
            << L.amountReturned << '#'
            << L.amountDue << '#'
            << L.bankName << '#'
            << L.loanCategory << '#';
    }

    // ensure at least one existing-loan block exists in the file to match expected format.
    if (a.existingLoans.empty()) {
        // write a single "No" block (six "No" fields) to preserve format
        ss << "No#No#No#No#No#No#";
    }

    // referees (10 fields total)
    ss << a.referee1.name << '#'
        << a.referee1.cnic << '#'
        << a.referee1.issueDate << '#'
        << a.referee1.phone << '#'
        << a.referee1.email << '#';

    ss << a.referee2.name << '#'
        << a.referee2.cnic << '#'
        << a.referee2.issueDate << '#'
        << a.referee2.phone << '#'
        << a.referee2.email << '#';

    // NEW: Document paths (4 fields)
    ss << a.cnicFrontPath << '#'
        << a.cnicBackPath << '#'
        << a.electricityBillPath << '#'
        << a.salarySlipPath << '#';

    // status
    ss << a.status;

    return ss.str();
}

// Write vector of unique_ptr<Applicant> back to applications file (overwrite)
bool writeApplications(const string& path, const vector<unique_ptr<Applicant>>& apps) {
    // Open file with Windows HANDLE for locking
    HANDLE hFile = CreateFileA(
        path.c_str(),
        GENERIC_WRITE,
        0,  // No sharing while we have the lock
        NULL,
        CREATE_ALWAYS,  // Overwrite existing file
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        cerr << "Error: cannot open " << path << " for writing. Error: " << GetLastError() << '\n';
        return false;
    }

    // Acquire exclusive lock
    if (!lockFile(hFile)) {
        cerr << "Error: cannot lock " << path << " for writing. Error: " << GetLastError() << '\n';
        CloseHandle(hFile);
        return false;
    }

    // Write content to file
    DWORD bytesWritten;
    for (size_t i = 0; i < apps.size(); ++i) {
        string line = applicantToLine(*apps[i]);
        if (i + 1 < apps.size()) line += '\n';

        if (!WriteFile(hFile, line.c_str(), line.length(), &bytesWritten, NULL)) {
            cerr << "Error writing to file. Error: " << GetLastError() << '\n';
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

// Print a brief list (index, id, name, status)
void printApplicationsList(const vector<unique_ptr<Applicant>>& apps) {
    cout << "\nApplications:\n";
    cout << "Idx  | ID       | Name                        | Status\n";
    cout << "-----+----------+-----------------------------+---------\n";
    for (size_t i = 0; i < apps.size(); ++i) {
        cout << setw(4) << right << (i + 1) << " | "
            << setw(8) << left << apps[i]->applicationId << " | "
            << setw(27) << left << apps[i]->fullName << " | "
            << setw(8) << left << apps[i]->status << '\n';
    }
    cout << '\n';
}

// Print full applicant details (table-like)
void printApplicantDetails(const Applicant& a) {
    cout << "\n=== Application " << a.applicationId << " ===\n";
    cout << "Full name:        " << a.fullName << '\n';
    cout << "Father's name:    " << a.fathersName << '\n';
    cout << "Postal address:   " << a.postalAddress << '\n';
    cout << "Contact number:   " << a.contactNumber << '\n';
    cout << "Email:            " << a.email << '\n';
    cout << "CNIC:             " << a.cnic << '\n';
    cout << "CNIC expiry:      " << a.cnicExpiry << '\n';
    cout << "Employment:       " << a.employmentStatus << '\n';
    cout << "Marital status:   " << a.maritalStatus << '\n';
    cout << "Gender:           " << a.gender << '\n';
    cout << "Dependents:       " << a.dependents << '\n';
    cout << "Annual income:    " << a.annualIncome << '\n';
    cout << "Avg elec bill:    " << a.avgElectricityBill << '\n';
    cout << "Current elec bill:" << a.currentElectricityBill << '\n';

    cout << "\nExisting loans (" << a.existingLoans.size() << "):\n";
    if (a.existingLoans.empty()) {
        cout << "  (none)\n";
    }
    else {
        for (size_t i = 0; i < a.existingLoans.size(); ++i) {
            const auto& L = a.existingLoans[i];
            cout << "  Loan " << (i + 1) << ": status=" << L.loanStatus
                << ", total=" << L.totalAmount
                << ", returned=" << L.amountReturned
                << ", due=" << L.amountDue
                << ", bank=" << L.bankName
                << ", category=" << L.loanCategory << '\n';
        }
    }

    cout << "\nReferee 1: " << a.referee1.name << ", CNIC: " << a.referee1.cnic
        << ", Issue: " << a.referee1.issueDate << ", Phone: " << a.referee1.phone
        << ", Email: " << a.referee1.email << '\n';

    cout << "Referee 2: " << a.referee2.name << ", CNIC: " << a.referee2.cnic
        << ", Issue: " << a.referee2.issueDate << ", Phone: " << a.referee2.phone
        << ", Email: " << a.referee2.email << '\n';

    // NEW: Display document paths
    cout << "\nDocument Paths:\n";
    cout << "CNIC Front:      " << (a.cnicFrontPath.empty() ? "Not uploaded" : a.cnicFrontPath) << '\n';
    cout << "CNIC Back:       " << (a.cnicBackPath.empty() ? "Not uploaded" : a.cnicBackPath) << '\n';
    cout << "Electricity Bill:" << (a.electricityBillPath.empty() ? "Not uploaded" : a.electricityBillPath) << '\n';
    cout << "Salary Slip:     " << (a.salarySlipPath.empty() ? "Not uploaded" : a.salarySlipPath) << '\n';

    cout << "Current status: " << a.status << '\n';
}

// Menu to select application and change status
void runLenderInterface(const string& path = "applications.txt") {
    vector<unique_ptr<Applicant>> apps = readApplications(path);
    if (apps.empty()) {
        cout << "No applications loaded.\n";
        return;
    }

    while (true) {
        printApplicationsList(apps);
        cout << "Select application by index (or 0 to exit): ";
        string sel;
        getline(cin, sel);
        int idx = -1;
        try { idx = stoi(sel); }
        catch (...) { idx = -1; }
        if (idx == 0) {
            cout << "Exiting lender interface.\n";
            break;
        }
        if (idx < 1 || idx >(int)apps.size()) {
            cout << "Invalid selection.\n";
            continue;
        }
        Applicant* chosen = apps[idx - 1].get();
        printApplicantDetails(*chosen);

        cout << "\nChange status? (1=submitted, 2=approved, 3=rejected, 0=back): ";
        string choice;
        getline(cin, choice);
        if (choice == "0") continue;
        string newStatus;
        if (choice == "1") newStatus = "submitted";
        else if (choice == "2") newStatus = "approved";
        else if (choice == "3") newStatus = "rejected";
        else {
            cout << "Unknown option, no change made.\n";
            continue;
        }

        if (newStatus == chosen->status) {
            cout << "Status is already \"" << newStatus << "\". No change.\n";
            continue;
        }

        cout << "Change status of application " << chosen->applicationId
            << " from \"" << chosen->status << "\" to \"" << newStatus << "\"? (y/n): ";
        string confirm;
        getline(cin, confirm);
        if (!confirm.empty() && (confirm[0] == 'y' || confirm[0] == 'Y')) {
            chosen->status = newStatus;
            if (!writeApplications(path, apps)) {
                cerr << "Failed to write changes to file.\n";
            }
            else {
                cout << "Status updated and written to file.\n";
            }
        }
        else {
            cout << "Cancelled.\n";
        }
    }
}

int main()
{
    runLenderInterface();
    return 0;
}