# Exercise

Implement the following functions:

```cpp
void unique_elements(std::int32_t *array, std::size_t length);

void remove_duplicates(std::int32_t *array, std::size_t &length);

void rotate_left(std::int32_t *array, std::size_t length);

void rotate_right(std::int32_t *array, std::size_t length);
```

- Unique Elements: Print out all elements that are unique in the array
- Remove Duplicates: Remove (move to the end of the array and set to zero) all duplicates in the array
- Rotate Left: Rotate all elements of the array to the left
- Rotate Right: Rotate all elements of the array to the right

E.g.  
Rotate Left: [1, 2, 3] => [2, 3, 1]  
Rotate Right: [1, 2, 3] => [3, 1, 2]  

## Main Function

```cpp
int main()
{
    std::int32_t array[]{1, 1, 2, 3, 3, 4};
    std::size_t length = 6;

    std::cout << "unique_elements: " << std::endl;
    unique_elements(array, length);

    remove_duplicates(array, length);
    print_array(array, length);

    rotate_left(array, length);
    print_array(array, length);

    rotate_right(array, length);
    print_array(array, length);

    return 0;
}
```
