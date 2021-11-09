# Exercise

Implement the following functions:

```cpp
double get_distance(const std::uint32_t x1,
                    const std::uint32_t y1,
                    const std::uint32_t x2,
                    const std::uint32_t y2);
```

Implement this function to compute the distance of two pixels: p1=(x1, y1), p2=(x2, y2).

```cpp
struct Shape
{
public:
    Shape() = default;
    ~Shape() noexcept = default;

    virtual double get_area() const = 0;
    double midpoint_distance(const Shape *other) const;
    virtual Coordiante get_midpoint() const = 0;
};

struct Circle : Shape
{
    Circle(std::uint32_t x_midpoint_, std::uint32_t y_midpoint_, std::uint32_t radius_);
    ~Circle() noexcept = default;

    virtual double get_area() const final;
    virtual Coordiante get_midpoint() const final;

    std::uint32_t x_midpoint;
    std::uint32_t y_midpoint;
    std::uint32_t radius;
};

struct Rectangle : Shape
{
    Rectangle(std::uint32_t x1_, std::uint32_t y1_, std::uint32_t x2_, std::uint32_t y2_);
    ~Rectangle() noexcept = default;

    virtual double get_area() const final;
    virtual Coordiante get_midpoint() const final;

    std::uint32_t x1;
    std::uint32_t y1;
    std::uint32_t x2;
    std::uint32_t y2;
};
```

Implement the member functions for the Shape, Rectangle, and Circle class.

```cpp
class Image
{
public:
    Image() = delete;
    Image(const std::uint32_t width, const std::uint32_t height);
    ~Image() noexcept = default;

    std::uint32_t get_width() const;
    std::uint32_t get_height() const;

    void save_image(std::string_view file_name) const;

    void clear_image();
    void set_pixel(const std::uint32_t x, const std::uint32_t y, const std::uint8_t value);
    void resize_image(const std::uint32_t new_width, const std::uint32_t new_height);
    void fill_image(const std::uint8_t value);
    void draw_line(const std::uint32_t x1,
                   const std::uint32_t y1,
                   const std::uint32_t x2,
                   const std::uint32_t y2,
                   const std::uint8_t value);

    void draw_rectangle(const Rectangle &rectangle, const std::uint8_t value);

    void fill_rectangle(const Rectangle &rectangle, const std::uint8_t value);

    void draw_circle(const Circle &circle, const std::uint8_t value);

    void fill_circle(const Circle &circle, const std::uint8_t value);

private:
    std::uint32_t m_width;
    std::uint32_t m_height;
    GrayscaleImage m_matrix;
};

```

Here we extend the Image class from the main course.
We will add the following functions:

- draw_rectangle: Draw (only) the border of a rectangle with the color **value**
- fill_rectangle:  Draw the border of a rectangle with the color **value** and also fill all pixels inside the rectangle with the color
- draw_circle: Draw (only) the border of a circle with the color **value**
- fill_circle: Draw the border of a circle with the color **value** and also fill all pixels inside the circle with the color

## Main Function

```cpp
int main()
{
    Circle circle1(200, 200, 20);
    Circle circle2(250, 250, 20);

    Rectangle rectangle1(100, 100, 150, 150);
    Rectangle rectangle2(300, 300, 350, 350);

    std::cout << "Distance c1-c2: " << circle1.midpoint_distance(&circle2) << std::endl;
    std::cout << "Distance c1-r1: " << circle1.midpoint_distance(&rectangle1) << std::endl;
    std::cout << "Distance r1-r2: " << rectangle1.midpoint_distance(&rectangle2) << std::endl;
    std::cout << "Distance r1-x2: " << rectangle1.midpoint_distance(&circle2) << std::endl;

    Image image1(400, 400);
    image1.fill_image(125);

    image1.draw_rectangle(rectangle1, 255);
    image1.draw_circle(circle1, 255);
    image1.fill_rectangle(rectangle2, 255);
    image1.fill_circle(circle2, 255);
    image1.save_image("image.bmp");

    return 0;
}

```
