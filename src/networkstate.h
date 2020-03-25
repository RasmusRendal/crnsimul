#pragma once

#include <iostream>
#include <map>
#include <string>

class NetworkState : public std::map<std::string, double> {
public:
	NetworkState() = default;
	NetworkState DeepCopy();
	void Print();
	std::string PrintCsvHeader();
	std::string PrintCsvRow();
	double time = 0;
	double Diff(const NetworkState &other);
};
