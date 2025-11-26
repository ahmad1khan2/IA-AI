#pragma once

#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
using namespace std;

class Loan {
private:
    double installmentPrice;

    void recalcInstallmentPrice() {
        if (installments <= 0) {
            installmentPrice = 0.0;
        }
        else {
            installmentPrice = static_cast<double>(price - down) / static_cast<double>(installments);
        }
    }

protected:
    int installments;
    int price;
    int down;

    Loan(int installments, int price, int down)
        : installments(installments), price(price), down(down) {
        // FIX: Prevent negative financed amount
        if (this->down > this->price) {
            this->down = this->price;
        }
        recalcInstallmentPrice();
    }

public:
    virtual ~Loan() = default;

    // Copy constructor
    Loan(const Loan& other)
        : installments(other.installments), price(other.price), down(other.down),
        installmentPrice(other.installmentPrice) {
    }

    // Getters
    int getInstallments() const { return installments; }
    int getPrice() const { return price; }
    int getDownPayment() const { return down; }
    double getInstallmentPrice() const { return installmentPrice; }

    // Setters
    void setInstallments(int v) { installments = v; recalcInstallmentPrice(); }
    void setPrice(int v) {
        price = v;
        // FIX: Adjust down payment if it exceeds new price
        if (down > price) {
            down = price;
        }
        recalcInstallmentPrice();
    }
    void setDownPayment(int v) {
        // FIX: Prevent down payment from exceeding price
        if (v > price) {
            down = price;
        }
        else {
            down = v;
        }
        recalcInstallmentPrice();
    }

    // Print using cout (not ostream)
    virtual void print() const {
        cout << "Loan:\n";
        cout << "  Price: " << price << '\n';
        cout << "  Down payment: " << down << '\n';
        cout << "  Installments: " << installments << '\n';
        cout << "  Installment price: " << fixed << setprecision(2) << installmentPrice << '\n';
    }

    // Print tabular installment plan
    void printInstallmentPlan() const {
        // First print unique info from derived class
        print();

        // Table header
        cout << '\n' << string(70, '=') << '\n';
        cout << "INSTALLMENT SCHEDULE" << '\n';
        cout << string(70, '=') << '\n';

        double loanAmount = static_cast<double>(price - down);
        int n = installments;

        // FIX: Check for non-positive loan amount
        if (loanAmount <= 0) {
            cout << "NO INSTALLMENT PLAN - FULL PAYMENT COMPLETE\n";
            cout << "Down payment covers the entire amount. No financing required.\n";
            cout << string(70, '=') << '\n';
            return;
        }

        if (n <= 0) {
            cout << "No installments available.\n";
            cout << string(70, '=') << '\n';
            return;
        }

        // FIXED: Calculate fresh each time to avoid precision errors
        double perInstallment = loanAmount / n;

        cout << "Loan Amount: $" << fixed << setprecision(2) << loanAmount << '\n';
        cout << "Number of installments: " << n << '\n';
        cout << "Installment (approx): $" << fixed << setprecision(2) << perInstallment << '\n';
        cout << string(70, '-') << '\n';

        // Column headings
        cout << left
            << setw(8) << "No."
            << setw(12) << "Installment"
            << setw(18) << "Principal Paid"
            << setw(18) << "Remaining" << '\n';
        cout << string(70, '-') << '\n';

        // FIXED: Proper installment calculation
        double remaining = loanAmount;
        double totalPaid = 0.0;

        for (int month = 1; month <= n; ++month) {
            double principalThisMonth;

            if (month == n) {
                // Last month: pay exact remaining balance
                principalThisMonth = remaining;
            }
            else {
                // Regular month: pay equal installment
                principalThisMonth = perInstallment;
            }

            // Ensure we don't pay more than remaining
            if (principalThisMonth > remaining) {
                principalThisMonth = remaining;
            }

            remaining -= principalThisMonth;
            totalPaid += principalThisMonth;

            // FIXED: Round to 2 decimal places for display
            cout << right << setw(4) << month << "   "
                << setw(12) << fixed << setprecision(2) << perInstallment << "   "
                << setw(12) << fixed << setprecision(2) << principalThisMonth << "   "
                << setw(12) << fixed << setprecision(2) << (remaining >= 0.0 ? remaining : 0.0)
                << '\n';

            // Break if fully paid
            if (remaining <= 0.0) break;
        }

        cout << string(70, '-') << '\n';
        cout << "Total principal paid: $" << fixed << setprecision(2) << totalPaid << '\n';
        cout << string(70, '=') << '\n';
    }

    // Print installment plan with an extra "Month" column starting at startMonth.
    void printInstallmentPlanStartingAt(int startMonth) const {
        static const string months[] = {
            "Jan", "Feb", "Mar", "Apr", "May", "Jun",
            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
        };

        if (startMonth <= 0) startMonth = 1;

        // allow derived classes to print their unique info first
        print();

        // Table header
        cout << '\n' << string(80, '=') << '\n';
        cout << "INSTALLMENT SCHEDULE (with Month column)" << '\n';
        cout << string(80, '=') << '\n';

        double loanAmount = static_cast<double>(price - down);
        int n = installments;

        // FIX: Check for non-positive loan amount
        if (loanAmount <= 0) {
            cout << "NO INSTALLMENT PLAN - FULL PAYMENT COMPLETE\n";
            cout << "Down payment covers the entire amount. No financing required.\n";
            cout << string(80, '=') << '\n';
            return;
        }

        if (n <= 0) {
            cout << "No installments available.\n";
            cout << string(80, '=') << '\n';
            return;
        }

        // FIXED: Calculate fresh each time
        double perInstallment = loanAmount / n;

        cout << "Loan Amount: $" << fixed << setprecision(2) << loanAmount << '\n';
        cout << "Number of installments: " << n << '\n';
        cout << "Installment (approx): $" << fixed << setprecision(2) << perInstallment << '\n';
        cout << string(80, '-') << '\n';

        // Column headings include Month
        cout << left
            << setw(8) << "No."
            << setw(10) << "Month"
            << setw(14) << "Installment"
            << setw(18) << "Principal Paid"
            << setw(18) << "Remaining" << '\n';
        cout << string(80, '-') << '\n';

        // FIXED: Proper installment calculation with months
        double remaining = loanAmount;
        double totalPaid = 0.0;

        for (int i = 1; i <= n; ++i) {
            string displayMonth = months[((startMonth - 1) + (i - 1)) % 12];
            double principalThisMonth;

            if (i == n) {
                principalThisMonth = remaining;
            }
            else {
                principalThisMonth = perInstallment;
            }

            if (principalThisMonth > remaining) {
                principalThisMonth = remaining;
            }

            remaining -= principalThisMonth;
            totalPaid += principalThisMonth;

            cout << right << setw(4) << i << "   "
                << setw(6) << displayMonth << "   "
                << setw(12) << fixed << setprecision(2) << perInstallment << "   "
                << setw(12) << fixed << setprecision(2) << principalThisMonth << "   "
                << setw(12) << fixed << setprecision(2) << (remaining >= 0.0 ? remaining : 0.0)
                << '\n';

            if (remaining <= 0.0) break;
        }

        cout << string(80, '-') << '\n';
        cout << "Total principal paid: $" << fixed << setprecision(2) << totalPaid << '\n';
        cout << string(80, '=') << '\n';
    }
};

// ... rest of your loan classes remain the same
class homeLoan : public Loan {
    int area;
    string size;
public:
    homeLoan(int area, string size, int installments, int price, int down)
        : Loan(installments, price, down), area(area), size(size) {
    }

    // Copy constructor
    homeLoan(const homeLoan& other)
        : Loan(other), area(other.area), size(other.size) {
    }

    // Getters
    int getArea() const { return area; }
    const string& getSize() const { return size; }

    // Setters
    void setArea(int a) { area = a; }
    void setSize(const string& s) { size = s; }

    // Print using cout
    void print() const override {
        cout << "HomeLoan:\n";
        Loan::print();
        cout << "  Area: " << area << '\n';
        cout << "  Size: " << size << '\n';
    }
};

class carLoan : public Loan {
    string make;
    string model;
    int engine;
    bool used;
    int year;
public:
    carLoan(const string& make, const string& model, int engine, bool used, int year,
        int installments, int price, int down)
        : Loan(installments, price, down), make(make), model(model), engine(engine),
        used(used), year(year) {
    }

    // Copy constructor
    carLoan(const carLoan& other)
        : Loan(other), make(other.make), model(other.model), engine(other.engine),
        used(other.used), year(other.year) {
    }

    // Getters
    const string& getMake() const { return make; }
    const string& getModel() const { return model; }
    int getEngine() const { return engine; }
    bool isUsed() const { return used; }
    int getYear() const { return year; }

    // Setters
    void setMake(const string& m) { make = m; }
    void setModel(const string& m) { model = m; }
    void setEngine(int e) { engine = e; }
    void setUsed(bool u) { used = u; }
    void setYear(int y) { year = y; }

    // Print using cout
    void print() const override {
        cout << "CarLoan:\n";
        Loan::print();
        cout << "  Make: " << make << '\n';
        cout << "  Model: " << model << '\n';
        cout << "  Engine: " << engine << '\n';
        cout << "  Used: " << (used ? "Yes" : "No") << '\n';
        cout << "  Year: " << year << '\n';
    }
};

class scooterLoan : public Loan {
    string make;
    string model;
    int distancePerChargeKm;
    double chargingTimeHrs;
    int maxSpeedKmH;

public:
    scooterLoan(const string& make, const string& model, int distancePerChargeKm,
        double chargingTimeHrs, int maxSpeedKmH, int installments, int price, int down)
        : Loan(installments, price, down), make(make), model(model),
        distancePerChargeKm(distancePerChargeKm), chargingTimeHrs(chargingTimeHrs),
        maxSpeedKmH(maxSpeedKmH) {
    }

    // Copy constructor
    scooterLoan(const scooterLoan& other)
        : Loan(other), make(other.make), model(other.model),
        distancePerChargeKm(other.distancePerChargeKm),
        chargingTimeHrs(other.chargingTimeHrs), maxSpeedKmH(other.maxSpeedKmH) {
    }

    // Getters
    const string& getMake() const { return make; }
    const string& getModel() const { return model; }
    int getDistancePerChargeKm() const { return distancePerChargeKm; }
    double getChargingTimeHrs() const { return chargingTimeHrs; }
    int getMaxSpeedKmH() const { return maxSpeedKmH; }

    // Setters
    void setMake(const string& m) { make = m; }
    void setModel(const string& m) { model = m; }
    void setDistancePerChargeKm(int d) { distancePerChargeKm = d; }
    void setChargingTimeHrs(double h) { chargingTimeHrs = h; }
    void setMaxSpeedKmH(int s) { maxSpeedKmH = s; }

    // Print using cout
    void print() const override {
        cout << "ScooterLoan:\n";
        Loan::print();
        cout << "  Make: " << make << '\n';
        cout << "  Model: " << model << '\n';
        cout << "  Distance per charge (km): " << distancePerChargeKm << '\n';
        cout << "  Charging time (hrs): " << fixed << setprecision(2) << chargingTimeHrs << '\n';
        cout << "  Max speed (km/h): " << maxSpeedKmH << '\n';
    }
};

class personalLoan : public Loan {
private:
    string loanType;
    int maxAmount;
    double interestRate;     // in percentage (e.g., 15 for 15%)
    int processingFee;

public:
    // Constructor
    personalLoan(const string& loanType, int maxAmount, double interestRate, int processingFee,
        int installments, int price, int down)
        : Loan(installments, price, down),
        loanType(loanType),
        maxAmount(maxAmount),
        interestRate(interestRate),
        processingFee(processingFee)
    {
        // FIX: Additional validation for personal loan
        if (down > price) {
            setDownPayment(price); // Use setter to ensure proper recalculation
        }
    }

    // Copy constructor
    personalLoan(const personalLoan& other)
        : Loan(other),
        loanType(other.loanType),
        maxAmount(other.maxAmount),
        interestRate(other.interestRate),
        processingFee(other.processingFee)
    {
    }

    // Getters
    const string& getLoanType() const { return loanType; }
    int getMaxAmount() const { return maxAmount; }
    double getInterestRate() const { return interestRate; }
    int getProcessingFee() const { return processingFee; }

    // Setters
    void setLoanType(const string& s) { loanType = s; }
    void setMaxAmount(int a) { maxAmount = a; }
    void setInterestRate(double r) { interestRate = r; }
    void setProcessingFee(int f) { processingFee = f; }

    // OVERRIDE: Print details using cout with interest calculation
    void print() const override {
        cout << "PersonalLoan:\n";
        cout << "  Loan Type: " << loanType << '\n';
        cout << "  Total Price: " << getPrice() << '\n';  // FIX: Changed label
        cout << "  Down payment: " << getDownPayment() << '\n';

        // FIX: Calculate and display non-negative financed amount
        int financed = getPrice() - getDownPayment();
        if (financed < 0) financed = 0;
        cout << "  Amount Financed: " << financed << '\n';

        cout << "  Installments: " << getInstallments() << '\n';
        cout << "  Max Amount: " << maxAmount << '\n';
        cout << "  Annual Interest Rate: " << fixed << setprecision(2) << interestRate << "%\n";
        cout << "  Processing Fee: " << processingFee << '\n';
    }

    // OVERRIDE: Installment plan with interest calculation
    void printInstallmentPlan() const {
        print();

        double loanAmount = static_cast<double>(getPrice() - getDownPayment());
        int n = getInstallments();

        // FIX: Enhanced validation for non-positive loan amount
        if (loanAmount <= 0) {
            cout << "\n" << string(80, '=') << '\n';
            if (loanAmount < 0) {
                cout << "INVALID LOAN - DOWN PAYMENT EXCEEDS PRICE\n";
            }
            else {
                cout << "NO INSTALLMENT PLAN - FULL PAYMENT COMPLETE\n";
            }
            cout << string(80, '=') << '\n';
            cout << "Down payment covers the entire amount. No financing required.\n";
            cout << string(80, '=') << '\n';
            return;
        }

        if (n <= 0) {
            cout << "No installments available.\n";
            return;
        }

        // Calculate monthly installment with interest
        double monthlyRate = interestRate / 12 / 100;
        double installment = 0.0;

        if (monthlyRate > 0) {
            // With interest: PMT = P * r * (1+r)^n / ((1+r)^n - 1)
            installment = loanAmount * monthlyRate * pow(1 + monthlyRate, n)
                / (pow(1 + monthlyRate, n) - 1);
        }
        else {
            // No interest: simple division
            installment = loanAmount / n;
        }

        cout << "\n" << string(80, '=') << '\n';
        cout << "INSTALLMENT SCHEDULE (with " << interestRate << "% interest)" << '\n';
        cout << string(80, '=') << '\n';
        cout << "Loan Amount: $" << fixed << setprecision(2) << loanAmount << '\n';
        cout << "Annual Interest Rate: " << interestRate << "%\n";
        cout << "Monthly Installment: $" << fixed << setprecision(2) << installment << '\n';
        cout << "Number of installments: " << n << '\n';
        cout << string(80, '-') << '\n';

        // Column headings
        cout << left
            << setw(8) << "No."
            << setw(12) << "Installment"
            << setw(15) << "Interest Paid"
            << setw(15) << "Principal Paid"
            << setw(15) << "Remaining" << '\n';
        cout << string(80, '-') << '\n';

        double remaining = loanAmount;
        double totalInterest = 0.0;
        double totalPrincipal = 0.0;

        for (int month = 1; month <= n; ++month) {
            double interestThisMonth = remaining * monthlyRate;
            double principalThisMonth;

            if (month == n) {
                // Last month: adjust to pay off exactly
                principalThisMonth = remaining;
            }
            else {
                principalThisMonth = installment - interestThisMonth;
            }

            // Ensure we don't overpay
            if (principalThisMonth > remaining) {
                principalThisMonth = remaining;
            }

            remaining -= principalThisMonth;
            totalInterest += interestThisMonth;
            totalPrincipal += principalThisMonth;

            cout << right << setw(4) << month << "   "
                << setw(10) << fixed << setprecision(2) << installment << "   "
                << setw(12) << fixed << setprecision(2) << interestThisMonth << "   "
                << setw(12) << fixed << setprecision(2) << principalThisMonth << "   "
                << setw(12) << fixed << setprecision(2) << (remaining >= 0.0 ? remaining : 0.0)
                << '\n';

            if (remaining <= 0.0) break;
        }

        cout << string(80, '-') << '\n';
        cout << "Total Interest Paid: $" << fixed << setprecision(2) << totalInterest << '\n';
        cout << "Total Principal Paid: $" << fixed << setprecision(2) << totalPrincipal << '\n';
        cout << "Total Amount Paid: $" << fixed << setprecision(2) << (totalInterest + totalPrincipal) << '\n';
        cout << string(80, '=') << '\n';
    }

    // OVERRIDE: Month-based plan with interest
    void printInstallmentPlanStartingAt(int startMonth) const {
        static const string months[] = {
            "Jan", "Feb", "Mar", "Apr", "May", "Jun",
            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
        };

        if (startMonth <= 0) startMonth = 1;

        print();

        double loanAmount = static_cast<double>(getPrice() - getDownPayment());
        int n = getInstallments();

        // FIX: Enhanced validation for non-positive loan amount
        if (loanAmount <= 0) {
            cout << "\n" << string(80, '=') << '\n';
            if (loanAmount < 0) {
                cout << "INVALID LOAN - DOWN PAYMENT EXCEEDS PRICE\n";
            }
            else {
                cout << "NO INSTALLMENT PLAN - FULL PAYMENT COMPLETE\n";
            }
            cout << string(80, '=') << '\n';
            cout << "Down payment covers the entire amount. No financing required.\n";
            cout << string(80, '=') << '\n';
            return;
        }

        if (n <= 0) {
            cout << "No installments available.\n";
            return;
        }

        // Calculate monthly installment with interest
        double monthlyRate = interestRate / 12 / 100;
        double installment = 0.0;

        if (monthlyRate > 0) {
            installment = loanAmount * monthlyRate * pow(1 + monthlyRate, n)
                / (pow(1 + monthlyRate, n) - 1);
        }
        else {
            installment = loanAmount / n;
        }

        cout << "\n" << string(90, '=') << '\n';
        cout << "INSTALLMENT SCHEDULE (with " << interestRate << "% interest)" << '\n';
        cout << string(90, '=') << '\n';
        cout << "Loan Amount: $" << fixed << setprecision(2) << loanAmount << '\n';
        cout << "Annual Interest Rate: " << interestRate << "%\n";
        cout << "Monthly Installment: $" << fixed << setprecision(2) << installment << '\n';
        cout << "Number of installments: " << n << '\n';
        cout << string(90, '-') << '\n';

        // Column headings with Month
        cout << left
            << setw(6) << "No."
            << setw(8) << "Month"
            << setw(12) << "Installment"
            << setw(14) << "Interest Paid"
            << setw(16) << "Principal Paid"
            << setw(16) << "Remaining" << '\n';
        cout << string(90, '-') << '\n';

        double remaining = loanAmount;
        double totalInterest = 0.0;
        double totalPrincipal = 0.0;

        for (int i = 1; i <= n; ++i) {
            string displayMonth = months[((startMonth - 1) + (i - 1)) % 12];
            double interestThisMonth = remaining * monthlyRate;
            double principalThisMonth;

            if (i == n) {
                principalThisMonth = remaining;
            }
            else {
                principalThisMonth = installment - interestThisMonth;
            }

            if (principalThisMonth > remaining) {
                principalThisMonth = remaining;
            }

            remaining -= principalThisMonth;
            totalInterest += interestThisMonth;
            totalPrincipal += principalThisMonth;

            cout << right << setw(4) << i << "   "
                << setw(6) << displayMonth << "   "
                << setw(10) << fixed << setprecision(2) << installment << "   "
                << setw(12) << fixed << setprecision(2) << interestThisMonth << "   "
                << setw(12) << fixed << setprecision(2) << principalThisMonth << "   "
                << setw(12) << fixed << setprecision(2) << (remaining >= 0.0 ? remaining : 0.0)
                << '\n';

            if (remaining <= 0.0) break;
        }

        cout << string(90, '-') << '\n';
        cout << "Total Interest Paid: $" << fixed << setprecision(2) << totalInterest << '\n';
        cout << "Total Principal Paid: $" << fixed << setprecision(2) << totalPrincipal << '\n';
        cout << "Total Amount Paid: $" << fixed << setprecision(2) << (totalInterest + totalPrincipal) << '\n';
        cout << string(90, '=') << '\n';
    }
};