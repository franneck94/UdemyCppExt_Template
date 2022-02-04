# Exercise

Implement the following operators for our book class:

```cpp
class Book
{
    friend std::ostream &operator<<(std::ostream &, const Book &);

public:
    Book() = default;
    Book(std::string_view name_, std::string_view author_, std::uint32_t pubyear_);
    ~Book() noexcept = default;

    bool operator==(const Book &other) const;
    bool operator!=(const Book &other) const;

private:
    std::string name;
    std::string author;
    std::uint32_t pubyear;
};

std::ostream &operator<<(std::ostream &, const Book &);
```

==, !=: A book is equal if all member variables are the same.  
<<: Print out all member variables separated by spaces  

## Main Function

```cpp
int main()
{
    Book b1("Harry Potter - Stein der Weisen", "J. K. Rowling", 1997);
    Book b2("Harry Potter - Die Kammer des Schreckens", "J. K. Rowling", 1998);

    std::cout << b1 << '\n';
    std::cout << b2 << '\n';

    std::cout << std::boolalpha;
    std::cout << (b1 == b2) << '\n';
    std::cout << (b1 == b1) << '\n';

    return 0;
}
```
