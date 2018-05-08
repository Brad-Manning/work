#include <iostream>
#include <random>

int main()
{
  const int nrolls = 100000;
  const int nstars = 100;

  std::default_random_engine generator;
  std::poisson_distribution<int> distribution(0.01);

  int p[10]={};

  for (int i=0; i<nrolls; ++i) {
    int number = distribution(generator);
    if (number < 3) ++p[number];
  }

  std::cout << "poisson distribution (mean=0.0001):" << std::endl;

  for (int i=0; i<10; ++i)
    std::cout << i << ": " << std::string(p[i]*nstars/nrolls,'*') << std::endl;
  return 0;
}
