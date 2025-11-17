#pragma once
#include <iostream>
#include <string>
using namespace std;

// loan parent class that will have 4 children classes (home, car, scooter, personal)
class Loan {
private:
	double installmentPrice; // cached calculated installment price

	void recalcInstallmentPrice() {
		if (installments <= 0) {
			installmentPrice = 0.0;
		} else {
			installmentPrice = static_cast<double>(price - down) / static_cast<double>(installments);
		}
	}

protected:
	int installments;
	int price;
	int down;

	// protected so only derived classes can construct Loan directly
	Loan(int installments, int price, int down)
		: installments(installments), price(price), down(down) {
		recalcInstallmentPrice();
	}

public:
	virtual ~Loan() = default;

	// Getters
	int getInstallments() const { return installments; }
	int getPrice() const { return price; }
	int getDownPayment() const { return down; }
	double getInstallmentPrice() const { return installmentPrice; }

	// Setters (recalculate cached installment price)
	void setInstallments(int v) { installments = v; recalcInstallmentPrice(); }
	void setPrice(int v) { price = v; recalcInstallmentPrice(); }
	void setDownPayment(int v) { down = v; recalcInstallmentPrice(); }
};

class homeLoan : public Loan
{
	int area;
	string size;
public:
	homeLoan(int area, string size, int installments, int price, int down)
		: Loan(installments, price, down), area(area), size(size) {}

	// Getters
	int getArea() const { return area; }
	const string& getSize() const { return size; }

	// Setters
	void setArea(int a) { area = a; }
	void setSize(const string& s) { size = s; }
};

// Make#Model#Engine#Used#Manufacturing Year#Installments#Price#Down Payment
class carLoan : public Loan
{
	string make;
	string model;
	int engine;
	bool used;
	int year;
public:
	carLoan(const string& make, const string& model, int engine, bool used, int year,
			int installments, int price, int down)
		: Loan(installments, price, down), make(make), model(model), engine(engine), used(used), year(year) {}

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
};

class scooterLoan : public Loan
{
	string make;
	string model;
	int distancePerChargeKm;   // Distance per charge in KM
	double chargingTimeHrs;    // Charging time in hours
	int maxSpeedKmH;           // Max speed in KM/H

public:
	scooterLoan(const string& make, const string& model, int distancePerChargeKm, double chargingTimeHrs, int maxSpeedKmH,
	            int installments, int price, int down)
		: Loan(installments, price, down),
		  make(make), model(model),
		  distancePerChargeKm(distancePerChargeKm),
		  chargingTimeHrs(chargingTimeHrs),
		  maxSpeedKmH(maxSpeedKmH) {}

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
};
