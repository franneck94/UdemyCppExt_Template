# Dear ImGUI Template to Start

## Example

![Example](./media/AdasGui.gif)

## Exercise

The new code will be added to the following files

- **Render.cc**
- **Render.hpp**
- **RenderConstants.hpp**

The GUI should display the following features:

- Scatter Plot with ImPlot
  - The X Range is $\in [-100, 100]$
  - The Y Range is $\in [-4.5, 4.5]$
  - Plot the lane borders
    - dashed: *gestrichelt*
    - solid: *durchgezogen*
  - Plot the position of the vehicles
    - The vehicles are always in the center of the lane

```cpp
void plot_lanes(const VehicleType &ego_vehicle, const NeighborVehiclesType &vehicles)
{
    if (ImPlot::BeginPlot("Lanes"))
    {
        

        ImPlot::EndPlot();
    }
}
```

![Example](./media/AdasGuiDescr.png)
