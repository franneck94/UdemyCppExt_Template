# Exercise

In this exercise, you have to use an**int** datatype for certain computations.

Implement the following functions:

```cpp
std::uint32_t modulo(std::uint32_t number_a, std::uint32_t number_b);

std::uint32_t sum_of_digits(std::uint32_t number);

std::uint32_t cross_sum(std::uint32_t number);
```

Modulo: Implement the modulo operator (a % b = c)  
Sum Of Digits: Number of decimal digits (e.g. 128 = 3 Digits)  
Cross Sum (*Quersumme*): Add up all decimal digits (e.g. 123 = 6)  

Note:

The datatype std::uint32_t is defined in the **<cstdint>** header file.

```cpp
#include <cstdint>

std::uint32_t i = 0;
```

These datatypes ensure that for every OS and compiler the value range is the same.

## Main Function

```cpp
int main()
{
    std::uint32_t input_number = 0;

    std::cout << "Please enter a unsinged integer: ";
    std::cin >> input_number;

    std::cout << input_number << " % 3: " << modulo(input_number, 3) << std::endl;
    std::cout << "sum_of_digits: " << sum_of_digits(input_number) << std::endl;
    std::cout << "cross_sum: " << cross_sum(input_number) << std::endl;
}
```
