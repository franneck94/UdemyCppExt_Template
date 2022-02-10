# Exercise

In this exercise, you have to use the datatype **char** to convert/check certain char values.

Implement the following checks:

```cpp
bool is_numeric(char character);

bool is_alpha(char character);

bool is_alpha_numeric(char character);

bool is_upper_case(char character);

bool is_lower_case(char character);
```

These functions should return **true** or **false**.

Meaning of the function names:

- Numeric: 0-9 (digits)
- Alpha: a-z, A-Z (alphabeth characters)
- Upper Case: A-Z
- Lower Case: a-z

Note:

The shift from **Upper Case** to **Lower Case** is 32 in the ASCII table.  

E.g. 'A' -> 'a' by subtracting 32
E.g. 'a' -> 'A' by adding 32

You do not have to look up the ASCII table for this exercise, but you can do so.

Afterward, implement the following conversion functions:

```cpp
char to_upper_case(char character);

char to_lower_case(char character);
```

These should return the **converted** character.

## Main Function

```cpp
int main()
{
    char input_character;

    std::cout << "Please enter any ASCII character: ";
    std::cin >> input_character;

    std::cout << std::boolalpha;
    std::cout << "is_numeric: " << is_numeric(input_character) << '\n';
    std::cout << "is_alpha: " << is_alpha(input_character) << '\n';
    std::cout << "is_alpha_numeric: " << is_alpha_numeric(input_character) << '\n';
    std::cout << "is_upper_case: " << is_upper_case(input_character) << '\n';
    std::cout << "is_lower_case: " << is_lower_case(input_character) << '\n';
    std::cout << std::dec;
    std::cout << "to_upper_case: " << to_upper_case(input_character) << '\n';
    std::cout << "to_lower_case: " << to_lower_case(input_character) << '\n';

    return 0;
}
```
