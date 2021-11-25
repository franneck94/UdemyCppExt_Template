# Exercise

Implement the following functions:

```cpp
namespace mystd
{

bool is_palindrom(std::string_view str);

bool starts_with(std::string_view str, std::string_view substr);

bool ends_with(std::string_view str, std::string_view substr);

bool contains(std::string_view str, std::string_view substr);

std::size_t num_occurences(std::string_view str, std::string_view substr);

} // namespace mystd
```

- is_palindrom
  - Check if the string is a palindrome
  - E.g. "anna" is a palindrome since the reversed string is the same.
- starts_with:
  - Check if the string starts with the substring
- ends_with:
  - Check if the string ends with the substring
- contains
  - Check if the string contains the substring
- num_occurences
  - Count how many times the substring is in the string

## Main Function

```cpp
int main()
{
    std::string str1 = "anna";
    std::cout << std::boolalpha << str1 << " is_palindrom: " << mystd::is_palindrom(str1) << std::endl;

    std::string str2 = "JanIstHier";
    std::cout << std::boolalpha << str2 << " starts_with: Jan = " << mystd::starts_with(str2, "Jan") << std::endl;
    std::cout << std::boolalpha << str2 << "starts_with: Ja = " << mystd::starts_with(str2, "Ja") << std::endl;
    std::cout << std::boolalpha << str2 << "starts_with: ja = " << mystd::starts_with(str2, "ja") << std::endl;

    std::string str3 = "JanWarHier";
    std::cout << std::boolalpha << str3 << " ends_with: Hier = " << mystd::ends_with(str3, "Hier") << std::endl;
    std::cout << std::boolalpha << str3 << " ends_with: Hie = " << mystd::ends_with(str3, "Hie") << std::endl;
    std::cout << std::boolalpha << str3 << " ends_with: Hir = " << mystd::ends_with(str3, "Hir") << std::endl;

    std::string str4 = "JanWarHier";
    std::cout << std::boolalpha << str4 << " contains: Hier = " << mystd::contains(str4, "Hier") << std::endl;
    std::cout << std::boolalpha << str4 << " contains: War = " << mystd::contains(str4, "War") << std::endl;
    std::cout << std::boolalpha << str4 << " contains: jan = " << mystd::contains(str4, "jan") << std::endl;

    std::string str5 = "HalloHallohaHallo";
    std::cout << "num_occurences: " << mystd::num_occurences(str5, "Hallo") << std::endl;

    return 0;
}
```
