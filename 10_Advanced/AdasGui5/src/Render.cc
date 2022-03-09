#include <algorithm>
#include <array>
#include <cstdint>
#include <string_view>

#include "imgui.h"
#include "implot.h"

#include "DataLoader.hpp"
#include "DataLoaderConstants.hpp"
#include "Render.hpp"
#include "RenderConstants.hpp"

void render_cycle(const VehicleInformationType &ego_vehicle,
                  const NeighborVehiclesType &vehicles,
                  const LanesInformationType &lanes,
                  const bool long_request,
                  const LaneAssociationType lat_request)
{
    ImGui::SetNextWindowPos(ImVec2(0.0F, 0.0F));
    ImGui::SetNextWindowSize(ImVec2(WINDOWS_WIDTH, LANE_PLOT_TOTAL_HEIGHT));

    if (ImGui::Begin("MainWindow", nullptr, WINDOW_FLAGS_CLEAN))
    {
        ImPlot::CreateContext();
        plot_lanes(ego_vehicle, vehicles, lanes, long_request, lat_request);
        plot_table(ego_vehicle, vehicles);
        ImGui::End();
    }
}

void plot_lanes_straight_solid_line(const Polynomial3rdDegreeType &polynomial,
                                    const float start_m,
                                    const float end_m)
{
    const auto num_rear_points = std::size_t{2};

    const auto x_rear = std::array<float, num_rear_points>{start_m, end_m};
    const auto y_rear = std::array<float, num_rear_points>{polynomial.d, polynomial.d};
    ImPlot::SetNextLineStyle(WHITE_MARKER, REAR_LINE_WIDTH);
    ImPlot::PlotLine("straight_solid_line", x_rear.data(), y_rear.data(), num_rear_points);
}

void plot_lanes_polynomial_solid_line(const Polynomial3rdDegreeType &polynomial,
                                      const float start_m,
                                      const float end_m)
{
    const auto num_front_points = std::size_t{100};
    const auto range_m = std::abs(end_m - start_m);
    const auto slice_length_m = range_m / static_cast<float>(num_front_points);

    auto xs = std::array<float, num_front_points>{};
    auto ys = std::array<float, num_front_points>{};

    for (std::int32_t slice_idx = 0; slice_idx < num_front_points; ++slice_idx)
    {
        const auto x = static_cast<float>(slice_idx) * slice_length_m;
        xs[slice_idx] = x;
        ys[slice_idx] = polynomial(x);
    }

    ImPlot::SetNextLineStyle(WHITE_MARKER, LINE_WIDTH);
    ImPlot::PlotLine("###polynomial_solid_line", xs.data(), ys.data(), num_front_points);
}

void plot_lanes_polynomial_dashed_line(const Polynomial3rdDegreeType &polynomial,
                                       const float start_m,
                                       const float end_m)
{
    const auto num_points = std::size_t{2};
    const auto range_m = std::abs(end_m - start_m);
    const auto num_slices = static_cast<std::uint32_t>(range_m / SLICE_LENGTH_M);

    for (std::uint32_t slice_idx = 0; slice_idx < num_slices; slice_idx++)
    {
        const auto slice_m = static_cast<float>(slice_idx);

        const auto xs = std::array<float, num_points>{start_m + slice_m * SLICE_LENGTH_M,
                                                      start_m + slice_m * SLICE_LENGTH_M + DASHED_LENGTH_M};
        const auto ys = std::array<float, num_points>{polynomial(xs[0]), polynomial(xs[1])};

        ImPlot::SetNextLineStyle(WHITE_MARKER, LINE_WIDTH);
        ImPlot::PlotLine("###polynomial_dashed_line", xs.data(), ys.data(), num_points);
    }
}

void plot_lane_boundary(const Polynomial3rdDegreeType &polynomial,
                        const LaneBoundaryType boundary_type,
                        const float view_range_m)
{
    plot_lanes_straight_solid_line(polynomial, -VIEW_RANGE_M, 0.0F);

    switch (boundary_type)
    {
    case LaneBoundaryType::DASHED:
    {
        plot_lanes_polynomial_dashed_line(polynomial, 0.0F, view_range_m);
        break;
    }
    case LaneBoundaryType::SOLID:
    {
        plot_lanes_polynomial_solid_line(polynomial, 0.0F, view_range_m);
        break;
    }
    default:
    {
        return;
    }
    }
}


void plot_lane_class()
{
    // TODO: Lane class shaded area
}

void plot_lanes_vehicles(const std::array<VehicleInformationType, MAX_NUM_VEHICLES> &vehicles)
{
    const auto num_elements = std::size_t{1};

    for (std::size_t i = 0; i < MAX_NUM_VEHICLES; i++)
    {
        const auto xs = vehicles[i].long_distance_m;
        const auto ys = vehicles[i].lat_distance_m;

        // TODO
        auto color = WHITE_MARKER;
        auto scatter_size = VEHICLE_SCATTER_SIZE;

        const auto label = std::string{"vehicle"} + std::to_string(i);

        ImPlot::SetNextMarkerStyle(VEHICLE_MARKER, scatter_size, color);
        ImPlot::PlotScatter(label.data(), &xs, &ys, num_elements);

        // TODO: ID plotting
    }
}

void plot_lanes_ego_vehicle(const VehicleInformationType &ego_vehicle,
                            const bool long_request,
                            const LaneAssociationType lat_request)
{
    ImPlot::SetNextMarkerStyle(VEHICLE_MARKER, VEHICLE_SCATTER_SIZE, WHITE_MARKER);
    ImPlot::PlotScatter("ego", &ego_vehicle.long_distance_m, &ego_vehicle.lat_distance_m, 1);

    // TODO: Plotting Long and Lat Request
    (void)long_request;
    (void)lat_request;
}

void plot_lanes(const VehicleInformationType &ego_vehicle,
                const NeighborVehiclesType &vehicles,
                const LanesInformationType &lanes,
                const bool long_request,
                const LaneAssociationType lat_request)
{
    if (ImPlot::BeginPlot("Lanes", PLOT_DIM, PLOT_FLAGS))
    {
        ImPlot::SetupAxes("Rel. LongDist. (m)", "Rel. LatDist. (m)", ImPlotAxisFlags_Lock, AXES_FLAGS);
        ImPlot::SetupAxisLimits(ImAxis_X1, -VIEW_RANGE_M, VIEW_RANGE_M, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, -LATERAL_RANGE_M, LATERAL_RANGE_M, ImGuiCond_Always);

        plot_lane_class();
        plot_lane_class();
        plot_lane_class();

        plot_lane_boundary(lanes.left_lane.left_polynomial,
                           lanes.left_lane.left_boundary_type,
                           lanes.left_lane.left_view_range_m);
        plot_lane_boundary(lanes.center_lane.left_polynomial,
                           lanes.center_lane.left_boundary_type,
                           lanes.center_lane.left_view_range_m);
        plot_lane_boundary(lanes.center_lane.right_polynomial,
                           lanes.center_lane.right_boundary_type,
                           lanes.center_lane.right_view_range_m);
        plot_lane_boundary(lanes.right_lane.right_polynomial,
                           lanes.right_lane.right_boundary_type,
                           lanes.right_lane.right_view_range_m);

        plot_lanes_vehicles(vehicles);
        plot_lanes_ego_vehicle(ego_vehicle, long_request, lat_request);

        ImPlot::EndPlot();
    }
}

void plot_vehicle_in_table(const VehicleInformationType &vehicle)
{
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::Text("%d", vehicle.id);
    ImGui::TableNextColumn();
    ImGui::Text("%s", LANE_NAMES[static_cast<std::int32_t>(vehicle.lane)]);
    ImGui::TableNextColumn();
    ImGui::Text("%f", vehicle.long_distance_m);
    ImGui::TableNextColumn();
    ImGui::Text("%f", vehicle.lat_distance_m);
    ImGui::TableNextColumn();
    ImGui::Text("%f", vehicle.speed_mps);
}

void plot_table(const VehicleInformationType &ego_vehicle, const NeighborVehiclesType &vehicles)
{
    const auto num_cols = std::size_t{5};

    ImGui::SetNextWindowPos(ImVec2(0.0F, BELOW_LANES));
    ImGui::SetNextWindowSize(ImVec2(VEHICLE_TABLE_WIDTH, VEHICLE_TABLE_HEIGHT));

    if (ImGui::Begin("TableWindow", nullptr, WINDOW_FLAGS_CLEAN))
    {
        if (ImGui::BeginTable("Table", num_cols, TABLE_FLAGS))
        {
            ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
            ImGui::TableNextColumn();
            ImGui::Text("ID:");
            ImGui::TableNextColumn();
            ImGui::Text("Lane:");
            ImGui::TableNextColumn();
            ImGui::Text("Long. Dist.:");
            ImGui::TableNextColumn();
            ImGui::Text("Lat. Dist.:");
            ImGui::TableNextColumn();
            ImGui::Text("Speed:");

            plot_vehicle_in_table(ego_vehicle);

            for (std::uint32_t idx = 0; idx < MAX_NUM_VEHICLES; ++idx)
            {
                plot_vehicle_in_table(vehicles[idx]);
            }

            ImGui::EndTable();
        }

        ImGui::End();
    }
}
