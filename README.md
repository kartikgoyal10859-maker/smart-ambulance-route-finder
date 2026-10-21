# Smart Ambulance Route Finder 🚑

## Project Overview

The Smart Ambulance Route Finder is a C-based project that uses Dijkstra's Shortest Path Algorithm to find the shortest route between two locations in a simulated road network.

The project demonstrates how graph algorithms can support route planning for emergency services.

## Objectives

- Find the shortest path between a starting location and a destination.
- Calculate the total route distance.
- Estimate travel time using simulated distances.
- Visualize the road network and route results.

## Technologies Used

- **Programming Language:** C
- **Algorithm:** Dijkstra's Shortest Path Algorithm
- **Compiler:** GCC
- **Visualization:** SVG graphs

## Features

- 20 simulated locations.
- Road network represented as a graph.
- Shortest-route calculation.
- Estimated travel time.
- Four generated visualizations:
  - `road_network.svg`
  - `best_route.svg`
  - `distance_line.svg`
  - `route_comparison.svg`

## How to Run

Compile the program:

```bash
gcc ambulance.c -o ambulance.exe
```

Run on Windows PowerShell:

```powershell
.\ambulance.exe
```

Enter the starting and destination location numbers when prompted.

## Important Note

This is an educational prototype. The road network, distances, and estimated travel times are simulated and do not represent live traffic conditions or verified real-world routes.

## Author

Student project — Smart Ambulance Route Finder.

