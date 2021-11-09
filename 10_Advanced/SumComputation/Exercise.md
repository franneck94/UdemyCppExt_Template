# Exercise

Implement the following functions:

```cpp
void random_vector(std::vector<int> &vec);

template <typename RandomIter>
auto range_sum_asyn(RandomIter start, RandomIter stop);

template <typename RandomIter>
void range_sum_thread_helper(RandomIter start, RandomIter stop, int &sum);

double range_sum_thread(const std::vector<int> &my_vector;
```

## Main Function

```cpp
int main()
{
    std::vector<int> my_vector(30'000'000, 0);
    random_vector(my_vector);

    double time1 = 0.0;
    volatile auto sum1 = 0;
    for (std::uint32_t i = 0; i < NUM_RUNS; ++i)
    {
        cpptiming::Timer t1;
        sum1 = range_sum_asyn(my_vector.begin(), my_vector.end());
        time1 += t1.elapsed_time<cpptiming::millisecs, double>();
    }
    std::cout << "Mean Async: " << time1 / NUM_RUNS << "ms sum: " << sum1 << std::endl;

    double time2 = 0.0;
    cpptiming::Timer t2;
    auto sum2 = range_sum_thread(my_vector);
    time2 += t2.elapsed_time<cpptiming::millisecs, double>();
    std::cout << "Mean Threadding: " << time2 / 1 << "ms sum: " << sum2 << std::endl;

    return 0;
}
```
