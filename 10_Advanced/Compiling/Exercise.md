# Exercise

Implement the following functions:

```cpp
namespace fs = std::filesystem;

using FileVec = std::vector<fs::path>;

bool is_c_source_file(const fs::path &file);

bool is_cpp_source_file(const fs::path &file);

bool is_c_header_file(const fs::path &file);

bool is_cpp_header_file(const fs::path &file);

auto number_of_source_files(const FileVec &files);

auto number_of_header_files(const FileVec &files);
```

- is_c_header_file: Return the number of C header files in a directory
- is_c_source_file: Return the number of C source files in a directory
- is_cpp_header_file: Return the number of C++ header files in a directory
- is_cpp_source_file: Return the number of C++ source files in a directory
- number_of_header_files: Return the number of C/C++ header files in a directory
- number_of_source_files: Return the number of C/C++ source files in a directory

Use lambda expressions for these functions!

## Main Function

```cpp
int main(int argc, char **argv)
{
    fs::path dir;

    if (argc != 2)
    {
        dir /= fs::current_path();
        dir /= "test";
    }
    else
    {
        const auto input_path = std::string(argv[1]);
        dir = fs::path(input_path);
    }

    auto all_files = FileVec{};
    for (auto it = fs::directory_iterator(dir); it != fs::directory_iterator{}; ++it)
    {
        all_files.push_back(*it);
    }

    const auto number_sources = number_of_source_files(all_files);
    const auto number_headers = number_of_header_files(all_files);

    std::cout << number_sources << std::endl;
    std::cout << number_headers << std::endl;

    for (const auto &file : all_files)
    {
        std::cout << "File: " << file << std::endl;
        std::cout << std::boolalpha;
        std::cout << "is_c_header_file: " << is_c_header_file(file) << std::endl;
        std::cout << "is_c_source_file: " << is_c_source_file(file) << std::endl;
        std::cout << "is_cpp_header_file: " << is_cpp_header_file(file) << std::endl;
        std::cout << "is_cpp_source_file: " << is_cpp_source_file(file) << std::endl << std::endl;
    }
}
```
