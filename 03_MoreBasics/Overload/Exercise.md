# Exercise

Implement the following functions:

```cpp
void print_array(const std::int32_t *array, const std::size_t length);

void print_array(const char *array);
```

- Print Array (int array)
  - Overloaded function to print out all values
- Print Array (char array)
  - Overloaded function to print out all characters

Recall yourself what the rules for function overloading are.

## Main Function

```cpp
int main()
{
    const std::int32_t array[]{1, 2, 3};
    const char String[]{"Jan"};

    print_array(array, 3);
    print_array(String);

    return 0;
}
```
