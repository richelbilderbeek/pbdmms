#include "sado_simulation.h"
#include "kewe_parameters.h"

void create_testrun_file(const std::string& filename)
{
  std::ofstream f(filename);
  f
    << "alleles 2 2 2\n"
    << "histbin 0.1 0.1 0.1 0.1\n"
    << "seed 123\n"
    << "pop0 100\n"
    << "type0 0.5 0.5 0.5\n"
    << "end 4000\n"
    << "sc 0.3\n"
    << "se 0.1\n"
    << "sk 1.2\n"
    << "c 0.01\n"
    << "sm 0.1\n"
    << "sv 0.02\n"
    << "sq 1.0\n"
    << "eta 1.0\n"
    << "b 4.0\n"
    << "output 10 testbestand\n"
    << "diploid 1\n"
  ;
}

int main(int argc, char *argv[])
{
  std::string filename = "testrun.txt";
  if (argc == 1)
  {
    create_testrun_file(filename);
  }
  else
  {
    filename = std::string(argv[1]);
  }

  return sado::do_simulation(filename);
}
