#include <iostream>

#include "AdConstants.hpp"
#include "AdFunctions.hpp"
#include "AdTypes.hpp"

float kph_to_mps(const float kph)
{
    return kph / 3.6F;
}

float mps_to_kph(const float mps)
{
    return mps * 3.6F;
}

void print_vehicle(const VehicleType &vehicle)
{
    std::cout << "ID: " << vehicle.id << '\n';
    std::cout << "Speed (m/s): " << vehicle.speed_mps << '\n';
    std::cout << "Distance (m): " << vehicle.distance_m << '\n';
    std::cout << "Lane: " << static_cast<std::int32_t>(vehicle.lane) << '\n';
}

void print_neighbor_vehicles(const NeighborVehiclesType &vehicles)
{
    print_vehicle(vehicles.vehicles_left_lane[0]);
    print_vehicle(vehicles.vehicles_left_lane[1]);
    print_vehicle(vehicles.vehicles_center_lane[0]);
    print_vehicle(vehicles.vehicles_center_lane[1]);
    print_vehicle(vehicles.vehicles_right_lane[0]);
    print_vehicle(vehicles.vehicles_right_lane[1]);
}

bool check_vehicle_in_tile(const VehicleType *vehicle, const float range_m, const float offset_m)
{
    return ((vehicle != nullptr) && (range_m >= vehicle->distance_m) &&
            (vehicle->distance_m > (range_m - offset_m)));
}

bool check_vehicle_is_out_of_view(const VehicleType *vehicle)
{
    return ((vehicle != nullptr) && (std::abs(vehicle->distance_m) > VIEW_RANGE_M));
}

void print_scene(const VehicleType &ego_vehicle, const NeighborVehiclesType &vehicles)
{
    std::cout << "    \t  L    C    R  \n";

    std::size_t left_idx = 0;
    std::size_t center_idx = 0;
    std::size_t right_idx = 0;

    const std::int32_t offset_m = 10;
    const auto view_range_m = static_cast<std::int32_t>(VIEW_RANGE_M);

    for (std::int32_t i = view_range_m; i >= -view_range_m; i -= offset_m)
    {
        const VehicleType *left_vehicle =
            left_idx < NUM_VEHICLES_ON_LANE ? &vehicles.vehicles_left_lane[left_idx] : nullptr;
        const VehicleType *center_vehicle =
            center_idx < NUM_VEHICLES_ON_LANE ? &vehicles.vehicles_center_lane[center_idx] : nullptr;
        const VehicleType *right_vehicle =
            right_idx < NUM_VEHICLES_ON_LANE ? &vehicles.vehicles_right_lane[right_idx] : nullptr;

        char left_string[]{"   "};
        char center_string[]{"   "};
        char right_string[]{"   "};
        char *ego_string = nullptr;

        switch (ego_vehicle.lane)
        {
        case LaneAssociationType::LEFT:
        {
            ego_string = left_string;
            break;
        }
        case LaneAssociationType::CENTER:
        {
            ego_string = center_string;
            break;
        }
        case LaneAssociationType::RIGHT:
        {
            ego_string = right_string;
            break;
        }
        default:
        {
            break;
        }
        }

        const auto range_m = static_cast<float>(i);

        if ((ego_string != nullptr) && check_vehicle_in_tile(&ego_vehicle, range_m, offset_m))
        {
            ego_string[1] = 'E';
        }

        if (check_vehicle_in_tile(left_vehicle, range_m, offset_m))
        {
            left_string[1] = 'V';
            left_idx++;
        }
        else if (check_vehicle_is_out_of_view(left_vehicle))
        {
            left_idx++;
        }

        if (check_vehicle_in_tile(center_vehicle, range_m, offset_m))
        {
            center_string[1] = 'V';
            center_idx++;
        }
        else if (check_vehicle_is_out_of_view(center_vehicle))
        {
            center_idx++;
        }

        if (check_vehicle_in_tile(right_vehicle, range_m, offset_m))
        {
            right_string[1] = 'V';
            right_idx++;
        }
        else if (check_vehicle_is_out_of_view(right_vehicle))
        {
            right_idx++;
        }

        std::cout << i << "\t| " << left_string << " |" << center_string << " |" << right_string
                  << " |\n";
    }

    std::cout << "\n";
    print_vehicle_speed(ego_vehicle, "E");
    std::cout << "\n";
}

void print_vehicle_speed(const VehicleType &vehicle, const char *name)
{
    std::cout.precision(3);
    std::cout << name << ": (" << vehicle.speed_mps << " mps)";
}

void compute_future_distance(VehicleType &vehicle,
                             const float ego_driven_distance_m,
                             const float seconds)
{
    const auto driven_distance_m = vehicle.speed_mps * seconds;

    vehicle.distance_m += driven_distance_m - ego_driven_distance_m;
}

void compute_future_state(const VehicleType &ego_vehicle,
                          NeighborVehiclesType &vehicles,
                          const float seconds)
{
    const auto ego_driven_distance_m = ego_vehicle.speed_mps * seconds;

    compute_future_distance(vehicles.vehicles_left_lane[0], ego_driven_distance_m, seconds);
    compute_future_distance(vehicles.vehicles_left_lane[1], ego_driven_distance_m, seconds);
    compute_future_distance(vehicles.vehicles_center_lane[0], ego_driven_distance_m, seconds);
    compute_future_distance(vehicles.vehicles_center_lane[1], ego_driven_distance_m, seconds);
    compute_future_distance(vehicles.vehicles_right_lane[0], ego_driven_distance_m, seconds);
    compute_future_distance(vehicles.vehicles_right_lane[1], ego_driven_distance_m, seconds);
}

void decrease_speed(VehicleType &ego_vehicle)
{
    const auto decrease = ego_vehicle.speed_mps * LONGITUDINAL_DIFFERENCE_PERCENTAGE;

    if ((ego_vehicle.speed_mps - decrease) >= 0.0F)
    {
        ego_vehicle.speed_mps -= decrease;
    }
}

void longitudinal_control(const VehicleType &front_vehicle, VehicleType &ego_vehicle)
{
    const auto minimal_distance_m = mps_to_kph(ego_vehicle.speed_mps) / 2.0F;
    const auto front_distance_m = front_vehicle.distance_m;

    if (front_distance_m < minimal_distance_m)
    {
        decrease_speed(ego_vehicle);
    }
}

const VehicleType *get_vehicle_array(const LaneAssociationType lane,
                                     const NeighborVehiclesType &vehicles)
{
    const VehicleType *vehicles_array = nullptr;

    switch (lane)
    {
    case LaneAssociationType::LEFT:
    {
        vehicles_array = vehicles.vehicles_left_lane;
        break;
    }
    case LaneAssociationType::CENTER:
    {
        vehicles_array = vehicles.vehicles_center_lane;
        break;
    }
    case LaneAssociationType::RIGHT:
    {
        vehicles_array = vehicles.vehicles_right_lane;
        break;
    }
    default:
    {
        break;
    }
    }

    return vehicles_array;
}

LaneAssociationType get_lane_change_request(const VehicleType &ego_vehicle,
                                            const NeighborVehiclesType &vehicles)
{
    const VehicleType *ego_lane_vehicles = get_vehicle_array(ego_vehicle.lane, vehicles);
    const VehicleType *rear_vehicle = &ego_lane_vehicles[1];

    const auto minimal_distance_m = mps_to_kph(ego_vehicle.speed_mps) / 5.0F;
    const auto rear_distance_m = std::abs(rear_vehicle->distance_m);

    if (rear_distance_m < minimal_distance_m)
    {
        switch (ego_vehicle.lane)
        {
        case LaneAssociationType::RIGHT: /* fall-thorugh */
        case LaneAssociationType::LEFT:
        {
            const LaneAssociationType target_lane = LaneAssociationType::CENTER;
            const VehicleType *center_vehicles = get_vehicle_array(target_lane, vehicles);

            const auto abs_front_center_distance_m = std::abs(center_vehicles[0].distance_m);
            const auto abs_rear_center_distance_m = std::abs(center_vehicles[1].distance_m);

            if ((abs_front_center_distance_m > minimal_distance_m) &&
                (abs_rear_center_distance_m > minimal_distance_m))
            {
                return target_lane;
            }

            break;
        }
        case LaneAssociationType::CENTER:
        {
            LaneAssociationType target_lane = LaneAssociationType::RIGHT;

            const VehicleType *right_vehicles = get_vehicle_array(target_lane, vehicles);

            const auto abs_front_right_distance_m = std::abs(right_vehicles[0].distance_m);
            const auto abs_rear_right_distance_m = std::abs(right_vehicles[1].distance_m);

            if ((abs_front_right_distance_m > minimal_distance_m) &&
                (abs_rear_right_distance_m > minimal_distance_m))
            {
                return target_lane;
            }

            target_lane = LaneAssociationType::LEFT;
            const VehicleType *left_vehicles = get_vehicle_array(target_lane, vehicles);

            const auto abs_front_left_distance_m = std::abs(left_vehicles[0].distance_m);
            const auto abs_rear_left_distance_m = std::abs(left_vehicles[1].distance_m);

            if ((abs_front_left_distance_m > minimal_distance_m) &&
                (abs_rear_left_distance_m > minimal_distance_m))
            {
                return target_lane;
            }

            break;
        }
        default:
        {
            break;
        }
        }
    }

    return ego_vehicle.lane;
}


bool lateral_control(const LaneAssociationType lane_change_request, VehicleType &ego_vehicle)
{
    if (lane_change_request == ego_vehicle.lane)
    {
        return false;
    }

    ego_vehicle.lane = lane_change_request;

    return true;
}
