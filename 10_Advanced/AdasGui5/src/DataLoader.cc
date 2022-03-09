#include <fstream>
#include <vector>

#include "AdTypes.hpp"
#include "DataLoader.hpp"
#include "DataLoaderConstants.hpp"
#include "DataLoaderTypes.hpp"

static auto ego_vehicle_log_data = EgoVehicleLogData{};
static auto vehicles_log_data = VehiclesLogData{};
static auto lanes_log_data = LanesLogData{};

void set_ego_vehicle_start_data(VehicleInformationType &ego_vehicle)
{
    ego_vehicle.id = EGO_VEHICLE_ID;
    ego_vehicle.long_distance_m = 0.0F;
    ego_vehicle.lat_distance_m = 0.0F;
    ego_vehicle.width_m = EGO_VEHICLE_WIDTH_M;
    ego_vehicle.height_m = EGO_VEHICLE_HEIGHT_M;

    ego_vehicle.speed_mps = static_cast<float>(ego_vehicle_log_data.speeds_mps[0]);
    ego_vehicle.lane = static_cast<LaneAssociationType>(ego_vehicle_log_data.lanes[0]);
}

void init_ego_vehicle(std::string_view filepath, VehicleInformationType &ego_vehicle)
{
    std::ifstream ifs(filepath.data());
    json parsed_data = json::parse(ifs);

    ego_vehicle_log_data.lanes = parsed_data["Lane"];
    ego_vehicle_log_data.speeds_mps = parsed_data["Speed"];

    set_ego_vehicle_start_data(ego_vehicle);
}

void set_vehicle_start_data(VehicleInformationType &vehicle, const VehicleLogData &vehicle_log_data)
{
    vehicle.id = vehicle_log_data.id;
    vehicle.object_class = vehicle_log_data.object_class;
    vehicle.height_m = vehicle_log_data.height_m;
    vehicle.width_m = vehicle_log_data.width_m;

    vehicle.lane = vehicle_log_data.lanes[0];
    vehicle.long_distance_m = vehicle_log_data.long_distances_m[0];
    vehicle.lat_distance_m = vehicle_log_data.lat_distances_m[0];
    vehicle.speed_mps = vehicle_log_data.speeds_mps[0];
}

void init_vehicles(std::string_view filepath, NeighborVehiclesType &vehicles)
{
    std::ifstream ifs(filepath.data());
    json parsed_data = json::parse(ifs);

    for (std::size_t vehicle_idx = 0; vehicle_idx < NUM_VEHICLES; vehicle_idx++)
    {
        const auto &vehicle_data = parsed_data[std::to_string(vehicle_idx)];
        auto &vehicle_log_data = vehicles_log_data[vehicle_idx];

        vehicle_log_data.id = static_cast<std::int32_t>(vehicle_idx);
        vehicle_log_data.object_class = static_cast<ObjectClassType>(vehicle_data["ObjectClass"]);
        vehicle_log_data.width_m = static_cast<float>(vehicle_data["Width"]);
        vehicle_log_data.height_m = static_cast<float>(vehicle_data["Height"]);

        vehicle_log_data.lanes = vehicle_data["Lane"];
        vehicle_log_data.speeds_mps = vehicle_data["Speed"];
        vehicle_log_data.long_distances_m = vehicle_data["LongDistance"];
        vehicle_log_data.lat_distances_m = vehicle_data["LatDistance"];
    }

    set_vehicle_start_data(vehicles[0], vehicles_log_data[0]);
    set_vehicle_start_data(vehicles[1], vehicles_log_data[1]);
    set_vehicle_start_data(vehicles[2], vehicles_log_data[2]);
    set_vehicle_start_data(vehicles[3], vehicles_log_data[3]);
    set_vehicle_start_data(vehicles[4], vehicles_log_data[4]);
    set_vehicle_start_data(vehicles[5], vehicles_log_data[5]);
}

void load_cycle_vehicle(const std::uint32_t cycle,
                        const std::uint32_t vehicle_idx,
                        std::array<VehicleInformationType, MAX_NUM_VEHICLES> &vehicles)
{
    vehicles[vehicle_idx].speed_mps = vehicles_log_data[vehicle_idx].speeds_mps[cycle];
    vehicles[vehicle_idx].long_distance_m = vehicles_log_data[vehicle_idx].long_distances_m[cycle];
    vehicles[vehicle_idx].lat_distance_m = vehicles_log_data[vehicle_idx].lat_distances_m[cycle];
}

void load_cycle_lane(const std::uint32_t cycle, const std::uint32_t lane_idx, LaneInformationType &lane)
{
    lane.left_polynomial = lanes_log_data[lane_idx].left_polynomials[cycle];
    lane.left_view_range_m = lanes_log_data[lane_idx].left_view_ranges_m[cycle];
    lane.left_boundary_type = lanes_log_data[lane_idx].left_boundary_types[cycle];

    lane.right_polynomial = lanes_log_data[lane_idx].right_polynomials[cycle];
    lane.right_view_range_m = lanes_log_data[lane_idx].right_view_ranges_m[cycle];
    lane.right_boundary_type = lanes_log_data[lane_idx].right_boundary_types[cycle];

    lane.lane_class = lanes_log_data[lane_idx].lane_classes[cycle];
    lane.lane_width_m = lanes_log_data[lane_idx].lane_widths_m[cycle];
}

void load_cycle(const std::uint32_t cycle, NeighborVehiclesType &vehicles, LanesInformationType &lanes)
{
    for (std::uint32_t vehicle_idx = 0; vehicle_idx < MAX_NUM_VEHICLES; ++vehicle_idx)
    {
        load_cycle_vehicle(cycle, vehicle_idx, vehicles);
    }

    load_cycle_lane(cycle, 0, lanes.left_lane);
    load_cycle_lane(cycle, 1, lanes.center_lane);
    load_cycle_lane(cycle, 2, lanes.right_lane);
}

void get_lane_border_data(const std::uint32_t i, const size_t lane_idx, const json &parsed_data)
{
    const auto i_str = std::to_string(i);
    const auto lane_str = std::to_string(lane_idx);

    lanes_log_data[lane_idx].left_polynomials[i].a = parsed_data[lane_str]["0"][i_str]["p"]["a"];
    lanes_log_data[lane_idx].left_polynomials[i].b = parsed_data[lane_str]["0"][i_str]["p"]["b"];
    lanes_log_data[lane_idx].left_polynomials[i].c = parsed_data[lane_str]["0"][i_str]["p"]["c"];
    lanes_log_data[lane_idx].left_polynomials[i].d = parsed_data[lane_str]["0"][i_str]["p"]["d"];
    lanes_log_data[lane_idx].left_view_ranges_m[i] = parsed_data[lane_str]["0"][i_str]["r"];
    lanes_log_data[lane_idx].left_boundary_types[i] = parsed_data[lane_str]["0"][i_str]["t"];

    lanes_log_data[lane_idx].right_polynomials[i].a = parsed_data[lane_str]["1"][i_str]["p"]["a"];
    lanes_log_data[lane_idx].right_polynomials[i].b = parsed_data[lane_str]["1"][i_str]["p"]["b"];
    lanes_log_data[lane_idx].right_polynomials[i].c = parsed_data[lane_str]["1"][i_str]["p"]["c"];
    lanes_log_data[lane_idx].right_polynomials[i].d = parsed_data[lane_str]["1"][i_str]["p"]["d"];
    lanes_log_data[lane_idx].right_view_ranges_m[i] = parsed_data[lane_str]["1"][i_str]["r"];
    lanes_log_data[lane_idx].right_boundary_types[i] = parsed_data[lane_str]["1"][i_str]["t"];

    lanes_log_data[lane_idx].lane_classes[i] = parsed_data[lane_str]["1"][i_str]["c"];
    lanes_log_data[lane_idx].lane_widths_m[i] = std::abs(lanes_log_data[lane_idx].left_polynomials[i].d -
                                                         lanes_log_data[lane_idx].right_polynomials[i].d);
}

void set_lanes_start_data(LaneInformationType &lane, const size_t lane_idx)
{
    lane.left_polynomial = lanes_log_data[lane_idx].left_polynomials[0];
    lane.left_view_range_m = lanes_log_data[lane_idx].left_view_ranges_m[0];
    lane.left_boundary_type = lanes_log_data[lane_idx].left_boundary_types[0];

    lane.right_polynomial = lanes_log_data[lane_idx].right_polynomials[0];
    lane.right_view_range_m = lanes_log_data[lane_idx].right_view_ranges_m[0];
    lane.right_boundary_type = lanes_log_data[lane_idx].right_boundary_types[0];

    lane.lane_class = lanes_log_data[lane_idx].lane_classes[0];
    lane.lane_width_m = lanes_log_data[lane_idx].lane_widths_m[0];
}

void init_lanes(std::string_view filepath, LanesInformationType &lanes)
{
    std::ifstream ifs(filepath.data());
    json parsed_data = json::parse(ifs);

    for (std::uint32_t i = 0; i < NUM_ITERATIONS; ++i)
    {
        get_lane_border_data(i, 0, parsed_data);
        get_lane_border_data(i, 0, parsed_data);
        get_lane_border_data(i, 1, parsed_data);
        get_lane_border_data(i, 1, parsed_data);
        get_lane_border_data(i, 2, parsed_data);
        get_lane_border_data(i, 2, parsed_data);
    }

    set_lanes_start_data(lanes.left_lane, 0);
    set_lanes_start_data(lanes.center_lane, 1);
    set_lanes_start_data(lanes.right_lane, 2);
}
