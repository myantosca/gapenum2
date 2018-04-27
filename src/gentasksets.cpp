#include <cstring>
#include <string>
#include <iostream>
#include <random>
#include <chrono>

int main(int argc, char *argv[])
{
     int n = 1;
     int m = 3;
     int a = 1;

     while (a < argc)
     {
	  if (!strcmp(argv[a], "-n"))
	  {
	       // Number of task sets
	       if (argc > ++a) n = std::atoi(argv[a]);
	  }
	  else if (!strcmp(argv[a], "-m"))
	  {
	       // Task set size
	       if (argc > ++a) m = std::atoi(argv[a]);
	  }
	  else
	  {
	       a++;
	  }
     }

     if ((n <= 0) || (m <= 0))
     {
	  std::cout << "Usage: gentasksets [-n <number of task sets>] [-m <task set size>]" << std::endl;
	  return -1;
     }
     // Uniform distribution of task computation times
     uint64_t c_seed = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
     std::knuth_b c_prng(c_seed);
     std::uniform_int_distribution<int> c_distrib(4,9);

     // Uniform distribution of task periods
     uint64_t p_seed = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
     std::knuth_b p_prng(p_seed);
     std::uniform_int_distribution<int> p_distrib(40,59);

     // For n task sets...
     for (int i = 0; i < n; i++)
     {
	  // For m tasks within a set...
	  for (int j = 0; j < m; j++)
	  {
	       // Print a line representing the task set "{r,c,p}", i.e., arrival, computation, period.
	       int r = 0;
	       int c = c_distrib(c_prng);
	       int p = p_distrib(p_prng);
	       std::cout << '{' << r << ',' << c << ',' << p << '}' << std::endl;
	  }
	  // Divide task sets by an empty line.
	  std::cout << std::endl;
     }
     return 0;
}
