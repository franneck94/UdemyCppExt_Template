// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs
#include <chrono>
#include <filesystem>
#include <iostream>
#include <string>
#include <string_view>
#include <thread>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_opengl3_loader.h"
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include "AdFunctions.hpp"
#include "AdTypes.hpp"
#include "DataLoader.hpp"
#include "DataLoaderConstants.hpp"
#include "DataLoaderTypes.hpp"
#include "Render.hpp"
#include "RenderConstants.hpp"

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

namespace fs = std::filesystem;

static void glfw_error_callback(int error, const char *description);

static void start_cycle();

static void end_cycle(GLFWwindow *const window);

static void reset_state(const fs::path &ego_filepath,
                        const fs::path &data_filepath,
                        std::uint32_t &cycle,
                        VehicleType &ego_vehicle,
                        NeighborVehiclesType &vehicles)
{
    cycle = 0;
    init_ego_vehicle(ego_filepath.string(), ego_vehicle);
    init_vehicles(data_filepath.string(), vehicles);
}

static void cycle(const fs::path &ego_filepath, const fs::path &data_filepath, GLFWwindow *const window)
{
    static bool is_playing = false;
    static bool pressed_play = false;
    static bool pressed_pause = false;
    static bool pressed_replay = false;

    constexpr std::int64_t sleep_time = 50;
    std::uint32_t cycle = 0;
    VehicleType ego_vehicle{};
    NeighborVehiclesType vehicles{};

    init_ego_vehicle(ego_filepath.string(), ego_vehicle);
    init_vehicles(data_filepath.string(), vehicles);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        start_cycle();

        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        if (ImGui::Begin("ButtonWindow",
                         nullptr,
                         ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground |
                             ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar))
        {
            if (ImGui::Button("Play", ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT)))
            {
                pressed_play = true;
            }
            ImGui::SameLine(1 * BUTTON_LINE_SHIFT);
            if (ImGui::Button("Pause", ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT)))
            {
                pressed_pause = true;
            }
            ImGui::SameLine(2 * BUTTON_LINE_SHIFT);
            if (ImGui::Button("Replay", ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT)))
            {
                pressed_replay = true;
            }

            ImGui::End();
        }

        if (pressed_play)
        {
            if (cycle == 0 && cycle >= NUM_ITERATIONS)
            {
                reset_state(ego_filepath, data_filepath, cycle, ego_vehicle, vehicles);
            }

            pressed_pause = false;
            pressed_play = false;
            is_playing = true;
        }

        if (pressed_replay)
        {
            reset_state(ego_filepath, data_filepath, cycle, ego_vehicle, vehicles);

            pressed_replay = false;
            is_playing = true;
        }

        if (!pressed_pause && is_playing && cycle < NUM_ITERATIONS)
        {
            render_cycle(cycle, ego_vehicle, vehicles);

            compute_future_state(ego_vehicle, vehicles, 0.050F);
            const auto lane_change_request = longitudinal_control(vehicles, ego_vehicle);
            const auto lane_change_successful = lateral_control(vehicles, lane_change_request, ego_vehicle);

            if (lane_change_request != ego_vehicle.lane)
            {
                std::cout << "Lane change request: " << static_cast<int>(lane_change_request) << std::endl;
            }
            if (lane_change_successful)
            {
                std::cout << "Lane change successull" << std::endl;
            }

            cycle++;
            load_cycle(cycle, vehicles);
        }
        else if (pressed_pause)
        {
            render_cycle(cycle, ego_vehicle, vehicles);
            is_playing = false;
        }
        else if (!is_playing)
        {
            render_cycle(0, ego_vehicle, vehicles);
        }
        else if (cycle >= NUM_ITERATIONS)
        {
            is_playing = false;
        }

        ImGui::Render();

        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
        ImGui::GetIO().DeltaTime = static_cast<float>(sleep_time) / 1000.0F;

        end_cycle(window);

        if (is_playing)
        {
            cycle++;
        }
    }
}

int main(int argc, char **argv)
{
    fs::path data_filepath;
    fs::path ego_filepath;

    if (argc != 3)
    {
        data_filepath /= fs::current_path().parent_path().parent_path();
        data_filepath /= "data";
        ego_filepath = data_filepath;

        data_filepath /= "vehicle_data.json";
        ego_filepath /= "ego_data.json";
    }
    else
    {
        auto vehicles_input_path = std::string(argv[1]);
        data_filepath = fs::path(vehicles_input_path);

        auto ego_input_path = std::string(argv[2]);
        ego_filepath = fs::path(ego_input_path);
    }

    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

        // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char *glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char *glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow *window =
        glfwCreateWindow(static_cast<int>(WINDOWS_WIDTH), static_cast<int>(WINDOWS_HEIGHT), "AdasGui", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImGuiStyle &style = ImGui::GetStyle();
    style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(1.0, 1.0, 1.0, 1.0);
    style.Colors[ImGuiCol_TableBorderLight] = ImVec4(1.0, 1.0, 1.0, 1.0);

    cycle(ego_filepath, data_filepath, window);

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static void start_cycle()
{
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
}

static void end_cycle(GLFWwindow *const window)
{
    ImVec4 clear_color = ImVec4(30.0F / 255.0F, 30.0F / 255.0F, 30.0F / 255.0F, 1.00f);
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w,
                 clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w,
                 clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
}
