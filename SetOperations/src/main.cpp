#include <assert.h>
#include <hpx/local/algorithm.hpp>
#include <hpx/local/execution.hpp>
#include <hpx/local/init.hpp>
#include <hpx/local/numeric.hpp>
#include <random>
// #include <hpx/modules/iterator_support.hpp>
#include <cstddef>
#include <hpx/local/init.hpp>
#include <hpx/modules/testing.hpp>
#include <hpx/parallel/algorithms/set_difference.hpp>
#include <hpx/parallel/algorithms/set_intersection.hpp>
#include <iostream>
#include <random>
#include <vector>

#include <algorithm>
#include <assert.h>
#include <execution>
#include <random>
#include <string>
#include <vector>

struct RandomIntInRange {
  int rangeMin, rangeMax;
  RandomIntInRange(int rangeMin, int rangeMax)
      : rangeMin(rangeMin), rangeMax(rangeMax){};
  int operator()() { return (rand() % (rangeMax - rangeMin + 1)) + rangeMin; }
};

void set_difference_randomized(int rounds, int maxLen) {
  while (rounds--) {
    std::size_t len_a = rand() % maxLen, len_b = rand() % maxLen;
    std::cout << "Round : " << rounds << " ";
    std::cout << "len_a : " << len_a << " "
              << "len_b : " << len_b << std::endl;
    std::vector<int> set_a(len_a), set_b(len_b);

    int rangeMin = 0;
    int rangeMax = std::min(len_a, len_b) * 2;

#ifdef HPX_WITH_CXX17_STD_EXECUTION_POLICES
    std::generate(std::execution::par_unseq, set_a.begin(), set_a.end(),
                  RandomIntInRange(rangeMin, rangeMax));
    std::generate(std::execution::par_unseq, set_b.begin(), set_b.end(),
                  RandomIntInRange(rangeMin, rangeMax));
#else
    std::generate(set_a.begin(), set_a.end(),
                  RandomIntInRange(rangeMin, rangeMax));
    std::generate(set_b.begin(), set_b.end(),
                  RandomIntInRange(rangeMin, rangeMax));
#endif
    std::sort(set_a.begin(), set_a.end());
    std::sort(set_b.begin(), set_b.end());

    len_a = std::unique(set_a.begin(), set_a.end()) - set_a.begin();
    len_b = std::unique(set_b.begin(), set_b.end()) - set_b.begin();

    set_a.resize(len_a);
    set_b.resize(len_b);

    // rand always gives non negative values, rangeMin >= 0
    std::vector<int> perfect(std::max(len_a, len_b), -1);
    std::vector<int> a_minus_b(std::max(len_a, len_b), -1);

    std::set_difference(set_a.begin(), set_a.end(), set_b.begin(), set_b.end(),
                        perfect.begin());

    hpx::set_difference(hpx::execution::par, set_a.begin(), set_a.end(),
                        set_b.begin(), set_b.end(), a_minus_b.begin());
    assert(perfect == a_minus_b);
  }
}

int hpx_main(hpx::program_options::variables_map &vm) {
  set_difference_randomized(100000, 1 << 3);
  set_difference_randomized(1000, 1 << 10);
  set_difference_randomized(1000, 1 << 20);
  return hpx::local::finalize();
}

int main(int argc, char *argv[]) {
  using namespace hpx::program_options;
  options_description cmdline("usage: " HPX_APPLICATION_STRING " [options]");
  hpx::local::init_params init_args;
  init_args.desc_cmdline = cmdline;

  return hpx::local::init(hpx_main, argc, argv, init_args);
}
