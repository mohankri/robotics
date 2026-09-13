In motion planning, we can differentiate between two main basic methods:

1.- Planning to a joint-space goal: In this method, we set the desired joint positions for the arm. The motion planning algorithm computes a trajectory for moving from the current joint positions to the new desired ones. This motion planning method is simpler as it doesn't involve inverse kinematics (IK).

2.- Planning to an end-effector pose: In this approach, we set the desired pose, which includes both position and orientation, for the end-effector link in space. This motion planning method is more complex as it does involve inverse kinematics (IK).
