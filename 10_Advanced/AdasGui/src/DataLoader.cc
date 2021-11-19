#include <exception>
#include <fstream>
#include <iostream>

#include "AdConstants.hpp"
#include "AdFunctions.hpp"
#include "AdTypes.hpp"
#include "DataLoader.hpp"

static VehiclesData vehicles_data =
    std::vector<std::vector<VehicleType>>(NUM_VEHICLES, std::vector<VehicleType>(NUM_ITERATIONS));

void init_ego_vehicle(std::string_view filepath, VehicleType &ego_vehicle)
{
    std::ifstream ifs(filepath.data());
    auto parsed_data = json::parse(ifs);

    ego_vehicle.id = EGO_VEHICLE_ID;
    ego_vehicle.distance_m = 0.0F;

    ego_vehicle.speed_mps = parsed_data["Speed"];
    ego_vehicle.lane = parsed_data["Lane"];
}

void init_vehicles(std::string_view filepath, NeighborVehiclesType &vehicles)
{
    std::ifstream ifs(filepath.data());
    auto parsed_data = json::parse(ifs);

    std::size_t vehicle_idx = 0;

    try
    {
        for (; vehicle_idx < NUM_VEHICLES; ++vehicle_idx)
        {
            const auto &vehicle_data = parsed_data[std::to_string(vehicle_idx)];

            const auto lane = static_cast<LaneAssociationType>(vehicle_data["Lane"]);
            const auto distance = static_cast<float>(vehicle_data["Distance"]);

            for (std::size_t iteration_idx = 0; iteration_idx < NUM_ITERATIONS; ++iteration_idx)
            {
                const auto speed = static_cast<float>(vehicle_data["Speed"][iteration_idx]);

                vehicles_data[vehicle_idx][iteration_idx] =
                    VehicleType{static_cast<std::int32_t>(vehicle_idx), lane, speed, distance};
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    vehicles.vehicles_left_lane[0] = vehicles_data[0][0];
    vehicles.vehicles_left_lane[1] = vehicles_data[1][0];
    vehicles.vehicles_center_lane[0] = vehicles_data[2][0];
    vehicles.vehicles_center_lane[1] = vehicles_data[3][0];
    vehicles.vehicles_right_lane[0] = vehicles_data[4][0];
    vehicles.vehicles_right_lane[1] = vehicles_data[5][0];
}

void load_cycle(const std::size_t cycle, NeighborVehiclesType &vehicles)
{
    vehicles.vehicles_left_lane[0].speed_mps = vehicles_data[0][cycle].speed_mps;
    vehicles.vehicles_left_lane[1].speed_mps = vehicles_data[1][cycle].speed_mps;
    vehicles.vehicles_center_lane[0].speed_mps = vehicles_data[2][cycle].speed_mps;
    vehicles.vehicles_center_lane[1].speed_mps = vehicles_data[3][cycle].speed_mps;
    vehicles.vehicles_right_lane[0].speed_mps = vehicles_data[4][cycle].speed_mps;
    vehicles.vehicles_right_lane[1].speed_mps = vehicles_data[5][cycle].speed_mps;
}
