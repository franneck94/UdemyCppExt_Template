# Exercise

Implement the following functions:

```cpp
using It = std::list<std::int32_t>::iterator;

using DifferenceType = It::difference_type;

void advance(It &it, DifferenceType n);

DifferenceType distance(It first, It last);

It next(It it, DifferenceType n = 1);

It prev(It it, DifferenceType n = 1);
```

- advance
  - Step n-steps to the right (forward) with the iterator
- distance
  - Compute the distance between two iterators of the same container
- next
  - Step 1-step to the right (forward) with the iterator
- prev
  - Step 1-step to the left (backward) with the iterator

## Main Function

```cpp
int main()
{
    std::list<int> list{1, 2, 3};
    auto list_it = list.begin();

    auto next_it = mystd::next(list_it);
    std::cout << "*next_it: " << *next_it << '\n';

    auto prev_it = mystd::prev(next_it);
    std::cout << "*prev_it: " << *prev_it << '\n';

    mystd::advance(list_it, 2);
    std::cout << "advance: " << *list_it << '\n';

    auto dist = mystd::distance(list_it, list.begin());
    std::cout << "distance: " << dist << '\n';

    return 0;
}
```
