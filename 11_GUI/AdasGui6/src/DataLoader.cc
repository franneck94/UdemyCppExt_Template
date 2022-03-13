#include <cmath>
#include <fstream>
#include <vector>

#include "AdTypes.hpp"
#include "DataLoader.hpp"
#include "DataLoaderConstants.hpp"
#include "DataLoaderTypes.hpp"
#include "MathUtil.hpp"

static auto ego_vehicle_log_data = EgoVehicleLogData{};
static auto vehicles_log_data = VehiclesLogData{};
static auto lanes_log_data = LanesLogData{};

void compute_velocities(const std::array<float, NUM_ITERATIONS> long_velocities_mps,
                        const std::array<float, NUM_ITERATIONS> lat_velocities_mps,
                        std::array<float, NUM_ITERATIONS> &velocities_mps)
{
    std::transform(long_velocities_mps.begin(),
                   long_velocities_mps.end(),
                   lat_velocities_mps.begin(),
                   velocities_mps.begin(),
                   [&](const auto v_long, const auto v_lat) {
                       return std::sqrt(std::pow(v_long, 2.0F) + std::pow(v_lat, 2.0F));
                   });
}

void compute_heading_degrees(const std::array<float, NUM_ITERATIONS> long_velocities_mps,
                             const std::array<float, NUM_ITERATIONS> lat_velocities_mps,
                             std::array<float, NUM_ITERATIONS> &heading_degrees)
{
    std::transform(
        long_velocities_mps.begin(),
        long_velocities_mps.end(),
        lat_velocities_mps.begin(),
        heading_degrees.begin(),
        [&](const auto v_long, const auto v_lat) { return (std::atan2(v_lat, v_long) / PI<float>)*180.0F; });
}

void compute_accelerations(const std::array<float, NUM_ITERATIONS> &velocities_mps,
                           std::array<float, NUM_ITERATIONS> &accelerations_mps2)
{
    accelerations_mps2[0] = 0.0F;

    for (std::size_t i = 1; i < NUM_ITERATIONS; ++i)
    {
        accelerations_mps2[i] = (velocities_mps[i] - velocities_mps[i - 1]) / CYCLE_TIME_S;
    }
}

void load_cycle_ego(const std::uint32_t cycle, VehicleInformationType &ego_vehicle)
{
    ego_vehicle.id = EGO_VEHICLE_ID;
    ego_vehicle.long_distance_m = 0.0F;
    ego_vehicle.lat_distance_m = 0.0F;
    ego_vehicle.width_m = EGO_VEHICLE_WIDTH_M;
    ego_vehicle.height_m = EGO_VEHICLE_HEIGHT_M;
    ego_vehicle.object_class = ObjectClassType::CAR;

    ego_vehicle.lane = ego_vehicle_log_data.lanes[cycle];
    ego_vehicle.long_velocity_mps = ego_vehicle_log_data.long_velocities_mps[cycle];
    ego_vehicle.lat_velocity_mps = ego_vehicle_log_data.lat_velocities_mps[cycle];
    ego_vehicle.velocity_mps = ego_vehicle_log_data.velocities_mps[cycle];
    ego_vehicle.heading_deg = ego_vehicle_log_data.heading_degs[cycle];
    ego_vehicle.acceleration_mps2 = ego_vehicle_log_data.accelerations_mps2[cycle];
}

void init_ego_vehicle(std::string_view filepath, VehicleInformationType &ego_vehicle)
{
    std::ifstream ifs(filepath.data());
    json parsed_data = json::parse(ifs);

    ego_vehicle_log_data.lanes = parsed_data["Lane"];
    ego_vehicle_log_data.long_velocities_mps = parsed_data["LongVelocity"];
    ego_vehicle_log_data.lat_velocities_mps = parsed_data["LatVelocity"];

    compute_velocities(ego_vehicle_log_data.long_velocities_mps,
                       ego_vehicle_log_data.lat_velocities_mps,
                       ego_vehicle_log_data.velocities_mps);
    compute_heading_degrees(ego_vehicle_log_data.long_velocities_mps,
                            ego_vehicle_log_data.lat_velocities_mps,
                            ego_vehicle_log_data.heading_degs);
    compute_accelerations(ego_vehicle_log_data.velocities_mps, ego_vehicle_log_data.accelerations_mps2);

    load_cycle_ego(0, ego_vehicle);
}

void load_cycle_vehicle(const std::uint32_t cycle,
                        const std::uint32_t vehicle_idx,
                        std::array<VehicleInformationType, MAX_NUM_VEHICLES> &vehicles)
{
    vehicles[vehicle_idx].id = vehicles_log_data[vehicle_idx].id;
    vehicles[vehicle_idx].object_class = vehicles_log_data[vehicle_idx].object_class;
    vehicles[vehicle_idx].height_m = vehicles_log_data[vehicle_idx].height_m;
    vehicles[vehicle_idx].width_m = vehicles_log_data[vehicle_idx].width_m;

    vehicles[vehicle_idx].lane = vehicles_log_data[vehicle_idx].lanes[cycle];
    vehicles[vehicle_idx].long_velocity_mps = vehicles_log_data[vehicle_idx].long_velocities_mps[cycle];
    vehicles[vehicle_idx].lat_velocity_mps = vehicles_log_data[vehicle_idx].lat_velocities_mps[cycle];
    vehicles[vehicle_idx].velocity_mps = vehicles_log_data[vehicle_idx].velocities_mps[cycle];
    vehicles[vehicle_idx].long_distance_m = vehicles_log_data[vehicle_idx].long_distances_m[cycle];
    vehicles[vehicle_idx].lat_distance_m = vehicles_log_data[vehicle_idx].lat_distances_m[cycle];
    vehicles[vehicle_idx].heading_deg = vehicles_log_data[vehicle_idx].heading_degs[cycle];
    vehicles[vehicle_idx].acceleration_mps2 = vehicles_log_data[vehicle_idx].accelerations_mps2[cycle];
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
        vehicle_log_data.object_class = vehicle_data["ObjectClass"];
        vehicle_log_data.width_m = vehicle_data["Width"];
        vehicle_log_data.height_m = vehicle_data["Height"];

        vehicle_log_data.lanes = vehicle_data["Lane"];
        vehicle_log_data.long_velocities_mps = vehicle_data["LongVelocity"];
        vehicle_log_data.lat_velocities_mps = vehicle_data["LatVelocity"];
        vehicle_log_data.long_distances_m = vehicle_data["LongDistance"];
        vehicle_log_data.lat_distances_m = vehicle_data["LatDistance"];

        compute_velocities(vehicle_log_data.long_velocities_mps,
                           vehicle_log_data.lat_velocities_mps,
                           vehicle_log_data.velocities_mps);
        compute_heading_degrees(vehicle_log_data.long_velocities_mps,
                                vehicle_log_data.lat_velocities_mps,
                                vehicle_log_data.heading_degs);
        compute_accelerations(vehicle_log_data.velocities_mps, vehicle_log_data.accelerations_mps2);

        load_cycle_vehicle(0, vehicle_idx, vehicles);
    }
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

    load_cycle_lane(0, 0, lanes.left_lane);
    load_cycle_lane(0, 1, lanes.center_lane);
    load_cycle_lane(0, 2, lanes.right_lane);
}

void load_cycle(const std::uint32_t cycle,
                NeighborVehiclesType &vehicles,
                VehicleInformationType &ego_vehicle,
                LanesInformationType &lanes)
{
    for (std::uint32_t vehicle_idx = 0; vehicle_idx < MAX_NUM_VEHICLES; ++vehicle_idx)
    {
        load_cycle_vehicle(cycle, vehicle_idx, vehicles);
    }

    load_cycle_ego(cycle, ego_vehicle);

    load_cycle_lane(cycle, 0, lanes.left_lane);
    load_cycle_lane(cycle, 1, lanes.center_lane);
    load_cycle_lane(cycle, 2, lanes.right_lane);
}
