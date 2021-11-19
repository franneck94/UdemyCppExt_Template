#pragma once

#include "imgui.h"
#include "implot.h"

constexpr auto WINDOW_FLAGS_CLEAN =
    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize;
constexpr auto PLOT_FLAGS = ImPlotFlags_NoTitle | ImPlotFlags_NoLegend;

constexpr auto WINDOWS_HEIGHT{720.0F};
constexpr auto WINDOWS_WIDTH{1280.0F};

constexpr auto LEFT_LANE_OFFSET_M{-3.0F};
constexpr auto CENTER_LANE_OFFSET_M{0.0F};
constexpr auto RIGHT_LANE_OFFSET_M{3.0F};
constexpr auto LANE_PLOT_HEIGHT{150.0F};
constexpr auto LANE_PLOT_WIDTH = WINDOWS_WIDTH - 15.0F;
constexpr auto LANE_PLOT_TOTAL_HEIGHT = LANE_PLOT_HEIGHT * 3.0F + 10.0F;
constexpr auto BELOW_LANES = LANE_PLOT_TOTAL_HEIGHT + 10.0F;
constexpr auto VEHICLE_SCATTER_SIZE{15.0F};
const auto PLOT_DIM = ImVec2(LANE_PLOT_WIDTH, LANE_PLOT_HEIGHT);

constexpr auto VEHICLE_TABLE_HEIGHT = WINDOWS_HEIGHT - LANE_PLOT_TOTAL_HEIGHT;
constexpr auto VEHICLE_TABLE_WIDTH = WINDOWS_WIDTH;

constexpr auto CYCLE_NUMBER_WIDTH{200.0F};
constexpr auto CYCLE_NUMBER_HEIGHT{50.0F};
constexpr auto CYCLE_OFFSET{200.0F};

constexpr auto BUTTON_WIDTH{100.0F};
constexpr auto BUTTON_HEIGHT{50.0F};
constexpr auto BUTTON_LINE_SHIFT{200.0F};
constexpr auto BUTTON_OFFSET1{200.0F};
constexpr auto BUTTON_OFFSET2{50.0F};
