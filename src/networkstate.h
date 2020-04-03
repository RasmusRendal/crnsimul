#pragma once

#include <iostream>
#include <map>
#include <string>

#define ZERO_TOLERANCE 0.00001

class NetworkStateOutOfRange : public std::exception {
	virtual const char *what() {
		return "A subtraction brought the value of a species below 0";
	}
};

class NetworkState : public std::map<std::string, double> {
public:
	NetworkState() = default;
	NetworkState DeepCopy();
	void Print();
	std::string PrintCsvHeader();
	std::string PrintCsvRow();
	double time = 0;
	double Diff(const NetworkState &other);
	NetworkState operator+(const NetworkState &);
	NetworkState operator-(const NetworkState &);
};
