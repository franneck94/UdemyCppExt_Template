# Exercise

In this exercise, you have to use the datatype **float** for certain computations.

Implement the following functions:

```cpp
float convert_milliseconds_to_seconds(float Millisconds);

float convert_seconds_to_minutes(float seconds);

float convert_minutes_to_hours(float Minutes);

float convert_milliseconds_to_minutes(float Millisconds);

float convert_milliseconds_to_hours(float Millisconds);

float convert_seconds_to_hours(float seconds);
```

These functions should return the converted value.

E.g. **convert_milliseconds_to_seconds(3200.0F)** should return 3.2F  
E.g. **convert_seconds_to_minutes(120.0F)** should return 2.0F  

Note:

- 1.000 Milliseconds = 1 Second
- 60 Seconds = 1 Minute
- 60 Minutes = 1 Hour

## Main Function

```cpp
int main()
{
    float Input_milliseconds = 0.0F;

    std::cout << "Please enter a millisecond value: ";
    std::cin >> Input_milliseconds;

    float seconds = convert_milliseconds_to_seconds(Input_milliseconds);
    float Minutes = convert_seconds_to_minutes(seconds);
    float Hours = convert_minutes_to_hours(Minutes);

    float Minutes2 = convert_milliseconds_to_minutes(Input_milliseconds);
    float Hours2 = convert_milliseconds_to_hours(Input_milliseconds);
    float Hours3 = convert_seconds_to_hours(seconds);

    std::cout << "seconds: " << seconds << '\n';
    std::cout << "Minutes: " << Minutes << '\n';
    std::cout << "Hours: " << Hours << '\n';

    std::cout << "Minutes2: " << Minutes2 << '\n';
    std::cout << "Hours2: " << Hours2 << '\n';
    std::cout << "Hours3: " << Hours3 << '\n';

    return 0;
}
```
