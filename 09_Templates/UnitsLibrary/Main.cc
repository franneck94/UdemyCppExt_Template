#include <iostream>

#include "Units.hpp"

int main()
{
    float x = 5;
    float y = 4;

    const float deg = 10.0;
    const float rad = deg_to_rad(deg);

    const auto [x_, y_] = rotate_point(rad, x, y);
    std::cout << x_ << " " << y_ << std::endl;

    return 0;
}
