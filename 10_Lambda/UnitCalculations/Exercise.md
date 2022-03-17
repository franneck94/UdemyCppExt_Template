# Exercise

Implement the following functions:

```cpp
void compute_velocities(const std::array<float, NUM_ENTRIES> long_velocities_mps,
                        const std::array<float, NUM_ENTRIES> lat_velocities_mps,
                        std::array<float, NUM_ENTRIES> &velocities_mps);

void compute_heading_degrees(const std::array<float, NUM_ENTRIES> long_velocities_mps,
                             const std::array<float, NUM_ENTRIES> lat_velocities_mps,
                             std::array<float, NUM_ENTRIES> &heading_degrees);

void compute_accelerations(const std::array<float, NUM_ENTRIES> &velocities_mps,
                           std::array<float, NUM_ENTRIES> &accelerations_mps2);

void compute_rel_velocities(const std::array<float, NUM_ENTRIES> &velocities_mps,
                            const std::array<float, NUM_ENTRIES> &ego_velocities_mps,
                            std::array<float, NUM_ENTRIES> &rel_velocities_mps);

void compute_rel_accelerations(const std::array<float, NUM_ENTRIES> &accelerations_mps2,
                               const std::array<float, NUM_ENTRIES> &ego_accelerations_mps2,
                               std::array<float, NUM_ENTRIES> &rel_accelerations_mps2);
```

### Velocity

To compute the general velocity we can use the lateral and long. velocity:  
$v = \sqrt{v_{lat}^2 + v_{long}^2}$

### Heading Degree

To compute the heading degree we can use the following formula:  
$d = \frac{\arctan2(v_{lat}, v_{long})}{\pi} * 180$

Here is a small introduction to the vehicle heading: [link](http://street.umn.edu/VehControl/javahelp/HTML/Definition_of_Vehicle_Heading_and_Steeing_Angle.htm)  

### Acceleration

To estimate the acceleration we can use the current and previous velocity:  
$v_f$: Current velocity  
$v_i$: Previous velocity  
$t$: Time span between those two velocities (in our case it is 50ms)  
$a = \frac{v_{f} - v_{i}}{t}$

### Relative Velocity/Acceleration

This should be easy, no tips from my side ;)

## Main Function

```cpp
#include <algorithm>
#include <array>
#include <cmath>

#include "Units.hpp"

constexpr std::size_t NUM_ENTRIES = 3;
constexpr std::uint32_t TIME_SPAN = 50;

void compute_velocities(const std::array<float, NUM_ENTRIES> long_velocities_mps,
                        const std::array<float, NUM_ENTRIES> lat_velocities_mps,
                        std::array<float, NUM_ENTRIES> &velocities_mps)
{
    (void)long_velocities_mps;
    (void)lat_velocities_mps;
    (void)velocities_mps;
}

void compute_heading_degrees(const std::array<float, NUM_ENTRIES> long_velocities_mps,
                             const std::array<float, NUM_ENTRIES> lat_velocities_mps,
                             std::array<float, NUM_ENTRIES> &heading_degrees)
{
    (void)long_velocities_mps;
    (void)lat_velocities_mps;
    (void)heading_degrees;
}

void compute_accelerations(const std::array<float, NUM_ENTRIES> &velocities_mps,
                           std::array<float, NUM_ENTRIES> &accelerations_mps2)
{
    (void)velocities_mps;
    (void)accelerations_mps2;
}

void compute_rel_velocities(const std::array<float, NUM_ENTRIES> &velocities_mps,
                            const std::array<float, NUM_ENTRIES> &ego_velocities_mps,
                            std::array<float, NUM_ENTRIES> &rel_velocities_mps)
{
    (void)velocities_mps;
    (void)ego_velocities_mps;
    (void)rel_velocities_mps;
}

void compute_rel_accelerations(const std::array<float, NUM_ENTRIES> &accelerations_mps2,
                               const std::array<float, NUM_ENTRIES> &ego_accelerations_mps2,
                               std::array<float, NUM_ENTRIES> &rel_accelerations_mps2)
{
    (void)accelerations_mps2;
    (void)ego_accelerations_mps2;
    (void)rel_accelerations_mps2;
}

int main(int argc, char **argv)
{
    const auto ego_velocities_mps = std::array<float, NUM_ENTRIES>{25.0F, 25.6F, 26.2F};
    const auto ego_accelerations_mps2 = std::array<float, NUM_ENTRIES>{0.0F, 0.6F, 0.6F};

    const auto long_velocities_mps = std::array<float, NUM_ENTRIES>{23.0F, 25.0F, 27.0F};
    const auto lat_velocities_mps = std::array<float, NUM_ENTRIES>{1.0F, 1.2F, 1.1F};

    auto velocities_mps = std::array<float, NUM_ENTRIES>{};
    auto heading_degrees = std::array<float, NUM_ENTRIES>{};
    auto accelerations_mps2 = std::array<float, NUM_ENTRIES>{};
    auto rel_velocities_mps = std::array<float, NUM_ENTRIES>{};
    auto rel_accelerations_mps2 = std::array<float, NUM_ENTRIES>{};

    compute_velocities(long_velocities_mps, lat_velocities_mps, velocities_mps);
    compute_heading_degrees(long_velocities_mps, lat_velocities_mps, heading_degrees);
    compute_accelerations(velocities_mps, accelerations_mps2);
    compute_rel_velocities(long_velocities_mps, ego_velocities_mps, rel_velocities_mps);
    compute_rel_accelerations(accelerations_mps2, ego_accelerations_mps2, rel_accelerations_mps2);
}
```
