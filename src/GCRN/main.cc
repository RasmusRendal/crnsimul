#include <iostream>
#include "driver.hh"

void PrintConcen(driver drv)
{
  for(auto& conc : drv.Concentration)
    std::cout << "Value of " << conc.first << " is " << conc.second << '\n';
  
  for(auto& Spec : drv.Specie)
    std::cout << "Specie " << Spec.second << Spec.first << '\n';
}

int main (int argc, char *argv[])
{
  int res = 0;
  driver drv;
  for (int i = 1; i < argc; ++i)
    if (argv[i] == std::string ("-p"))
      drv.trace_parsing = true;
    else if (argv[i] == std::string ("-s"))
      drv.trace_scanning = true;
    else if (!drv.parse (argv[i]))
      PrintConcen(drv);
      //std::cout << drv.result << '\n';
    else
      res = 1;
  return res;
}
