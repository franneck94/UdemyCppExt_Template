// #include <algorithm>
// #include <assert.h>
// #include <chrono>
// #include <cstdint>
// #include <iostream>
// #include <random>
// #include <thread>
// #include <vector>

// #include "Timer.hpp"
// #include "omp.h"
// #include "utils.hpp"

// constexpr std::uint32_t NUM_THREADS = 8;
// constexpr std::uint32_t NUM_RUNS = 100;

// std::int32_t serial_sum(std::vector<std::int32_t> &vec)
// {
//     std::int32_t sum = 0;
//     std::int32_t n = static_cast<std::int32_t>(vec.size());

//     for (std::int32_t i = 0; i != n; ++i)
//     {
//         sum = sum + vec[i];
//     }

//     return sum;
// }

// std::int32_t parallel_sum_omp(std::vector<std::int32_t> &vec)
// {
//     std::int32_t final_sum = 0;
//     std::int32_t sum = 0;
//     std::int32_t i = 0;
//     std::int32_t n = static_cast<std::int32_t>(vec.size());

// #pragma omp parallel for reduction(+ : sum) num_threads(NUM_THREADS)
//     for (i = 0; i < n; ++i)
//     {
//         sum = sum + vec[i];
//     }
// #pragma omp critical
//     {
//         final_sum += sum;
//     }

//     return final_sum;
// }

/*
   Serial time in ms: 2.70182
2: OpenMP time in ms: 1.75325
4: OpenMP time in ms: 1.77568
6: OpenMP time in ms: 1.55757
8: OpenMP time in ms: 1.48456
*/
// int main()
// {
//     // SETUP
//     std::vector<std::int32_t> my_vector(30'000'000, 0);
//     random_vector(my_vector);

//     auto time1 = 0.0;
//     volatile auto sum1 = 0;
//     for (std::uint32_t i = 0; i < NUM_RUNS; ++i)
//     {
//         cpptiming::Timer t1;
//         sum1 = serial_sum(my_vector);
//         time1 += t1.elapsed_time<cpptiming::millisecs, double>();
//     }
//     std::cout << "Mean Serial: " << time1 / NUM_RUNS << "ms sum: " << sum1 << std::endl;

//     auto time2 = 0.0;
//     volatile auto sum2 = 0;
//     for (std::uint32_t i = 0; i < NUM_RUNS; ++i)
//     {
//         cpptiming::Timer t1;
//         sum2 = parallel_sum_omp(my_vector);
//         time2 += t1.elapsed_time<cpptiming::millisecs, double>();
//     }
//     std::cout << "Mean OpenMP: " << time2 / NUM_RUNS << "ms sum: " << sum2 << std::endl;

//     return 0;
// }
