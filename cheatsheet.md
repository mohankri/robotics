
```
ros2 pkg create cartographer_slam --build-type ament_cmake --dependencies cartographer_ros

/* keyboard teleop */
ros2 run teleop_twist_keyboard teleop_twist_keyboard

/* Set initial pose */
ros2 topic pub -1 /initialpose geometry_msgs/msg/PoseWithCovarianceStamped "{header: {stamp: {sec: 0}, frame_id: 'map'}, pose: {pose: {position: {x: 0.2, y: 0.0, z: 0.0}, orientation: {w: 1.0}}}}"


ros2 action send_goal /navigate_to_pose nav2_msgs/action/NavigateToPose "pose: {header: {frame_id: map}, pose: {position: {x: 1.52, y: 1.92, z: 0.0}, orientation:{x: 0.0, y: 0.0, z: 0, w: 1.0000000}}}"
```
```
<robot name="grasp_box">

    <!-- Colours for RViz for geometric elements -->
    <material name="blue">
        <color rgba="0 0 0.8 1"/>
    </material>
    <material name="red">
        <color rgba="0.8 0 0 1"/>
    </material>
    <material name="green">
        <color rgba="0 0.8 0 1"/>
    </material>
    <material name="grey">
        <color rgba="0.75 0.75 0.75 1"/>
    </material>
    <material name="white">
        <color rgba="1.0 1.0 1.0 1"/>
    </material>
    <material name="black">
        <color rgba="0 0 0 1"/>
    </material>


	<!-- * * * Link Definitions * * * -->

    <link name="grasp_box_base_link">
 	    <inertial>
            <origin xyz="0 0 0" rpy="0 0 0"/>
            <mass value="0.5" />
            <inertia ixx="0.000133333333333" ixy="0.0" ixz="0.0" iyy="0.000133333333333" iyz="0.0" izz="0.000133333333333"/>
        </inertial>
        <collision>
            <origin xyz="0 0 0" rpy="0 0 0"/>
            <geometry>
                <box size="0.04 0.04 0.08"/>
            </geometry>
        </collision>
        <visual>
            <origin rpy="0.0 0 0" xyz="0 0 0"/>
            <geometry>
                <box size="0.04 0.04 0.08"/>
            </geometry>
            <material name="red"/>
        </visual>
    </link>

    <gazebo reference="grasp_box_base_link">
        <kp>100000.0</kp>
        <kd>100.0</kd>
        <mu1>0.25</mu1>
        <mu2>0.25</mu2>
        <material>Gazebo/Red</material>
    </gazebo>

</robot>

/* draw object on gazebo */
ros2 run gazebo_ros spawn_entity.py -file ~/ros2_ws/src/grasp_box.urdf -x 0.34 -y 0.13 -z 0.1 -entity grasp_box

```
