# Exercise

In the C++ std, there are different string/string_view types.

```cpp
// Type             Definition
std::string         std::basic_string<char>;
std::wstring        std::basic_string<wchar_t>;
std::u8string       std::basic_string<char8_t>;
std::u16string      std::basic_string<char16_t>;
std::u32string      std::basic_string<char32_t>;
```

We only want to allow std::string and std::wstring for our *library*.  
For that, you have to create type_traits that will check whether the string/string_view is based on char/wchar.  

Afterward, implement the following template functions that will use the type_trait internally with a static_assert:

```cpp
template <typename StringType, typename ViewType>
StringType to_upper_case(ViewType text);

template <typename StringType, typename ViewType>
StringType to_lower_case(ViewType text);
```

## Main Function

```cpp
int main()
{
    auto input_text = std::string{};
    std::cout << "Please enter any text: ";
    std::cin >> input_text;

    std::cout << "to_upper_case: " << to_upper_case<std::string, std::string_view>(input_text)
              << '\n';
    std::cout << "to_lower_case: " << to_lower_case<std::string, std::string_view>(input_text)
              << '\n';

    auto input_text_w = std::wstring(input_text.begin(), input_text.end());
    to_upper_case<std::wstring, std::wstring_view>(input_text_w);
    to_lower_case<std::wstring, std::wstring_view>(input_text_w);

    // Should raise an assertion error
    // auto input_text_u16 = std::u16string(input_text.begin(), input_text.end());
    // to_upper_case<std::u16string, std::u16string_view>(input_text_w);
    // to_lower_case<std::u16string, std::u16string_view>(input_text_w);

    return 0;
}
```
