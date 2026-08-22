# Managed Node Commands
```
# check the state of the node
ros2 lifecycle get /param_vel_lifecycle_node

# Change node state to configured

ros2 service call /param_vel_lifecycle_node/change_state lifecycle_msgs/ChangeState "{transition: {id: 1}}"

or

ros2 lifecycle set /param_vel_lifecycle_node configure

# change state of activate

ros2 lifecycle set /param_vel_lifecycle_node activate

# chage state to shutdown

ros2 lifecycle set /param_vel_lifecycle_node shutdown

# deactivate

ros2 lifecycle set /param_vel_lifecycle_node deactivate

```

# Repeat
```
ros2 launch managed_node_tests start_managed_node.launch.py

ros2 lifecycle set /param_vel_lifecycle_node configure

ros2 lifecycle set /param_vel_lifecycle_node activate

ros2 param set /param_vel_lifecycle_node angular_speed 0.5 (will fail once managed state is in activate state)

ros2 lifecycle set /param_vel_lifecycle_node deactivate
ros2 param set /param_vel_lifecycle_node angular_speed 0.5

ros2 lifecycle set /param_vel_lifecycle_node cleanup (reset the node)

/* activate again */
ros2 lifecycle set /param_vel_lifecycle_node configure
ros2 lifecycle set /param_vel_lifecycle_node activate

