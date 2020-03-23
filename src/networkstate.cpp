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
	result += "t,";
	for (auto &specie : *this) {
		if (specie.first == last) {
			result += specie.first + "\n";
		} else {
			result += specie.first + ",";
		}
	}
	return result;
}

string NetworkState::PrintCsvRow() {
	string last = rbegin()->first;
	string result;
	result += to_string(time) + ",";
	for (auto specie : *this) {
		if (specie.first == last) {
			result += to_string(specie.second) + "\n";
		} else {
			result += to_string(specie.second) + ",";
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
