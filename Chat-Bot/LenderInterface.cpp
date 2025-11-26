#include "applicant.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <memory>
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

// Read all applications from file into unique_ptr<Applicant> objects
vector<unique_ptr<Applicant>> readApplications(const string& path) {
    vector<unique_ptr<Applicant>> apps;
    ifstream in(path);
    if (!in) {
        cerr << "Error: cannot open " << path << " for reading.\n";
        return apps;
    }

    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        vector<string> t = splitTokens(line, '#');
        // Need at least 15 fields + one existing-loan(6) + 5 + 5 + 1 = 32 fields for full data
        if (t.size() < 32) {
            // tolerate shorter lines but skip invalid
            cerr << "Warning: skipping malformed line (fields=" << t.size() << "):\n  " << line << '\n';
            continue;
        }

        auto a = make_unique<Applicant>();
        size_t idx = 0;
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

        // remaining fields: one or more existing loans (groups of 6),
        // then referee1 (5), referee2 (5), then status (1).
        size_t remaining = t.size() - idx;
        if (remaining < 11) { // not enough for two referees + status
            cerr << "Warning: not enough fields for referees/status, skipping line\n";
            continue;
        }
        size_t loansArea = remaining - 11; // fields dedicated to existing loans (multiple of 6)
        size_t loanCount = 0;
        if (loansArea >= 6) loanCount = loansArea / 6;
        // parse existing loans
        for (size_t li = 0; li < loanCount; ++li) {
            size_t base = idx + li * 6;
            ExistingLoan L(
                t[base + 0], t[base + 1], t[base + 2],
                t[base + 3], t[base + 4], t[base + 5]);
            a->existingLoans.push_back(L);
        }
        idx += loanCount * 6;

        // parse referees
        a->referee1.name = (idx < t.size()) ? t[idx++] : "";
        a->referee1.cnic = (idx < t.size()) ? t[idx++] : "";
        a->referee1.issueDate = (idx < t.size()) ? t[idx++] : "";
        a->referee1.phone = (idx < t.size()) ? t[idx++] : "";
        a->referee1.email = (idx < t.size()) ? t[idx++] : "";

        a->referee2.name = (idx < t.size()) ? t[idx++] : "";
        a->referee2.cnic = (idx < t.size()) ? t[idx++] : "";
        a->referee2.issueDate = (idx < t.size()) ? t[idx++] : "";
        a->referee2.phone = (idx < t.size()) ? t[idx++] : "";
        a->referee2.email = (idx < t.size()) ? t[idx++] : "";

        // final field: status
        a->status = (idx < t.size()) ? t[idx++] : "";

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

    // referees
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

    // status
    ss << a.status;

    return ss.str();
}

// Write vector of unique_ptr<Applicant> back to applications file (overwrite)
bool writeApplications(const string& path, const vector<unique_ptr<Applicant>>& apps) {
    ofstream out(path, ios::trunc);
    if (!out) {
        cerr << "Error: cannot open " << path << " for writing.\n";
        return false;
    }
    for (size_t i = 0; i < apps.size(); ++i) {
        out << applicantToLine(*apps[i]);
        if (i + 1 < apps.size()) out << '\n';
    }
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
    } else {
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
        try { idx = stoi(sel); } catch (...) { idx = -1; }
        if (idx == 0) {
            cout << "Exiting lender interface.\n";
            break;
        }
        if (idx < 1 || idx > (int)apps.size()) {
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
            } else {
                cout << "Status updated and written to file.\n";
            }
        } else {
            cout << "Cancelled.\n";
        }
    }
}

//int main()
//{
//    runLenderInterface();
//    return 0;
//}