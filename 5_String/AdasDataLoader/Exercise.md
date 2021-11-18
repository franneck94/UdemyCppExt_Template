# Exercise

For this exercise we want to load a data log of a fictional test drive of our autonomous car.

The collected data is stored in the **data** folder.
There, the initial data of out ego vehicle is stored in **ego_data.json**, and the data of the other vehicles is stored in **vehicle_data.json**.

To read in the json data we will use the most used C++ JSON library called **nlohman/json**. For this there is the header-only library file in **nlohman**.

Delete the following functions from the **AdFunctions.cc** file:

```cpp
void init_ego_vehicle(VehicleType &ego_vehicle);

void init_vehicles(NeighborVehiclesType &vehicles);
```

and implement the following functions in the DataLoader.hpp/.cpp:

```cpp
void init_vehicles(std::string_view filepath, NeighborVehiclesType &vehicles);

void init_ego_vehicle(std::string_view filepath, VehicleType &ego_vehicle);
```

Instead of generating the data by just random numbers, you have to load the JSON data and fill it into the **vehicles** and **ego_vehicle** struct.

Note:

- **vehicles** and **ego_vehicle** will be initialized only once
- **init_ego_vehicle** should load the whole json data into one big static struct
  - This means a global variable:
    ```cpp
    static VehiclesData vehicles_data =
        std::vector<std::vector<VehicleType>>(NUM_VEHICLES, std::vector<VehicleType>(NUM_ITERATIONS));
    ```

Afterwards implement the following function:

```cpp
void load_cycle(const std::uint32_t cycle, NeighborVehiclesType &vehicles);
```

There you will take the other vehicle data (the static struct from above).
Use the current **cycle** index to get the data from the correct cycle and store it into the **vehicles** struct.

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

    init_vehicles(data_filepath.c_str(), vehicles);
    init_ego_vehicle(ego_filepath.c_str(), ego_vehicle);

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
            std::cout << "Lane change request: " << static_cast<int>(lane_change_request) << std::endl;
        }
        if (lane_change_successful)
        {
            std::cout << "Lane change successull" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        cycle++;

        load_cycle(cycle, vehicles);
    }

    return 0;
}
```
