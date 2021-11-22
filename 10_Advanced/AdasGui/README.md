# Dear ImGUI Template to Start

## Example

![Example](./media/AdasGui.gif)

## Exercise

The GUI should display the following features:

- For Left, Center, and Right Lane: Scatter Plot with ImPlot
  - Plot the position of the vehicles
  - Every vehicle should have a different color

```cpp
// Scatter Plot
if (ImPlot::BeginPlot("##Lane1", PLOT_DIM, PLOT_FLAGS))
    {
        ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_Lock, ImPlotAxisFlags_Lock | ImPlotAxisFlags_Invert);
        ImPlot::SetupAxisLimits(ImAxis_X1, -MAX_VIEW_RANGE_M, MAX_VIEW_RANGE_M, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, LEFT_LEFT_BORDER, LEFT_RIGHT_BORDER, ImGuiCond_Always);
        ImPlot::SetNextMarkerStyle(...); // See documentation
        ImPlot::PlotScatter(...); // See documentation
        ...
    }
```

- Below the lanes plot, you should plot a table with all the vehicles data
- Below the table, you should plot the current cycle number and start/stop/replay buttons

![Example](./media/AdasGuiDescr.png)
