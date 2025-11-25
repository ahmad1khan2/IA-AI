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
    void setPrice(int v) { price = v; recalcInstallmentPrice(); }
    void setDownPayment(int v) { down = v; recalcInstallmentPrice(); }

    // Print using cout (not ostream)
    virtual void print() const {
        cout << "Loan:\n";
        cout << "  Price: " << price << '\n';
        cout << "  Down payment: " << down << '\n';
        cout << "  Installments: " << installments << '\n';
        cout << "  Installment price: " << fixed << setprecision(2) << installmentPrice << '\n';
    }

    // Print tabular installment plan. Calls virtual print() first so derived classes
    // can display their unique information, then prints a numbered schedule.
    void printInstallmentPlan() const {
        // First print unique info from derived class
        print();

        // Table header
        cout << '\n' << string(70, '=') << '\n';
        cout << "INSTALLMENT SCHEDULE" << '\n';
        cout << string(70, '=') << '\n';

        double loanAmount = static_cast<double>(price - down);
        int n = installments;
        if (n <= 0) {
            cout << "No installments available.\n";
            cout << string(70, '=') << '\n';
            return;
        }

        // Use cached installmentPrice (already calculated and rounded display handled below)
        double perInstallment = installmentPrice;

        cout << "Loan Amount: $" << fixed << setprecision(2) << loanAmount << '\n';
        cout << "Number of installments: " << n << '\n';
        cout << "Installment (approx): $" << fixed << setprecision(2) << perInstallment << '\n';
        cout << string(70, '-') << '\n';

        // Column headings
        cout << left
             << setw(8)  << "No."
             << setw(12) << "Installment"
             << setw(18) << "Principal Paid"
             << setw(18) << "Remaining" << '\n';
        cout << string(70, '-') << '\n';

        // Schedule calculation
        double remaining = loanAmount;
        double totalPaid = 0.0;

        for (int month = 1; month <= n; ++month) {
            double principalThisMonth;
            if (month == n) {
                // last month: pay whatever remains (to handle rounding)
                principalThisMonth = remaining;
            }
            else {
                principalThisMonth = perInstallment;
            }

            // Ensure we don't produce tiny negative values due to floating point
            if (principalThisMonth < 0.0) principalThisMonth = 0.0;

            remaining -= principalThisMonth;
            if (remaining < 0.0 && fabs(remaining) < 0.005) remaining = 0.0; // small epsilon fix

            totalPaid += principalThisMonth;

            cout << right << setw(4) << month << "   "
                 << setw(12) << fixed << setprecision(2) << perInstallment << "   "
                 << setw(12) << fixed << setprecision(2) << principalThisMonth << "   "
                 << setw(12) << fixed << setprecision(2) << (remaining > 0.0 ? remaining : 0.0)
                 << '\n';
        }

        cout << string(70, '-') << '\n';
        cout << "Total principal paid: $" << fixed << setprecision(2) << totalPaid << '\n';
        cout << string(70, '=') << '\n';
    }

    // Print installment plan with an extra "Month" column starting at startMonth.
    // startMonth <= 0 will be treated as 1.
    void printInstallmentPlanStartingAt(int startMonth) const {
        static const string months[] = {
            "Jan", "Feb", "Mar", "Apr", "May", "Jun",
            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec",
            "Jan", "Feb", "Mar", "Apr", "May", "Jun",
            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
		};
        // normalize startMonth
        if (startMonth <= 0) startMonth = 1;

        // allow derived classes to print their unique info first
        print();

        // Table header
        cout << '\n' << string(80, '=') << '\n';
        cout << "INSTALLMENT SCHEDULE (with Month column)" << '\n';
        cout << string(80, '=') << '\n';

        double loanAmount = static_cast<double>(price - down);
        int n = installments;
        if (n <= 0) {
            cout << "No installments available.\n";
            cout << string(80, '=') << '\n';
            return;
        }

        double perInstallment = installmentPrice;

        cout << "Loan Amount: $" << fixed << setprecision(2) << loanAmount << '\n';
        cout << "Number of installments: " << n << '\n';
        cout << "Installment (approx): $" << fixed << setprecision(2) << perInstallment << '\n';
        cout << string(80, '-') << '\n';

        // Column headings include Month
        cout << left
             << setw(8)  << "No."
             << setw(10) << "Month"
             << setw(14) << "Installment"
             << setw(18) << "Principal Paid"
             << setw(18) << "Remaining" << '\n';
        cout << string(80, '-') << '\n';

        // Schedule calculation
        double remaining = loanAmount;
        double totalPaid = 0.0;

        for (int i = 1; i <= n; ++i) {
            string displayMonth = months[((startMonth-1) + (i - 1))%12];
            double principalThisMonth;
            if (i == n) {
                principalThisMonth = remaining;
            } else {
                principalThisMonth = perInstallment;
            }

            if (principalThisMonth < 0.0) principalThisMonth = 0.0;

            remaining -= principalThisMonth;
            if (remaining < 0.0 && fabs(remaining) < 0.005) remaining = 0.0;

            totalPaid += principalThisMonth;

            cout << right << setw(4) << i << "   "
                 << setw(6) << displayMonth << "   "
                 << setw(12) << fixed << setprecision(2) << perInstallment << "   "
                 << setw(12) << fixed << setprecision(2) << principalThisMonth << "   "
                 << setw(12) << fixed << setprecision(2) << (remaining > 0.0 ? remaining : 0.0)
                 << '\n';
        }

        cout << string(80, '-') << '\n';
        cout << "Total principal paid: $" << fixed << setprecision(2) << totalPaid << '\n';
        cout << string(80, '=') << '\n';
    }
};

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

class personalLoan : public Loan {//for personal loan price will be entered by the user validated to be less than maxAmount
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

    // Print details using cout
    void print() const override {
        cout << "PersonalLoan:\n";
        Loan::print();
        cout << "  Loan Type: " << loanType << '\n';
        cout << "  Max Amount: " << maxAmount << '\n';
        cout << "  Interest Rate: " << fixed << setprecision(2) << interestRate << "%\n";
        cout << "  Processing Fee: " << processingFee << '\n';
    }
};
