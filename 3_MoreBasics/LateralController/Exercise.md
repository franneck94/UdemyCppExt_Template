# Exercise

Implement the following functions:

```cpp
LaneAssociationType get_lane_change_request(const VehicleType &ego_vehicle, const NeighborVehiclesType &vehicles);

bool lateral_control(const LaneAssociationType lane_change_request, VehicleType &ego_vehicle);
```

- get_lane_change_request
  - Send a lane change request if the vehicle would crash with the rear vehicle
    - If there is enough space on the right lane, send a request
    - Else If there is enough space on the left lane, send a request
    - Else, don't do anything
    - Check the size of the gap on the target lane
- lateral_control
  - If there is a request from **get_lane_change_request** do a lane change

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

        const VehicleType *ego_lane_vehicles = get_vehicle_array(ego_vehicle.lane, vehicles);
        longitudinal_control(ego_lane_vehicles[0], ego_vehicle);

        const LaneAssociationType lane_change_request = get_lane_change_request(ego_vehicle, vehicles);
        const bool lane_change_executed = lateral_control(lane_change_request, ego_vehicle);

        if (lane_change_executed)
        {
            printf("Executed lane change!");
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
```
