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

float get_minimal_distance(const VehicleType &ego_vehicle)
{
    return mps_to_kph(ego_vehicle.speed_mps) / 2.0F;
}

float get_maximal_collision_distance(const VehicleType &ego_vehicle)
{
    return mps_to_kph(ego_vehicle.speed_mps) / 10.0F;
}

void compute_future_distance(VehicleType &vehicle, const float ego_driven_distance, const float seconds)
{
    const auto driven_distance = vehicle.speed_mps * seconds;
    vehicle.distance_m += driven_distance - ego_driven_distance;
}

void compute_future_state(const VehicleType &ego_vehicle, NeighborVehiclesType &vehicles, const float seconds)
{
    const auto ego_driven_distance = ego_vehicle.speed_mps * seconds;

    compute_future_distance(vehicles.vehicles_left_lane[0], ego_driven_distance, seconds);
    compute_future_distance(vehicles.vehicles_left_lane[1], ego_driven_distance, seconds);
    compute_future_distance(vehicles.vehicles_center_lane[0], ego_driven_distance, seconds);
    compute_future_distance(vehicles.vehicles_center_lane[1], ego_driven_distance, seconds);
    compute_future_distance(vehicles.vehicles_right_lane[0], ego_driven_distance, seconds);
    compute_future_distance(vehicles.vehicles_right_lane[1], ego_driven_distance, seconds);
}

void increase_speed(VehicleType &ego_vehicle)
{
    const auto increase = ego_vehicle.speed_mps * SPEED_ADAPTATION_FACTOR;

    if (ego_vehicle.speed_mps + increase < MAX_VEHICLE_SPEED_MPS)
    {
        ego_vehicle.speed_mps += increase;
    }
}

void decrease_speed(VehicleType &ego_vehicle)
{
    const auto decrease = ego_vehicle.speed_mps * SPEED_ADAPTATION_FACTOR;

    ego_vehicle.speed_mps -= decrease;
}

LaneAssociationType longitudinal_control(const NeighborVehiclesType &vehicles, VehicleType &ego_vehicle)
{
    const VehicleType *vehicles_array = nullptr;

    switch (ego_vehicle.lane)
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
        return ego_vehicle.lane;
        break;
    }
    }

    const auto &front_vehicle = vehicles_array[0];
    const auto &rear_vehicle = vehicles_array[1];

    auto rear_vehicle_too_close = false;
    auto front_vehicle_too_close = false;
    auto rear_vehicle_too_close_collision = false;
    auto front_vehicle_too_close_collision = false;

    const auto minimal_distance = get_minimal_distance(ego_vehicle);
    const auto maximal_distance_collision = get_maximal_collision_distance(ego_vehicle);

    const auto front_distance_abs = std::abs(front_vehicle.distance_m);
    const auto rear_distance_abs = std::abs(rear_vehicle.distance_m);

    if (front_distance_abs < minimal_distance)
    {
        front_vehicle_too_close = true;
    }

    if (rear_distance_abs < minimal_distance)
    {
        rear_vehicle_too_close = true;
    }

    if (front_distance_abs < maximal_distance_collision)
    {
        front_vehicle_too_close_collision = true;
    }

    if (rear_distance_abs < maximal_distance_collision)
    {
        rear_vehicle_too_close_collision = true;
    }

    const auto collision_incomming = (front_vehicle_too_close_collision || rear_vehicle_too_close_collision);

    if (!front_vehicle_too_close && !rear_vehicle_too_close && !collision_incomming)
    {
        return ego_vehicle.lane;
    }

    if (front_vehicle_too_close && !rear_vehicle_too_close)
    {
        decrease_speed(ego_vehicle);
    }
    else if (!front_vehicle_too_close && rear_vehicle_too_close)
    {
        increase_speed(ego_vehicle);
    }

    if (collision_incomming)
    {
        return get_lane_change_request(ego_vehicle,
                                       front_distance_abs,
                                       rear_distance_abs,
                                       front_vehicle_too_close_collision,
                                       rear_vehicle_too_close_collision);
    }

    return ego_vehicle.lane;
}

LaneAssociationType get_lane_change_request(const VehicleType &ego_vehicle,
                                            const float front_distance_abs,
                                            const float rear_distance_abs,
                                            const bool front_vehicle_too_close_collision,
                                            const bool rear_vehicle_too_close_collision)
{
    const auto left_lane_change = (front_distance_abs < rear_distance_abs);

    if (left_lane_change)
    {
        switch (ego_vehicle.lane)
        {
        case LaneAssociationType::LEFT:
        {
            if (front_vehicle_too_close_collision)
            {
                return LaneAssociationType::CENTER;
            }
            else
            {
                return ego_vehicle.lane; // stay in lane
            }
            break;
        }
        case LaneAssociationType::CENTER:
        {
            return LaneAssociationType::LEFT;
            break;
        }
        case LaneAssociationType::RIGHT:
        {
            return LaneAssociationType::CENTER;
            break;
        }
        default:
        {
            return ego_vehicle.lane;
            break;
        }
        }
    }
    else // to the right
    {
        switch (ego_vehicle.lane)
        {
        case LaneAssociationType::LEFT:
        {
            return LaneAssociationType::CENTER;
            break;
        }
        case LaneAssociationType::CENTER:
        {
            return LaneAssociationType::RIGHT;
            break;
        }
        case LaneAssociationType::RIGHT:
        {
            if (rear_vehicle_too_close_collision)
            {
                return LaneAssociationType::CENTER;
            }
            else
            {
                return ego_vehicle.lane; // stay in lane
            }
            break;
        }
        default:
        {
            return ego_vehicle.lane; // stay in lane
            break;
        }
        }
    }

    return ego_vehicle.lane;
}

bool gap_is_valid(const VehicleType &front_vehicle, const VehicleType &rear_vehicle, const VehicleType &ego_vehicle)
{
    const auto front_distance_abs = std::abs(front_vehicle.distance_m);
    const auto rear_distance_abs = std::abs(rear_vehicle.distance_m);

    const auto minimal_distance = get_minimal_distance(ego_vehicle);

    if ((minimal_distance < front_distance_abs) && (minimal_distance < rear_distance_abs))
    {
        return true;
    }

    return false;
}

bool lateral_control(const NeighborVehiclesType &vehicles,
                     const LaneAssociationType lane_change_request,
                     VehicleType &ego_vehicle)
{
    if (ego_vehicle.lane == lane_change_request)
    {
        return false;
    }

    const VehicleType *vehicle_array = nullptr;

    switch (lane_change_request)
    {
    case LaneAssociationType::LEFT:
    {
        vehicle_array = vehicles.vehicles_left_lane;
        break;
    }
    case LaneAssociationType::CENTER:
    {

        vehicle_array = vehicles.vehicles_center_lane;
        break;
    }
    case LaneAssociationType::RIGHT:
    {
        vehicle_array = vehicles.vehicles_right_lane;
        break;
    }
    default:
    {
        break;
    }
    }

    if (vehicle_array == nullptr)
    {
        return false;
    }

    const auto valid = gap_is_valid(vehicle_array[0], vehicle_array[1], ego_vehicle);

    if (valid)
    {
        ego_vehicle.lane = lane_change_request;

        return true;
    }

    return false;
}
