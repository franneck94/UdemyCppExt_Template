#pragma once

#include "imgui.h"

constexpr auto WINDOW_FLAGS_CLEAN =
    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize;

constexpr auto WINDOWS_HEIGHT{720.0F};
constexpr auto WINDOWS_WIDTH{1280.0F};

constexpr auto BUTTON_WIDTH{100.0F};
constexpr auto BUTTON_HEIGHT{50.0F};
constexpr auto BUTTON_LINE_SHIFT{200.0F};
constexpr auto BUTTON_OFFSET1{200.0F};
constexpr auto BUTTON_OFFSET2{50.0F};
