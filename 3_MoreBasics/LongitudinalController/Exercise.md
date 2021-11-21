# Exercise

Update the following functions

```cpp
void print_scene(const VehicleType &ego_vehicle, const NeighborVehiclesType &vehicles);
```

- print_scene:
  - Update the distance slices to 10m instead of 20m
  - Add at the bottom of the function the following console output

<img src="../../media/vehicle2.png" alt="vehicle"/>

Implement the following functions:

```cpp
void longitudinal_control(const VehicleType &front_vehicle, VehicleType &ego_vehicle);
```

If there is a vehicle in front of the ego vehicle and the distance is below half speed (kph, "halbe tacho"), the vehicle needs to slow down (brake pedal).

## Main Function

```cpp
#include <chrono>
#include <cstring>
#include <iostream>
#include <numeric>
#include <thread>

#include "AdFunctions.hpp"
#include "AdTypes.hpp"

#include "utils.hpp"

int main()
{
    VehicleType ego_vehicle{};
    NeighborVehiclesType vehicles{};

    init_ego_vehicle(ego_vehicle);
    init_vehicles(vehicles);

    print_vehicle(ego_vehicle);
    print_neighbor_vehicles(vehicles);

    std::cout << "Start simulation?: ";
    char Input;
    std::cin >> Input;

    while (true)
    {
        clear_console();

        print_scene(ego_vehicle, vehicles);
        compute_future_state(ego_vehicle, vehicles, 0.100F);
        longitudinal_control(vehicles.vehicles_center_lane[0], ego_vehicle);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
```
