#include <hpx/local/algorithm.hpp>
#include <hpx/local/init.hpp>
#include <hpx/local/numeric.hpp>

#include <iostream>
#include <cmath>
#include <complex>

#define DEBUG

// #define N 1'000'000

extern std::vector<double> IR;
extern std::vector<double> Red;

const int PI = acos(0);

typedef double(F_)(double);

double get_weight(const std::vector<double> &signal, int scaledFreq)
{
    /*
        W[k] = \sum An e ^ (2 pi * k * n / N)
        we parallelise the exponentiation and use reduction to add all of them up
    */

    std::vector<hpx::future<std::pair<double, double>>> coilPositions;
    coilPositions.reserve(signal.size());

    for (std::size_t i = 0; i < signal.size(); i++)
    {
        coilPositions.emplace_back(hpx::async([i, signal, scaledFreq]()
                                              { return std::make_pair(signal[i] * cos(2 * PI * scaledFreq * i / signal.size()),
                                                                      signal[i] * sin(2 * PI * scaledFreq * i / signal.size())); }));
    }

    double x = 0, y = 0;

    for (std::size_t i = 0; i < coilPositions.size(); i++)
    {
        std::pair<double, double> p = coilPositions[i].get();
        x += p.first;
        y += p.second;
    }

    return sqrt(x * x + y * y);
}

std::vector<hpx::future<double>> fft(const std::vector<double> &signal)
{
    std::vector<hpx::future<double>> freqComp;
    freqComp.reserve(signal.size());
    for (int i = 0; i < signal.size(); i++)
        freqComp.emplace_back(hpx::async(get_weight, signal, i));

    return std::move(freqComp);
}

#ifdef DEBUG

int hpx_main(hpx::program_options::variables_map &vm)
{
    std::vector<hpx::future<double>> IRFFT = fft(IR);
    std::vector<double> v(IRFFT.size());
    for (int i = 0; i < IRFFT.size(); i++)
    {
        v[i] = IRFFT[i].get();
    }
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