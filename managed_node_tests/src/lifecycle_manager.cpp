#include <chrono>
#include <future>
#include <memory>
#include <string>
#include <thread>

#include "lifecycle_msgs/msg/transition.hpp"
#include "lifecycle_msgs/srv/change_state.hpp"
#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;

static constexpr char const * change_state_topic =
  "/param_vel_lifecycle_node/change_state";

template<typename FutureT, typename WaitTimeT>
std::future_status wait_for_result(FutureT & future, WaitTimeT time_to_wait)
{
  auto end = std::chrono::steady_clock::now() + time_to_wait;
  std::chrono::milliseconds wait_period(100);
  std::future_status status = std::future_status::timeout;

  do {
    auto now = std::chrono::steady_clock::now();
    auto time_left = end - now;
    if (time_left <= std::chrono::seconds(0)) {
      break;
    }
    status = future.wait_for((time_left < wait_period) ? time_left : wait_period);
  } while (rclcpp::ok() && status != std::future_status::ready);

  return status;
}

class MyLifecycleServiceClient : public rclcpp::Node
{
public:
  explicit MyLifecycleServiceClient(const std::string & node_name)
  : Node(node_name)
  {
  }

  void init()
  {
    client_change_state_ =
      this->create_client<lifecycle_msgs::srv::ChangeState>(change_state_topic);
  }

  bool change_state(
    std::uint8_t transition,
    std::chrono::seconds timeout = 3s)
  {
    auto request =
      std::make_shared<lifecycle_msgs::srv::ChangeState::Request>();
    request->transition.id = transition;

    if (!client_change_state_->wait_for_service(timeout)) {
      RCLCPP_ERROR(
        this->get_logger(),
        "Service %s is not available.",
        client_change_state_->get_service_name());
      return false;
    }

    auto future_result = client_change_state_->async_send_request(request);
    auto future_status = wait_for_result(future_result, timeout);

    if (future_status != std::future_status::ready) {
      RCLCPP_ERROR(
        this->get_logger(),
        "Timed out while requesting transition %d.",
        static_cast<int>(transition));
      return false;
    }

    if (future_result.get()->success) {
      RCLCPP_INFO(
        this->get_logger(),
        "Transition %d successfully triggered.",
        static_cast<int>(transition));
      return true;
    } else {
      RCLCPP_WARN(
        this->get_logger(),
        "Failed to trigger transition %d.",
        static_cast<int>(transition));
      return false;
    }
  }

private:
  rclcpp::Client<lifecycle_msgs::srv::ChangeState>::SharedPtr client_change_state_;
};

void lifecycle_manager_callback(std::shared_ptr<MyLifecycleServiceClient> lc_client)
{
  rclcpp::WallRate time_between_state_changes(10.0);

  if (!lc_client->change_state(
        lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE))
  {
    return;
  }

  time_between_state_changes.sleep();

  if (!rclcpp::ok()) {
    return;
  }

  if (!lc_client->change_state(
        lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE))
  {
    return;
  }
}

int main(int argc, char ** argv)
{
  setvbuf(stdout, NULL, _IONBF, BUFSIZ);

  rclcpp::init(argc, argv);

  auto manager_client_node =
    std::make_shared<MyLifecycleServiceClient>("manager_client");
  manager_client_node->init();

  rclcpp::executors::SingleThreadedExecutor executor;
  executor.add_node(manager_client_node);

  std::shared_future<void> script = std::async(
    std::launch::async,
    std::bind(lifecycle_manager_callback, manager_client_node));

  executor.spin_until_future_complete(script);

  rclcpp::shutdown();
  return 0;
}