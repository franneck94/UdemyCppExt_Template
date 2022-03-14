#include <algorithm>
#include <iostream>
#include <vector>

#include "AdTypes.hpp"
#include "DataLoaderConstants.hpp"
#include "DataLoaderTypes.hpp"
#include "Perception.hpp"

void compute_velocities(const std::array<float, NUM_ITERATIONS> long_velocities_mps,
                        const std::array<float, NUM_ITERATIONS> lat_velocities_mps,
                        std::array<float, NUM_ITERATIONS> &velocities_mps)
{
    (void)long_velocities_mps;
    (void)lat_velocities_mps;
    (void)velocities_mps;
}

void compute_heading_degrees(const std::array<float, NUM_ITERATIONS> long_velocities_mps,
                             const std::array<float, NUM_ITERATIONS> lat_velocities_mps,
                             std::array<float, NUM_ITERATIONS> &heading_degrees)
{
    (void)long_velocities_mps;
    (void)lat_velocities_mps;
    (void)heading_degrees;
}

void compute_accelerations(const std::array<float, NUM_ITERATIONS> &velocities_mps,
                           std::array<float, NUM_ITERATIONS> &accelerations_mps2)
{
    (void)velocities_mps;
    (void)accelerations_mps2;
}

void compute_rel_velocities(const std::array<float, NUM_ITERATIONS> &velocities_mps,
                            const std::array<float, NUM_ITERATIONS> &ego_velocities_mps,
                            std::array<float, NUM_ITERATIONS> &rel_velocities_mps)
{
    (void)velocities_mps;
    (void)ego_velocities_mps;
    (void)rel_velocities_mps;
}

void compute_rel_accelerations(const std::array<float, NUM_ITERATIONS> &accelerations_mps2,
                               const std::array<float, NUM_ITERATIONS> &ego_accelerations_mps2,
                               std::array<float, NUM_ITERATIONS> &rel_accelerations_mps2)
{
    (void)accelerations_mps2;
    (void)ego_accelerations_mps2;
    (void)rel_accelerations_mps2;
}

void compute_ego_kinematics(EgoVehicleLogData &ego_data)
{
    compute_velocities(ego_data.long_velocities_mps, ego_data.lat_velocities_mps, ego_data.velocities_mps);
    compute_heading_degrees(ego_data.long_velocities_mps, ego_data.lat_velocities_mps, ego_data.heading_degs);
    compute_accelerations(ego_data.velocities_mps, ego_data.accelerations_mps2);
}

void compute_vehicle_kinematics(const EgoVehicleLogData &ego_data, VehicleLogData &vehicle_data)
{
    compute_velocities(vehicle_data.long_velocities_mps,
                       vehicle_data.lat_velocities_mps,
                       vehicle_data.velocities_mps);
    compute_heading_degrees(vehicle_data.long_velocities_mps,
                            vehicle_data.lat_velocities_mps,
                            vehicle_data.heading_degs);
    compute_accelerations(vehicle_data.velocities_mps, vehicle_data.accelerations_mps2);
    compute_rel_velocities(vehicle_data.velocities_mps,
                           ego_data.velocities_mps,
                           vehicle_data.rel_velocities_mps);
    compute_rel_accelerations(vehicle_data.accelerations_mps2,
                              ego_data.accelerations_mps2,
                              vehicle_data.rel_accelerations_mps2);
}
