#include "driver.h"
#include <iostream>
#include <string>

void PrintConcen(driver drv) {
	for (auto &conc : drv.Concentration)
		std::cout << "Value of " << conc.first << " is " << conc.second << '\n';
}

int main(int argc, char *argv[]) {
	int res = 0;
	driver drv;
	for (int i = 1; i < argc; ++i)
		if (argv[i] == std::string("-p"))
			drv.trace_parsing = true;
		else if (argv[i] == std::string("-s"))
			drv.trace_scanning = true;
		else if (!drv.parse_file(argv[i]))
			PrintConcen(drv);
		else
			res = 1;
	return res;
}
