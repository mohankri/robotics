#include "geometry_msgs/msg/twist.hpp"
#include "lifecycle_msgs/msg/state.hpp"
#include "rcl_interfaces/msg/parameter_descriptor.hpp"
#include "rcl_interfaces/msg/set_parameters_result.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "rclcpp_lifecycle/lifecycle_publisher.hpp"

#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <vector>

using CallbackReturn =
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;
using namespace std::chrono_literals;

class MoveRobotLifecycle : public rclcpp_lifecycle::LifecycleNode {
public:
  MoveRobotLifecycle()
      : rclcpp_lifecycle::LifecycleNode("param_vel_lifecycle_node") {
    // Parameter descriptors
    rcl_interfaces::msg::ParameterDescriptor linear_desc;
    linear_desc.description = "Linear speed of the robot in m/s.";

    rcl_interfaces::msg::ParameterDescriptor angular_desc;
    angular_desc.description = "Angular speed of the robot in rad/s.";

    rcl_interfaces::msg::ParameterDescriptor rate_desc;
    rate_desc.description =
        "Rate in Hz at which velocity commands are published.";

    // Declare parameters
    this->declare_parameter<double>("linear_speed", 0.0, linear_desc);
    this->declare_parameter<double>("angular_speed", 0.0, angular_desc);
    this->declare_parameter<double>("publish_rate", 1.0, rate_desc);

    // Register parameter callback
    callback_handle_ = this->add_on_set_parameters_callback(std::bind(
        &MoveRobotLifecycle::parameters_callback, this, std::placeholders::_1));

    RCLCPP_INFO(this->get_logger(),
                "Lifecycle node created. Currently unconfigured.");
  }

protected:
  CallbackReturn on_configure(const rclcpp_lifecycle::State &) {
    RCLCPP_INFO(this->get_logger(), "Configuring node...");

    // Read parameters
    linear_speed_ = this->get_parameter("linear_speed").as_double();
    angular_speed_ = this->get_parameter("angular_speed").as_double();
    publish_rate_ = this->get_parameter("publish_rate").as_double();

    if (publish_rate_ <= 0.0) {
      RCLCPP_ERROR(this->get_logger(), "publish_rate must be greater than 0.0");
      return CallbackReturn::FAILURE;
    }

    // Create lifecycle publisher
    publisher_ =
        this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);

    // Create timer
    auto period = std::chrono::duration<double>(1.0 / publish_rate_);
    timer_ = this->create_wall_timer(
        std::chrono::duration_cast<std::chrono::nanoseconds>(period),
        std::bind(&MoveRobotLifecycle::timer_callback, this));

    RCLCPP_INFO(this->get_logger(),
                "Node configured successfully. linear_speed=%.2f "
                "angular_speed=%.2f publish_rate=%.2f",
                linear_speed_, angular_speed_, publish_rate_);

    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_activate(const rclcpp_lifecycle::State &) {
    RCLCPP_INFO(this->get_logger(), "Activating node...");
    publisher_->on_activate();
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_deactivate(const rclcpp_lifecycle::State &) {
    RCLCPP_INFO(this->get_logger(), "Deactivating node...");
    publisher_->on_deactivate();
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_cleanup(const rclcpp_lifecycle::State &) {
    RCLCPP_INFO(this->get_logger(), "Cleaning up node...");
    timer_.reset();
    publisher_.reset();
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_shutdown(const rclcpp_lifecycle::State &) {
    RCLCPP_INFO(this->get_logger(), "Shutting down node...");
    timer_.reset();
    publisher_.reset();
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_error(const rclcpp_lifecycle::State &) {
    RCLCPP_ERROR(this->get_logger(),
                 "An error occurred. Cleaning up resources...");

    timer_.reset();
    publisher_.reset();

    return CallbackReturn::SUCCESS;
  }

private:
  rcl_interfaces::msg::SetParametersResult
  parameters_callback(const std::vector<rclcpp::Parameter> &parameters) {
    rcl_interfaces::msg::SetParametersResult result;

    // Reject parameter changes while ACTIVE
    if (this->get_current_state().id() ==
        lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE) {
      result.successful = false;
      result.reason = "Parameters cannot be changed while the node is ACTIVE.";
      RCLCPP_WARN(this->get_logger(),
                  "Rejected parameter change: node is ACTIVE.");
      return result;
    }

    // Basic validation
    for (const auto &parameter : parameters) {
      if (parameter.get_name() == "publish_rate" &&
          parameter.as_double() <= 0.0) {
        result.successful = false;
        result.reason = "Parameter 'publish_rate' must be greater than 0.0.";
        return result;
      }
    }

    result.successful = true;
    result.reason = "Parameters accepted.";
    RCLCPP_INFO(this->get_logger(), "Parameter change accepted.");
    return result;
  }

  void timer_callback() {
    if (!publisher_ || !publisher_->is_activated()) {
      return;
    }

    // Read parameters at runtime
    linear_speed_ = this->get_parameter("linear_speed").as_double();
    angular_speed_ = this->get_parameter("angular_speed").as_double();

    geometry_msgs::msg::Twist twist;
    twist.linear.x = linear_speed_;
    twist.angular.z = angular_speed_;

    RCLCPP_INFO(this->get_logger(),
                "Publishing command | linear=%.2f angular=%.2f", linear_speed_,
                angular_speed_);

    publisher_->publish(twist);
  }

  double linear_speed_;
  double angular_speed_;
  double publish_rate_;

  rclcpp_lifecycle::LifecyclePublisher<geometry_msgs::msg::Twist>::SharedPtr
      publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  OnSetParametersCallbackHandle::SharedPtr callback_handle_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<MoveRobotLifecycle>();
  rclcpp::spin(node->get_node_base_interface());
  rclcpp::shutdown();
  return 0;
}