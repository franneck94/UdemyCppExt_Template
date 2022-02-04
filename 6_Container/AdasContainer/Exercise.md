# Exercise

Update the code such that we use *std::string*, *std::array* and *std::string_view*.

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
        auto vehicles_input_path = std::string(argv[1]);
        data_filepath = fs::path(vehicles_input_path);

        auto ego_input_path = std::string(argv[2]);
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
        compute_future_state(ego_vehicle, vehicles, 0.050F);
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

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        cycle++;

        load_cycle(cycle, vehicles);
    }

    return 0;
}
```
