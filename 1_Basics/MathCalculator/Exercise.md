# Exercise

In this exercise, you have to use the datatype **double** and **std::uint8_t** for certain computations.

Implement the following functions:

```cpp
double calculate_pi(std::uint32_t num_iterations);

void print_dec_to_bin(std::uint8_t value);
```

## Calculate Pi

With the following formula, you can compute $\frac{\pi}{4}$.  
Implement the formula in the function with $n$ = **num_iterations**.  
Then use a for-loop, to sum up, the value.
At the end multiply with 4 before you return the value.

$\frac{\pi}{4} = \sum_0^n \frac{1}{(4k + 1)} - \frac{1}{(4k + 3)}$

## Decimal to Binary

Implement a function that prints out the binary representation of the decimal (regular number) input.

E.g. the decimal number 142 would be 10001110 in binary.

### Hint

To compute $2^i$ you can use:

```cpp
#include <cmath>

std::pow(2, i)
```

## Main Function

```cpp
int main()
{
    std::uint32_t num_iterations = 1'000'000;
    double pi = 3.14159265358979323846;
    double pi_calculated = calculate_pi(num_iterations);

    std::cout << std::setprecision(30);
    std::cout << "pi (calculated): " << pi_calculated << std::endl;
    std::cout << "pi (correct): " << pi << std::endl;

    std::uint8_t dec = 142;
    print_dec_to_bin(dec);
}
```
