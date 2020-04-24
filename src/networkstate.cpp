#include "networkstate.h"
#include <cmath>

NetworkState NetworkState::DeepCopy() {
	NetworkState copy;
	for (const auto &specie : *this) {
		copy.insert(specie);
	}
	return copy;
}

void NetworkState::Print() {
	for (const auto &specie : *this) {
		std::cout << specie.first << ": " << specie.second << std::endl;
	}
}

std::string NetworkState::PrintCsvHeader() {
	std::string last = rbegin()->first;
	std::string result;
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

std::string NetworkState::PrintCsvRow() {
	std::string last = rbegin()->first;
	std::string result;
	result += std::to_string(time) + ",";
	for (const auto &specie : *this) {
		if (specie.first == last) {
			result += std::to_string(specie.second) + "\n";
		} else {
			result += std::to_string(specie.second) + ",";
		}
	}
	return result;
}

double NetworkState::Diff(const NetworkState &other) {
	double diff = 0;
	for (auto &pair : other) {
		if (find(pair.first) == end()) {
			diff += std::abs(pair.second);
		} else {
			diff += std::abs(pair.second - at(pair.first));
		}
	}
	for (auto &pair : *this) {
		if (other.find(pair.first) == other.end()) {
			diff += std::abs(pair.second);
		}
	}
	return diff;
}

NetworkState NetworkState::operator+(const NetworkState &other) {
	NetworkState state;
	for (const auto &specie : *this) {
		std::string name = specie.first;
		auto otherVal = other.find(specie.first);
		double value = specie.second + otherVal->second;
		auto pair = std::pair<std::string, double>(name, value);
		state.insert(pair);
	}

	for (const auto &specie : other) {
		if (find(specie.first) == end()) {
			state.insert(specie);
		}
	}
	return state;
}

NetworkState NetworkState::operator-(const NetworkState &other) {
	NetworkState state;
	for (const auto &specie : *this) {
		std::string name = specie.first;
		double value = specie.second - other.find(specie.first)->second;
		if (value < 0) {
			if (value > -ZERO_TOLERANCE) {
				value = 0;
			} else {
				throw NetworkStateOutOfRange();
			}
		}
		auto pair = std::pair<std::string, double>(name, value);
		state.insert(pair);
	}

	for (const auto &specie : other) {
		if (specie.second != 0 && find(specie.first) == end()) {
			throw NetworkStateOutOfRange();
		}
	}
	return state;
}
