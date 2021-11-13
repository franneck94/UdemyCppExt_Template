# Exercise

The use-case:

The radars and cameras of your car detect other vehicles that are nearby your (ego) vehicle.

Example image other vehicles (V) ego vehicle (E):

![vehicle](../../media/vehicle.png)

Note: this plot will be implemented in the next exercise.

Implement the following functions:

```cpp
enum class LaneAssociationType
{
    // Unknown Lane
    // Left Lane
    // Center Lane
    // Right Lane
};

struct VehicleType
{
    // Id
    // Lane
    // speed (meter/s)
    // Distance (meter)
};

struct NeighborVehiclesType
{
    // array for vehicles on left lane (2 vehicles)
    // array for vehicles on center lane (2 vehicles)
    // array for vehicles on right lane (2 vehicles)
};
```

Implement the following functions:

```cpp
void init_ego_vehicle(VehicleType &ego_vehicle);

void init_vehicles(NeighborVehiclesType &vehicles);

void print_vehicle(const VehicleType &vehicle);

void print_neighbor_vehicles(const NeighborVehiclesType &vehicles);
```

- init_ego_vehicle: Init ego vehicle with ID=-1, Speed=135kmh, Lane=Center
- init_vehicles: Init vehicles with IDs starting at 0, you can choose the speed of the vehicles and the distance to the ego vehicle, 2 Vehicles on Left, 2 on Right, and 2 on Center Lane
- print_vehicle:
  - If the vehicle is an ego vehicle, just print the speed
  - otherwise, print ID, Speed, and Distance
- print_neighbor_vehicles: iterate over all vehicles and print them

## Main Function

```cpp
#include <cstring>
#include <iostream>
#include <numeric>

#include "AdFunctions.hpp"
#include "AdTypes.hpp"

int main()
{
    VehicleType ego_vehicle{};
    NeighborVehiclesType vehicles{};

    init_ego_vehicle(ego_vehicle);
    init_vehicles(vehicles);

    print_vehicle(ego_vehicle);
    print_neighbor_vehicles(vehicles);

    return 0;
}
```
