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

void NetworkState::PrintCsvHeader() {
	for (auto &specie : *this) {
		std::cout << specie.first << ",";
	}
	std::cout << std::endl;
}

void NetworkState::PrintCsvRow() {
	for (auto specie : *this) {
		std::cout << specie.second << ",";
	}
	std::cout << std::endl;
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
