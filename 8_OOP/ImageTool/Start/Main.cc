#include <iostream>

#include "Image.hpp"

int main()
{
    Image image1(100, 400);

    image1.resize_image(500, 300);
    image1.fill_image(125);
    image1.draw_line(150, 0, 150, 200, 255);
    image1.draw_line(100, 150, 200, 150, 255);
    image1.save_image("image.bmp");

    return 0;
}
