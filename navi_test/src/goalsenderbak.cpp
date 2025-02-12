
#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "nav2_msgs/action/navigate_to_pose.hpp"

class NavigateClient : public rclcpp::Node {
public:
    using NavigateToPose = nav2_msgs::action::NavigateToPose;
    using GoalHandleNavigate = rclcpp_action::ClientGoalHandle<NavigateToPose>;

    NavigateClient() : Node("navigate_client") {
        client_ = rclcpp_action::create_client<NavigateToPose>(this, "navigate_to_pose");

        if (!client_->wait_for_action_server(std::chrono::seconds(5))) {
            RCLCPP_ERROR(this->get_logger(), "Action server not available!");
            return;
        }

        while (rclcpp::ok()) {
            
            std::string input;
            std::cout << "Enter x, y, theta: ";
            std::cin >> input;
            std::istringstream iss(input);
            double x, y, theta;
            iss >> x >> y >> theta;
            send_goal(x, y, theta);
            // send_goal(0.0, 0.0, 0.0);
            // rclcpp::sleep_for(std::chrono::seconds(5));
        // send_goal();
         }
        

        
    }

// private:
//     void goal_response_callback(std::shared_future<GoalHandleNavigate::SharedPtr> future) {
//         auto goal_handle = future.get();
//         if (!goal_handle) {
//             RCLCPP_ERROR(this->get_logger(), "Goal was rejected by server");
//         } else {
//             RCLCPP_INFO(this->get_logger(), "Goal accepted by server, waiting for result");
//         }
//     }



private:
    rclcpp_action::Client<NavigateToPose>::SharedPtr client_;

    void send_goal(double x, double y, double theta) { //

        // std::string input;
        // std::cout << "Enter x, y, theta: ";
        // std::cin >> input;
        // if (input == "q") {
        //     RCLCPP_INFO(rclcpp::get_logger("navigate_client"), "Exiting...");
        //     return;
        // }
        // std::istringstream iss(input);
        // double x, y, theta;
        // iss >> x >> y >> theta;


        auto goal_msg = NavigateToPose::Goal();
        goal_msg.pose.header.frame_id = "map";
        goal_msg.pose.header.stamp = this->now();
        goal_msg.pose.pose.position.x = x;
        goal_msg.pose.pose.position.y = y;
        goal_msg.pose.pose.orientation.w = cos(theta / 2);

        // auto send_goal_options = rclcpp_action::Client<NavigateToPose>::SendGoalOptions();
        // send_goal_options.goal_response_callback = std::bind(&NavigateClient::goal_response_callback, this, std::placeholders::_1);
        // GoalHandleNavigate::WrappedResult result = client_->async_send_goal(goal_msg);

        auto send_goal_options = rclcpp_action::Client<NavigateToPose>::SendGoalOptions();
        send_goal_options.result_callback = [](const GoalHandleNavigate::WrappedResult &result) {
            if (result.code == rclcpp_action::ResultCode::SUCCEEDED) {
                RCLCPP_INFO(rclcpp::get_logger("navigate_client"), "Navigation succeeded!");
                // this->send_goal();
            } else {
                RCLCPP_WARN(rclcpp::get_logger("navigate_client"), "Navigation failed!");
                // this->send_goal();
            }
        };
        
        // client_->async_send_goal(goal_msg, send_goal_options);
    
    }
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<NavigateClient>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
