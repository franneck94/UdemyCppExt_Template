# Dear ImGUI Template to Start

## Example

![Example](./media/AdasGui.gif)

## Exercise

The new code will be added to the following files

- **Render.cpp**
- **Render.hpp**
- **RenderConstants.cpp**

The GUI should display the following features:

- Scatter Plot with ImPlot
  - Plot the position of the vehicles
- Tabular Plot with ImPlot
  - Plot the ID, distance and speed of all vehicles

```cpp
// Scatter Plot
if (ImPlot::BeginPlot("##Lanes", PLOT_DIM, PLOT_FLAGS))
{
    ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_Lock, ImPlotAxisFlags_Lock | ImPlotAxisFlags_Invert);
    ImPlot::SetupAxisLimits(ImAxis_X1, -MAX_VIEW_RANGE_M, MAX_VIEW_RANGE_M, ImGuiCond_Always);
    ImPlot::SetupAxisLimits(ImAxis_Y1, LEFT_LEFT_BORDER, LEFT_RIGHT_BORDER, ImGuiCond_Always);

    // ...
}
```

```cpp
// Tabular Plot
 if (ImGui::Begin("TableWindow", nullptr, WINDOW_FLAGS_CLEAN))
    {
        if (ImGui::BeginTable("Table", 5, ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuter))
        {
            // ...

            ImGui::EndTable();
        }

        ImGui::End();
    }
```

![Example](./media/AdasGuiDescr.png)
