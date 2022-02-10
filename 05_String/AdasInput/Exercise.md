# Exercise

After this video, we will implement a feature to load in *recorded* data of a test drive of our fictional autonomous car.  
For that, we will read in JSON files.  
To be able to read in the files, update the code from the refactoring video:

- Add the argc, \*\*argv to the main function such that the user can input the file paths
  - *ego_data* file path
  - *vehicle_data* file path
- If the user does not provide these paths just assume that these files are relative to the main file
  - *./data/ego_data.json*
  - *./data/vehicle_data.json*

Note:

In this exercise, we will only update the *main.cc* file.  
For the file paths, use the *filesystem* library.

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

        const auto lane_change_request = get_lane_change_request(ego_vehicle, vehicles);
        const auto lane_change_executed = lateral_control(lane_change_request, ego_vehicle);

        if (lane_change_executed)
        {
            printf("Executed lane change!");
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
```
