#include "cnic.h"


// Check if CNIC contains exactly 13 digits
bool isValidCNIC(const string& cnic) {
    if (cnic.length() != 13) return false;
    for (char c : cnic) {
        if (!isdigit(c)) return false;
    }
    return true;
}

// Count applications for a given CNIC
void countApplicationsByCNIC(const string& cnic) {
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
        // Split line by #
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
        fields[idx] = temp; // last field

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

    cout << "\n--- Application Count for CNIC: " << cnic << " ---\n";
    cout << "Submitted: " << submitted << endl;
    cout << "Approved:  " << approved << endl;
    cout << "Rejected:  " << rejected << endl;
}

// Ask user for CNIC and call count function
void queryApplicationsByCNIC() {
    string cnic;

    while (true) {
        cout << "Enter CNIC to query (13 digits, without dashes): ";
        getline(cin, cnic);

        if (isValidCNIC(cnic)) {
            break;
        }
        else {
            cout << "Error: CNIC must be 13 digits without dashes. Please try again.\n\n";
        }
    }

    countApplicationsByCNIC(cnic);

    // show menu again
    cout << "\n\nPress A to apply for a loan, C to check another CNIC, or X to exit.\n";
}
