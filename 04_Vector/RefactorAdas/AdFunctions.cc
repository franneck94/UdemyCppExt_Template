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

void init_ego_vehicle(VehicleType &ego_vehicle)
{
    ego_vehicle.id = EGO_VEHICLE_ID;
    ego_vehicle.speed_mps = kph_to_mps(135.0F);
    ego_vehicle.distance_m = 0.0F;
    ego_vehicle.lane = LaneAssociationType::CENTER;
}

void init_vehicle(VehicleType &vehicle,
                  const std::int32_t id,
                  const float speed_mps,
                  const float distance_m,
                  const LaneAssociationType lane)
{
    vehicle.id = id;
    vehicle.speed_mps = kph_to_mps(speed_mps);
    vehicle.distance_m = distance_m;
    vehicle.lane = lane;
}

void init_vehicles(NeighborVehiclesType &vehicles)
{
    init_vehicle(vehicles.vehicles_left_lane[0], 0, 130.0F, 80.0F, LaneAssociationType::LEFT);
    init_vehicle(vehicles.vehicles_left_lane[1], 1, 80.0F, -20.0F, LaneAssociationType::LEFT);
    init_vehicle(vehicles.vehicles_center_lane[0], 2, 80.0F, 50.0F, LaneAssociationType::CENTER);
    init_vehicle(vehicles.vehicles_center_lane[1], 3, 120.0F, -50.0F, LaneAssociationType::CENTER);
    init_vehicle(vehicles.vehicles_right_lane[0], 4, 110.0F, 30.0F, LaneAssociationType::RIGHT);
    init_vehicle(vehicles.vehicles_right_lane[1], 5, 90.0F, -30.0F, LaneAssociationType::RIGHT);
}

void print_vehicle(const VehicleType &vehicle)
{
    if (EGO_VEHICLE_ID == vehicle.id)
    {
        std::cout << "Ego Vehicle: \n";
        std::cout << "Speed (m/s): " << vehicle.speed_mps << "\n";
    }
    else
    {
        std::cout << "ID: " << vehicle.id << "\n";
        std::cout << "Speed (m/s): " << vehicle.speed_mps << "\n";
        std::cout << "Distance (m): " << vehicle.distance_m << "\n";
    }
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

void print_scene(const VehicleType &ego_vehicle, const NeighborVehiclesType &vehicles)
{
    std::cout << "    \t  L    C    R  \n";

    std::size_t left_idx = 0;
    std::size_t center_idx = 0;
    std::size_t right_idx = 0;

    const std::int32_t offset_m = 10;
    const std::int32_t view_range_m = static_cast<std::int32_t>(VIEW_RANGE_M);

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

        const float range_m = static_cast<float>(i);

        if ((ego_string != nullptr) && (range_m >= ego_vehicle.distance_m) &&
            (ego_vehicle.distance_m > (range_m - offset_m)))
        {
            ego_string[1] = 'E';
        }

        if ((left_vehicle != nullptr) && (range_m >= left_vehicle->distance_m) &&
            (left_vehicle->distance_m > (range_m - offset_m)))
        {
            left_string[1] = 'V';
            left_idx++;
        }
        else if ((left_vehicle != nullptr) && (std::abs(left_vehicle->distance_m) > VIEW_RANGE_M))
        {
            left_idx++;
        }

        if ((center_vehicle != nullptr) && (range_m >= center_vehicle->distance_m) &&
            (center_vehicle->distance_m > (range_m - offset_m)))
        {
            center_string[1] = 'V';
            center_idx++;
        }
        else if ((center_vehicle != nullptr) && (std::abs(center_vehicle->distance_m) > VIEW_RANGE_M))
        {
            center_idx++;
        }

        if ((right_vehicle != nullptr) && (range_m >= right_vehicle->distance_m) &&
            (right_vehicle->distance_m > (range_m - offset_m)))
        {
            right_string[1] = 'V';
            right_idx++;
        }
        else if ((right_vehicle != nullptr) && (std::abs(right_vehicle->distance_m) > VIEW_RANGE_M))
        {
            right_idx++;
        }

        std::cout << i << "\t| " << left_string << " |" << center_string << " |" << right_string << " |\n";
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

void compute_future_distance(VehicleType &vehicle, const float ego_driven_distance_m, const float seconds)
{
    const float driven_distance_m = vehicle.speed_mps * seconds;

    vehicle.distance_m += driven_distance_m - ego_driven_distance_m;
}

void compute_future_state(const VehicleType &ego_vehicle, NeighborVehiclesType &vehicles, const float seconds)
{
    const float ego_driven_distance_m = ego_vehicle.speed_mps * seconds;

    compute_future_distance(vehicles.vehicles_left_lane[0], ego_driven_distance_m, seconds);
    compute_future_distance(vehicles.vehicles_left_lane[1], ego_driven_distance_m, seconds);
    compute_future_distance(vehicles.vehicles_center_lane[0], ego_driven_distance_m, seconds);
    compute_future_distance(vehicles.vehicles_center_lane[1], ego_driven_distance_m, seconds);
    compute_future_distance(vehicles.vehicles_right_lane[0], ego_driven_distance_m, seconds);
    compute_future_distance(vehicles.vehicles_right_lane[1], ego_driven_distance_m, seconds);
}

void decrease_speed(VehicleType &ego_vehicle)
{
    const float decrease = ego_vehicle.speed_mps * LONGITUDINAL_DIFFERENCE_PERCENTAGE;

    if (ego_vehicle.speed_mps - decrease >= 0.0F)
    {
        ego_vehicle.speed_mps -= decrease;
    }
}

void longitudinal_control(const VehicleType &front_vehicle, VehicleType &ego_vehicle)
{
    const float minimal_distance_m = mps_to_kph(ego_vehicle.speed_mps) / 2.0f;
    const float front_distance_m = front_vehicle.distance_m;

    if (front_distance_m < minimal_distance_m)
    {
        decrease_speed(ego_vehicle);
    }
}

const VehicleType *get_vehicle_array(const LaneAssociationType lane, const NeighborVehiclesType &vehicles)
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

LaneAssociationType get_lane_change_request(const VehicleType &ego_vehicle, const NeighborVehiclesType &vehicles)
{
    const VehicleType *ego_lane_vehicles = get_vehicle_array(ego_vehicle.lane, vehicles);
    const VehicleType *rear_vehicle = &ego_lane_vehicles[1];

    const float minimal_distance_m = mps_to_kph(ego_vehicle.speed_mps) / 5.0F;
    const float rear_distance_m = std::abs(rear_vehicle->distance_m);

    if (rear_distance_m < minimal_distance_m)
    {
        switch (ego_vehicle.lane)
        {
        case LaneAssociationType::RIGHT: /* fall-thorugh */
        case LaneAssociationType::LEFT:
        {
            const LaneAssociationType target_lane = LaneAssociationType::CENTER;
            const VehicleType *center_vehicles = get_vehicle_array(target_lane, vehicles);

            const float abs_front_center_distance_m = std::abs(center_vehicles[0].distance_m);
            const float abs_rear_center_distance_m = std::abs(center_vehicles[1].distance_m);

            if ((abs_front_center_distance_m > minimal_distance_m) && (abs_rear_center_distance_m > minimal_distance_m))
            {
                return target_lane;
            }

            break;
        }
        case LaneAssociationType::CENTER:
        {
            LaneAssociationType target_lane = LaneAssociationType::RIGHT;

            const VehicleType *right_vehicles = get_vehicle_array(target_lane, vehicles);

            const float abs_front_right_distance_m = std::abs(right_vehicles[0].distance_m);
            const float abs_rear_right_distance_m = std::abs(right_vehicles[1].distance_m);

            if ((abs_front_right_distance_m > minimal_distance_m) && (abs_rear_right_distance_m > minimal_distance_m))
            {
                return target_lane;
            }

            target_lane = LaneAssociationType::LEFT;
            const VehicleType *left_vehicles = get_vehicle_array(target_lane, vehicles);

            const float abs_front_left_distance_m = std::abs(left_vehicles[0].distance_m);
            const float abs_rear_left_distance_m = std::abs(left_vehicles[1].distance_m);

            if ((abs_front_left_distance_m > minimal_distance_m) && (abs_rear_left_distance_m > minimal_distance_m))
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
