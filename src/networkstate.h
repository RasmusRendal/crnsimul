#pragma once

#include <iostream>
#include <map>
#include <string>

using namespace std;

class NetworkState : public map<string, double> {
public:
	NetworkState() : map<string, double>() {}
	NetworkState DeepCopy() {
		NetworkState copy;
		for (auto &specie : *this) {
			copy.insert(specie);
		}
		return copy;
	}
	void Print() {
		for (auto &specie : *this) {
			std::cout << specie.first << ": " << specie.second << std::endl;
		}
	}
	void PrintCsvHeader() {
		for (auto &specie : *this) {
			std::cout << specie.first << ",";
		}
		std::cout << std::endl;
	}
	void PrintCsvRow() {
		for (auto specie : *this) {
			std::cout << specie.second << ",";
		}
		std::cout << std::endl;
	}
};
