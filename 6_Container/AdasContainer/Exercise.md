# Exercise

## Update Ad

Update the *AdFunctions* code such that we use *std::string*, *std::string_view* and *std::array*.

Update the following functions:

```cpp
const VehicleType *get_vehicle_array(const LaneAssociationType lane,
                                     const NeighborVehiclesType &vehicles);
```

- get_vehicle_array
  - Instead of **VehicleType \*** return a reference to a vehicle type array

## Update DataLoader

We will now use a new struct that is more memory efficient to store the log data.

```cpp
struct VehicleLogData
{
    std::int32_t id;
    LaneAssociationType lane;
    float start_distance_m;
    std::array<float, NUM_ITERATIONS> speeds_mps;
};

using VehiclesLogData = std::array<VehicleLogData, NUM_VEHICLES>;
```

Hence, we will only store the speed for every cycle (iteration) and not as before the other vehicle attributes, because these doesn't change in the cycles anyway.

Update the following functions regarding the new *VehiclesLogData* struct.

- init_vehicles
- load_cycle

## Main Function

```cpp
#include <chrono>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <numeric>
#include <thread>

#include "AdFunctions.hpp"
#include "AdTypes.hpp"
#include "DataLoader.hpp"

#include "utils.hpp"

namespace fs = std::filesystem;

int main(int argc, char **argv)
{
    fs::path data_filepath;
    fs::path ego_filepath;

    if (argc != 3)
    {
        data_filepath /= fs::current_path();
        data_filepath /= "data";
        ego_filepath = data_filepath;

        data_filepath /= "vehicle_data.json";
        ego_filepath /= "ego_data.json";
    }
    else
    {
        const auto vehicles_input_path = std::string(argv[1]);
        data_filepath = fs::path(vehicles_input_path);

        const auto ego_input_path = std::string(argv[2]);
        ego_filepath = fs::path(ego_input_path);
    }

    std::uint32_t cycle = 0;
    VehicleType ego_vehicle{};
    NeighborVehiclesType vehicles{};

    init_vehicles(data_filepath.string(), vehicles);
    init_ego_vehicle(ego_filepath.string(), ego_vehicle);

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
        const auto lane_change_request = longitudinal_control(vehicles, ego_vehicle);
        const auto lane_change_successful = lateral_control(vehicles, lane_change_request, ego_vehicle);

        if (lane_change_request != ego_vehicle.lane)
        {
            std::cout << "Lane change request: " << static_cast<std::int32_t>(lane_change_request) << '\n';
        }
        if (lane_change_successful)
        {
            std::cout << "Lane change successull" << '\n';
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        cycle++;

        load_cycle(cycle, vehicles);
    }

    return 0;
}
```
