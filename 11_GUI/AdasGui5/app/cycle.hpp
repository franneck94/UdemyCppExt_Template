#pragma once

#include <filesystem>

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

#include "AdTypes.hpp"
#include "DataLoaderTypes.hpp"
#include "Render.hpp"

namespace fs = std::filesystem;

void cycle_function(const fs::path &ego_filepath,
                    const fs::path &vehicle_filepath,
                    const fs::path &lane_filepath,
                    GLFWwindow *const window);

static void execute_cycle(const std::size_t cycle,
                          VehicleInformationType &ego_vehicle,
                          NeighborVehiclesType &vehicles,
                          LanesInformationType &lanes);

static void reset_state(const fs::path &ego_filepath,
                        const fs::path &vehicle_filepath,
                        const fs::path &lanes_filepath,
                        std::size_t &cycle,
                        VehicleInformationType &ego_vehicle,
                        NeighborVehiclesType &vehicles,
                        LanesInformationType &lanes);

static void start_cycle();

static void end_cycle(GLFWwindow *const window);

static void plot_cycle_number(const std::size_t cycle);

static void plot_buttons();
