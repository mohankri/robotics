In motion planning, we can differentiate between two main basic methods:

1.- Planning to a joint-space goal: In this method, we set the desired joint positions for the arm. The motion planning algorithm computes a trajectory for moving from the current joint positions to the new desired ones. This motion planning method is simpler as it doesn't involve inverse kinematics (IK).

2.- Planning to an end-effector pose: In this approach, we set the desired pose, which includes both position and orientation, for the end-effector link in space. This motion planning method is more complex as it does involve inverse kinematics (IK).

# Move Group Interface
http://docs.ros.org/en/noetic/api/moveit_ros_planning_interface/html/classmoveit_1_1planning__interface_1_1MoveGroupInterface.html

# Create Project for Motion Planning
cd ~/ros2_ws/src

ros2 pkg create moveit2_scripts --dependencies rclcpp rclcpp_action moveit_core moveit_ros_planning moveit_ros_planning_interface interactive_markers geometric_shapes control_msgs moveit_msgs

cd ~/ros2_ws/src/moveit2_scripts/src
touch joint_space_trajectory.cpp

cd ~/ros2_ws/src/moveit2_scripts
mkdir launch
touch launch/joint_space_trajectory.launch.py
