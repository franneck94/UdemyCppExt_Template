#include <iostream>
#include <list>

#include "IteratorLib.hpp"

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
