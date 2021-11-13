# Exercise

Update the following functions:

```cpp
void print_scene(const VehicleType &ego_vehicle, const NeighborVehiclesType &vehicles);
```

Such that the ego vehicle can be displayed on every lane not only the center lane.

Implement the following functions:

```cpp
LaneAssociationType get_lane_change_request(const VehicleType &ego_vehicle,
                                            const float front_distance_abs,
                                            const float rear_distance_abs,
                                            const bool front_vehicle_too_close_collision,
                                            const bool rear_vehicle_too_close_collision);

bool lateral_control(const NeighborVehiclesType &vehicles,
                     const LaneAssociationType lane_change_request,
                     VehicleType &ego_vehicle);
```

- get_lane_change_request:
  - Is called in the else case of the **longitudinal_control** function
  - Send a lane change request if the vehicle would crash into a front/rear vehicle
  - Send a request to the left, if we are faster than the front vehicle which we would crash into
  - Send a request to the right, if we are slower than the rear vehicle which would crash into us
- lateral_control:
  - Checks if we can change the lane onto the target lane that was sent from the **get_lane_change_request** function


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
        longitudinal_control(vehicles.vehicles_center_lane[0], vehicles.vehicles_center_lane[1], ego_vehicle);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
```
