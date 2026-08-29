
```
ros2 pkg create cartographer_slam --build-type ament_cmake --dependencies cartographer_ros

/* keyboard teleop */
ros2 run teleop_twist_keyboard teleop_twist_keyboard

/* Set initial pose */
ros2 topic pub -1 /initialpose geometry_msgs/msg/PoseWithCovarianceStamped "{header: {stamp: {sec: 0}, frame_id: 'map'}, pose: {pose: {position: {x: 0.2, y: 0.0, z: 0.0}, orientation: {w: 1.0}}}}"

```
