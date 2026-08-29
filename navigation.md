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

-    Cartographer (use cartography_ros package)
    -    Real-time Simultaneous Localization and Mapping (SLAM) system that works in both 2D and 3D across various
         platforms and sensor setups
-    SLAM-Toolbox

Launch cartographer and occupancy grid node from cartography_ros package.
Launch rviz2 to see and create the map
    (Include map in Display also ensure /map topic is available)
Save the map for later use. It will create pgm image file.

ros2 run nav2_map_server map_saver_cli -f turtlebot_area

Once mapping is done, there is no need to use cartographer anymore.

Now to view the map created you can launch map_server node from nav2_map_server package and lifecycle_manager from nav2_lifecycle_manager package (point to yaml file you saved as map)


## Saving Map

cd ~/ros2_ws/src/cartographer_slam/config (directory where .lua file is present)
ros2 run nav2_map_server map_saver_cli -f turtlebot_area

# Localization

ROS2 using AMCL (Adaptive Monte Carlo Localization).

Robot is localized if somebody publishes the transform between /odom and /map frame because robot /base_link is connected to /odom frame. If all are connected & correct AMCL will publish the transform.

Pose Estimate is set, the whole AMCL system will start localizing the robot. This means that it will start publishing the transform between map and odom. The whole system becomes live!

robot is localized when you know its (x, y,  θ) in the corresponding map (for 2D localization):

x is the x position of the robot in the map frame
y is the y position of the robot in the map frame
θ is the orientation of the robot in the map frame

<img width="244" height="207" alt="image" src="https://github.com/user-attachments/assets/a013537c-ed58-4cbd-88e9-cf2087edd707" />

The Adaptive Monte Carlo approach uses particles to localize the robot. These particles have their own coordinates and orientation values, like the actual robot, and a given weight. The weight value ( 
w
t
 ) is defined as the absolute difference between the actual pose of the robot and the predicted pose by that specific particle. The bigger or larger the particle's weight, the more accurately it defines the robot's pose.

# Nav2 LifeCycle Manager

<img width="973" height="440" alt="image" src="https://github.com/user-attachments/assets/3a5b235c-517f-4ecc-821c-b87d37622e36" />

```
Node(
    package='nav2_lifecycle_manager',
    executable='lifecycle_manager',
    name='lifecycle_manager',
    output='screen',
    parameters=[{'autostart': True},
                {'node_names': ['map_server',
                                'amcl',
                                'controller_server',
                                'planner_server',
                                'recoveries_server',
                                'bt_navigator']}])
```
  

