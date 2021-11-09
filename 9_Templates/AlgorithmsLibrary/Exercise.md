# Exercise

Implement the following functions:

```cpp
template <class InputIter1, class InputIter2>
bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2);

template <class OutputIter, class Size, class T>
OutputIter fill_n(OutputIter first, Size count, const T &value);

template <class ForwardIter, class T>
void iota(ForwardIter first, ForwardIter last, T value);

template <class InputIter, class OutputIter>
OutputIter copy(InputIter first, InputIter last, OutputIter d_first);

template <class InputIter, class T>
T accumulate(InputIter first, InputIter last, T init);
```

Transform the functions from chapter 7 to template functions.

## Main Function

```cpp
int main()
{
    auto vec1 = std::vector<std::uint32_t>(5, 0);
    auto vec2 = std::vector<std::uint32_t>(5, 0);

    std::cout << std::boolalpha << mystd::equal(vec1.begin(), vec1.end(), vec2.begin()) << std::dec
              << std::endl;

    mystd::fill_n(vec1.begin(), 5, 1);
    print_vector(vec1);

    mystd::iota(vec2.begin(), vec2.end(), 0);
    print_vector(vec2);

    std::cout << std::boolalpha << mystd::equal(vec1.begin(), vec1.end(), vec2.begin()) << std::dec
              << std::endl;

    mystd::copy(vec2.begin(), vec2.end(), vec1.begin());

    std::cout << std::boolalpha << mystd::equal(vec1.begin(), vec1.end(), vec2.begin()) << std::dec
              << std::endl;

    const auto sum = mystd::accumulate(vec1.begin(), vec1.end(), 0);
    std::cout << sum << std::endl;

    return 0;
}
```
