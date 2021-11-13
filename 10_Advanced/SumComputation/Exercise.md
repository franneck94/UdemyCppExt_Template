# Exercise

Note: Based on Chapter 13 in the main course.

```cpp
#include "utils.hpp"
#include "Timer.hpp"

template <typename RandomIter>
auto range_sum_asyn(RandomIter start, RandomIter stop)
```

Implement an async function that computes the sum of a container in the range of the **start** and **stop** iterator.  
The function should launch several tasks within an async context.

E.g.

```cpp
std::async(std::launch::async, range_sum_asyn<RandomIter>, mid, stop);
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

    return 0;
}
```
