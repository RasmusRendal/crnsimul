#pragma once

#include <iostream>
#include <map>
#include <string>

using namespace std;

class NetworkState : public map<string, double> {
public:
	NetworkState() : map<string, double>() {}
	NetworkState DeepCopy();
	void Print();
	void PrintCsvHeader();
	void PrintCsvRow();
	double Diff(const NetworkState &other);
};
