from launch import LaunchDescription
from launch_ros.actions import LifecycleNode


def generate_launch_description():
    return LaunchDescription([
        LifecycleNode(package='managed_node_tests', executable='managed_node_example',
                      name='param_vel_lifecycle_node', namespace='', output='screen')
    ])