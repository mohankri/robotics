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
```
A map is a representation of the environment in which the robot operates. The robot relies on a map to:

Localize itself within the environment.
Plan trajectories to navigate safely from one point to another.

In ROS, a map is typically an occupancy grid map, where each cell contains values that indicate whether an area is free, occupied by an obstacle, or unknown.

To create a map, you need:

-  A robot equipped with:
    -    LIDAR for detecting obstacles and measuring distances.
    -    Odometry to track the robot's movement.
-  An environment for the robot to explore and map.

```
