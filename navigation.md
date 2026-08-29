# Navigation

To move from one point to another, a robot must:

- Map the environment (Mapping)
    - The robot needs to create or access a map of its surroundings.
- Determine its location (Localization)
    - It must identify where it is within the mapped environment.
- Plan a route (Path Planning)
    - The robot calculates the best way to move between two points.
- Execute movement while avoiding obstacles (Robot Control & Obstacle Avoidance)
    - It sends commands to its wheels (or other actuators) to follow the planned path while detecting and avoiding obstacles in real time.


# Nav2 includes tools for:

-    Mapping – Load, serve, and store environment maps (Map Server).
-    Localization – Determine the robot’s position on the map (AMCL).
-    Path Planning – Compute a path from A to B while avoiding obstacles (Nav2 Planner).
-    Path Execution – Control the robot as it follows the planned path (Nav2 Controller).
-    Obstacle Awareness – Process sensor data to create an obstacle-aware representation of the world (Nav2 Costmap 2D).
-    Recovery Behaviors – Implement fallback strategies in case of navigation failures (Nav2 Recoveries).
-    Lifecycle Management – Handle the lifecycle of navigation servers (Nav2 Lifecycle Manager).
-    Customization – Extend functionality with plugins for custom algorithms and behaviors (Nav2 BT Server).

  <img width="3976" height="2453" alt="image" src="https://github.com/user-attachments/assets/03cd4375-f4ee-4c9a-a518-a353eb082f49" />


# Map

A **map** is a representation of the environment in which the robot operates.
The robot relies on a map to:

- **Localize** itself within the environment.
- **Plan trajectories** to navigate safely from one point to another.

In ROS, a map is typically an **occupancy grid map**, where each cell contains
a value indicating whether an area is *free*, *occupied* by an obstacle, or
*unknown*.

## Requirements

To create a map, you need:

- A robot equipped with:
  - **LIDAR** — for detecting obstacles and measuring distances.
  - **Odometry** — to track the robot's movement.
- An environment for the robot to explore and map.

## SLAM

SLAM (Simultaneous Localization and Mapping) is a technique that allows a robot to create a map of an unknown environment 
while simultaneously determining its own location within it. SLAM algorithms enable robots to explore and navigate without 
prior knowledge of their surroundings.

SLAM solutions in ROS 2 include:

-    Cartographer
    -    Real-time Simultaneous Localization and Mapping (SLAM) system that works in both 2D and 3D across various
         platforms and sensor setups
-    SLAM-Toolbox

## Saving Map

cd ~/ros2_ws/src/cartographer_slam/config (directory where .lua file is present)
ros2 run nav2_map_server map_saver_cli -f turtlebot_area

# Localization

ROS2 using AMCL (Adaptive Monte Carlo Localization).

Robot is localized if somebody publishes the transform between /odom and /map frame because robot /base_link is connected to /odom frame. If all are connected & correct AMCL will publish the transform.

