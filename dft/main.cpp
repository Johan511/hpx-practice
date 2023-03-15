#include <hpx/local/algorithm.hpp>
#include <hpx/local/init.hpp>
#include <iostream>
#include <cmath>

// #define DEBUG

#define N 1'000'000

typedef double(F_)(double);

double integrate(F_ &&f, double begin, double end, int numPartitions = N)
{
    double step = (end - begin) / numPartitions;
    std::vector<hpx::future<double>> results_f;

    for (int i = 0; i < numPartitions; i++)
    {
        results_f.push_back(hpx::async([step, f](double j)
                                       { return step * f((j + 0.5) * step); },
                                       i));
    }

    double area = 0;

    for (hpx::future<double> &x : results_f)
        area += x.get();

    return area;
}

#ifdef DEBUG

int hpx_main(hpx::program_options::variables_map &vm)
{
    double area = integrate(cos,
                            0, 3.14 / 2);
    std::cout << area << std::endl;
    return hpx::local::finalize();
}

int main(int argc, char *argv[])
{
    using namespace hpx::program_options;
    options_description cmdline("usage: " HPX_APPLICATION_STRING " [options]");
    hpx::local::init_params init_args;
    init_args.desc_cmdline = cmdline;

    return hpx::local::init(hpx_main, argc, argv, init_args);
}

#endif