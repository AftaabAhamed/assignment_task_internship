#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <future>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "nav2_msgs/action/navigate_to_pose.hpp"
#include "geometry_msgs/msg/pose_with_covariance_stamped.hpp"

class NavigateClient : public rclcpp::Node {
public:
    using NavigateToPose = nav2_msgs::action::NavigateToPose;
    using GoalHandleNavigate = rclcpp_action::ClientGoalHandle<NavigateToPose>;

    NavigateClient() : Node("navigate_client") {
        client_ = rclcpp_action::create_client<NavigateToPose>(this, "navigate_to_pose");

        sleep(2);

        initial_pose_publisher_ = this->create_publisher<geometry_msgs::msg::PoseWithCovarianceStamped>("initialpose", 1);


        publish_initial_pose();

        if (!client_->wait_for_action_server(std::chrono::seconds(30))) {
            RCLCPP_ERROR(this->get_logger(), "Action server not available!");
            return;
        }

        // Publish the initial pose
        // publish_initial_pose();

        while (rclcpp::ok()) {
            double x, y, theta;
            std::cout << "Enter x: ";
            std::cin >> x;
            std::cout << "Enter y: ";
            std::cin >> y;
            std::cout << "Enter theta: ";
            std::cin >> theta;

            // Create a promise and future to wait for the result
            auto result_promise = std::make_shared<std::promise<void>>();
            auto result_future = result_promise->get_future();

            send_goal(x, y, theta, result_promise);

            // Wait for the result callback to be resolved
            // result_future.get();
        }
    }

private:
    rclcpp_action::Client<NavigateToPose>::SharedPtr client_;
    rclcpp::Publisher<geometry_msgs::msg::PoseWithCovarianceStamped>::SharedPtr initial_pose_publisher_;

    void publish_initial_pose() {
        auto initial_pose_msg = geometry_msgs::msg::PoseWithCovarianceStamped();
        initial_pose_msg.header.frame_id = "map";
        // initial_pose_msg.header.stamp = 0;
        initial_pose_msg.pose.pose.position.x = 0.2;
        initial_pose_msg.pose.pose.position.y = 0.0;
        initial_pose_msg.pose.pose.position.z = 0.15;
        initial_pose_msg.pose.pose.orientation.z = 0.0;
        initial_pose_msg.pose.pose.orientation.w = 1.0;

        RCLCPP_INFO(this->get_logger(), "Publishing initial pose...");
        initial_pose_publisher_->publish(initial_pose_msg);
    }

    void send_goal(double x, double y, double theta, std::shared_ptr<std::promise<void>> result_promise) {
        auto goal_msg = NavigateToPose::Goal();
        goal_msg.pose.header.frame_id = "map";
        goal_msg.pose.header.stamp = this->now();
        goal_msg.pose.pose.position.x = x;
        goal_msg.pose.pose.position.y = y;
        goal_msg.pose.pose.orientation.w = cos(theta / 2);
        goal_msg.pose.pose.orientation.z = sin(theta / 2);
        

        auto send_goal_options = rclcpp_action::Client<NavigateToPose>::SendGoalOptions();
        send_goal_options.feedback_callback = [this](GoalHandleNavigate::SharedPtr, const std::shared_ptr<const NavigateToPose::Feedback> feedback) {
            RCLCPP_INFO(this->get_logger(), "Received feedback: %f", feedback->distance_remaining);
        };
        send_goal_options.result_callback = [this, result_promise](const GoalHandleNavigate::WrappedResult &result) {
            RCLCPP_INFO(this->get_logger(), "Result callback triggered");
            if (result.code == rclcpp_action::ResultCode::SUCCEEDED) {
                RCLCPP_INFO(this->get_logger(), "Navigation succeeded!");
            } else {
                RCLCPP_WARN(this->get_logger(), "Navigation failed with result code");
            }
            // Set the promise value to unblock the waiting future
            result_promise->set_value();
        };

        std::shared_future<GoalHandleNavigate::SharedPtr> future =  client_->async_send_goal(goal_msg, send_goal_options);
        RCLCPP_INFO(this->get_logger(), "Sending goal...");
        future.wait();
        RCLCPP_INFO(this->get_logger(), "resolved goal...");


    }
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<NavigateClient>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}