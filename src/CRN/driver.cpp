#include "driver.h"
#include "CRNParse.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

driver::driver() : trace_parsing(false), trace_scanning(false) {}

int driver::parse_file(char *f) {
	std::ifstream t(f);
	if (!t.good()) {
		std::cout << "File not found" << std::endl;
		;
		return 1;
	}
	std::stringstream buffer;
	buffer << t.rdbuf();
	return parse_string(buffer.str());
}

int driver::parse_string(const std::string &f) {
	scan_begin(f);
	return parse();
}

int driver::parse() {
	yy::parser parse(*this);
	parse.set_debug_level(trace_parsing);
	int res = parse();
	scan_end();
	return res;
}
