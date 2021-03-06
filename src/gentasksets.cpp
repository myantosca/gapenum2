#include <cstring>
#include <string>
#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include <vector>

#include "task.h"

int main(int argc, char *argv[])
{
     int n = 1;
     int m = 3;
     int a = 1;

     while (a < argc)
     {
	  if (!strcmp(argv[a], "-m"))
	  {
	       // Number of task sets
	       if (argc > ++a) m = std::atoi(argv[a]);
	  }
	  else if (!strcmp(argv[a], "-n"))
	  {
	       // Task set size
	       if (argc > ++a) n = std::atoi(argv[a]);
	  }
	  else
	  {
	       a++;
	  }
     }

     if ((n <= 0) || (m <= 0))
     {
	  std::cout << "Usage: gentasksets [-m <number of task sets>] [-n <task set size>]" << std::endl;
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

     // For m task sets...
     for (int i = 0; i < m; i++)
     {
	  std::vector<task_t> tasks;
	  // For n tasks within a set...
	  for (int j = 0; j < n; j++)
	  {
	       tasks.push_back({0, c_distrib(c_prng), p_distrib(p_prng)});
	  }
	  std::sort(tasks.begin(), tasks.end(),
		    [&](task_t a, task_t b){ return a.p == b.p ? a.c < b.c : a.p > b.p; });
	  // Header indicating task set size.
	  std::cout << n << ':';
	  for (auto iter = tasks.begin(); iter < tasks.end(); iter++)
	  {
	       // Print a line representing the task set "{r,c,p}", i.e., arrival, computation, period.
	       std::cout << '{' << iter->r << ',' << iter->c << ',' << iter->p << '}';
	  }
	  // One task set per line.
	  std::cout << std::endl;
     }
     return 0;
}
