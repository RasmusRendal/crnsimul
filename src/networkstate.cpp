#include "networkstate.h"

NetworkState NetworkState::DeepCopy() {
	NetworkState copy;
	for (auto &specie : *this) {
		copy.insert(specie);
	}
	return copy;
}

void NetworkState::Print() {
	for (auto &specie : *this) {
		std::cout << specie.first << ": " << specie.second << std::endl;
	}
}

string NetworkState::PrintCsvHeader() {
	string last = rbegin()->first;
	string result;
	for (auto &specie : *this) {
		if(specie.first == last) {
			result += specie.first+"_x,"+specie.first+"_y\n";
		}
		else {
			result += specie.first+"_x,"+specie.first+"_y,";
		}
	}
	return result;
}

string NetworkState::PrintCsvRow(int iterations, double stepSize) {
	string last = rbegin()->first;
	string result;
	double xValue = iterations*stepSize;
	for (auto specie : *this) {
		if(specie.first == last) {
			result += to_string(xValue)+","+to_string(specie.second)+"\n";
		}
		else {
			result += to_string(xValue)+","+to_string(specie.second)+",";
		}
	}
	return result;
}

double NetworkState::Diff(const NetworkState &other) {
	double diff = 0;
	for (auto &pair : other) {
		if (find(pair.first) == end()) {
			diff += pair.second;
		} else {
			diff += pair.second - at(pair.first);
		}
	}
	for (auto &pair : *this) {
		if (other.find(pair.first) == other.end()) {
			diff += pair.second;
		}
	}
	return diff;
}
