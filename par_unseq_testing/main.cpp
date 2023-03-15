////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2021 Dimitra Karatza
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <hpx/local/algorithm.hpp>
#include <hpx/local/execution.hpp>
#include <hpx/local/init.hpp>
#include <hpx/local/numeric.hpp>
// #include <hpx/modules/iterator_support.hpp>

#include <cstddef>
#include <iostream>
#include <random>
#include <vector>

#define N 1'000'000'000

void hpx_for_each_par_unseq()
{
    std::vector<int> v(N, 0);
    hpx::for_each(
        hpx::execution::unseq, v.begin(), v.end(), [](int &r)
        { return r++; });
}

void hpx_for_each_n_par_unseq()
{
    std::vector<int> v(N, 0);
    hpx::for_each_n(
        hpx::execution::par_unseq, v.begin(), 100, [](int &r)
        { return r++; });
}

void hpx_transform_reduce_par_unseq()
{
    std::vector<double> xvalues(10007);
    std::vector<double> yvalues(10007);
    std::fill(std::begin(xvalues), std::end(xvalues), 1.0);
    std::fill(std::begin(yvalues), std::end(yvalues), 1.0);

    double result = hpx::transform_reduce(hpx::execution::par_unseq,
                                          hpx::util::counting_iterator<size_t>(0),
                                          hpx::util::counting_iterator<size_t>(10007), 0.0, std::plus<double>(),
                                          [&xvalues, &yvalues](size_t i)
                                          { return xvalues[i] * yvalues[i]; });
    // print the result
}

void hpx_transform_par_unseq()
{
    std::vector<int> v1(N, 0);
    std::vector<int> v2(N, 0);

    hpx::transform(hpx::execution::par_unseq, v1.begin(), v1.end(), v2.begin(), [](int &i)
                   { return i++; });
}

void hpx_adjacent_difference_par_unseq()
{
    std::vector<int> v(N, 5);
    std::vector<int> diff(N, 12);

    hpx::adjacent_difference(hpx::execution::par_unseq, v.begin(), v.end(), diff.begin(), std::plus<int>());
}

void hpx_rotate_par_unseq()
{
    std::vector<int> v(N);

    hpx::rotate(hpx::execution::par_unseq, v.begin(), v.begin() + 10, v.end());
}

void hpx_adj_find_par_unseq()
{
    std::vector<int> v(N);

    hpx::adjacent_find(hpx::execution::par_unseq, v.begin(), v.end(), std::greater<int>());
}

void hpx_all_of_par_unseq()
{
    std::vector<int> v(N);

    hpx::all_of(hpx::execution::par_unseq, v.begin(), v.end(), [](int &r)
                { return r % 2 == 0; });
}

void hpx_any_of_par_unseq()
{
    std::vector<int> v(N);

    hpx::any_of(hpx::execution::par_unseq, v.begin(), v.end(), [](int &r)
                { return r % 2 == 0; });
}

void hpx_none_of_par_unseq()
{
    std::vector<int> v(N);

    hpx::none_of(hpx::execution::par_unseq, v.begin(), v.end(), [](int &r)
                 { return r % 2 == 0; });
}

void hpx_copy_par_unseq()
{
    std::vector<int> v1(N, 0);
    std::vector<int> v2(N, 0);

    hpx::copy(hpx::execution::par, v1.begin(), v1.end(), v2.begin());
}

void hpx_copy_if_par_unseq()
{
    std::vector<int> v1(N, 0);
    std::vector<int> v2(N, 0);

    hpx::copy_if(hpx::execution::par_unseq, v1.begin(), v1.end(), v2.begin(), [](int r)
                 { return r % 2 == 0; });
}

void hpx_move_par_unseq()
{
    std::vector<int> v1(N, 0);
    std::vector<int> v2(N, 1);

    hpx::move(hpx::execution::par, v1.begin(), v1.end(), v2.begin());
}

void hpx_count_par_unseq()
{
    std::vector<int> v1(N, 0);

    int c = hpx::count(hpx::execution::par_unseq, v1.begin(), v1.end(), 0);
    assert(c == N);
}

void hpx_equal_par_unseq()
{
    std::vector<int> v1(N, 0);
    std::vector<int> v2(N, 0);

    auto eq = hpx::equal(hpx::execution::par_unseq, v1.begin(), v1.end(), v2.begin(), v2.end());
    assert(eq == true);
}

void hpx_exclusive_scan_par_unseq()
{
    std::vector<int> v(N, 0);
    std::vector<int> scan(N, 0);

    hpx::exclusive_scan(hpx::execution::par_unseq, v.begin(), v.end(), scan.begin(), 0, std::plus<>{});
}

void hpx_inclusive_scan_par_unseq()
{
    std::vector<int> v(N, 0);
    std::vector<int> scan(N, 0);

    hpx::inclusive_scan(hpx::execution::par_unseq, v.begin(), v.end(), scan.begin(),
                        std::multiplies<>{});
    // hpx::inclusive_scan(hpx::execution::par_unseq, v.begin(), v.end(), scan.begin(), 0, std::plus<int>());
}

void hpx_fill_par_unseq()
{
    std::vector<int> v(N, 0);

    hpx::fill(hpx::execution::par_unseq, v.begin(), v.end(), 2);
}

void hpx_generate_par_unseq()
{
    std::vector<int> v(N, 0);
    hpx::generate(
        hpx::execution::par_unseq, v.begin(), v.end(), []()
        { return 0; });
}

void hpx_generate_n_par_unseq()
{
    std::vector<int> v(N, 0);
    hpx::generate_n(
        hpx::execution::par_unseq, v.begin(), 200, []()
        { return 0; });
}

void hpx_is_heap()
{
    std::vector<int> v(N, 0);
    hpx::is_heap(hpx::execution::par_unseq, v.begin(), v.end());
}

void hpx_is_sorted()
{
    std::vector<int> v(N, 0);
    hpx::is_sorted(hpx::execution::par_unseq, v.begin(), v.end());
}

void hpx_is_partitioned()
{
    std::vector<int> v(N, 0);
    hpx::is_partitioned(hpx::execution::par_unseq, v.begin(), v.end(), [](int i)
                        { return i % 2; });
}

void hpx_lexicographical_compare()
{
    std::vector<int> v1(N, 0);
    std::vector<int> v2(N, 0);

    hpx::lexicographical_compare(hpx::execution::par_unseq, v1.begin(), v1.end(), v2.begin(), v2.end());
}

void hpx_min_element()
{
    std::vector<int> v(N, 0);

    hpx::min_element(hpx::execution::par_unseq, v.begin(), v.end());
}

void hpx_make_heap()
{
    std::vector<int> v(N, 0);

    hpx::make_heap(hpx::execution::par_unseq, v.begin(), v.end());
}

void hpx_partial_sort()
{
    std::vector<int> v(N, 0);
    hpx::partial_sort(hpx::execution::par_unseq, v.begin(), v.begin() + 100, v.end());
}

void hpx_partial_sort_copy()
{
    std::vector<int> v1(N, 0);
    std::vector<int> v2(N, 0);

    hpx::partial_sort_copy(hpx::execution::par_unseq, v1.begin(), v1.end(), v2.begin(), v2.end());
}

void hpx_sort()
{
    std::vector<int> v(N, 0);

    hpx::sort(hpx::execution::par_unseq, v.begin(), v.end());
}

int hpx_main(hpx::program_options::variables_map &vm)
{
    // hpx_move_par_unseq();

    hpx_copy_par_unseq();

    // hpx_sort();
    // hpx_partial_sort_copy();

    // hpx_partial_sort();

    // hpx_make_heap();
    // hpx_min_element();

    // hpx_lexicographical_compare();

    // hpx_is_partitioned();

    // hpx_is_sorted();

    // hpx_is_heap();

    // hpx_generate_n_par_unseq();
    hpx_generate_par_unseq();

    // hpx_fill_par_unseq();

    // hpx_transform_par_unseq();
    // hpx_transform_reduce_par_unseq();

    // hpx_exclusive_scan_par_unseq();
    // hpx_inclusive_scan_par_unseq();

    // hpx_equal_par_unseq();

    // hpx_count_par_unseq();

    // hpx_move_par_unseq();

    // hpx_copy_par_unseq();
    // hpx_copy_if_par_unseq();

    // hpx_all_of_par_unseq();
    // hpx_any_of_par_unseq();
    // hpx_none_of_par_unseq();

    // hpx_adj_find_par_unseq();
    // hpx_for_each_n_par_unseq();
    // hpx_for_each_par_unseq();

    return hpx::local::finalize();
}
//]

///////////////////////////////////////////////////////////////////////////////
//[mul_main
int main(int argc, char *argv[])
{
    using namespace hpx::program_options;
    options_description cmdline("usage: " HPX_APPLICATION_STRING " [options]");
    hpx::local::init_params init_args;
    init_args.desc_cmdline = cmdline;

    return hpx::local::init(hpx_main, argc, argv, init_args);
}
//]