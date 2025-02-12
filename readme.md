
# Autonomous Navigation with Nav2 in Gazebo





## Requirements
 - ROS 2 Humble
 - Gazebo Fortress
 - [Navigation 2](https://navigation.ros.org/build_instructions/index.html#install)
   
## Requirements for setting up gazebo robot simulation package
link to original package : [Github Repo](https://github.com/art-e-fact/navigation2_ignition_gazebo_example)

setup and build
```

# Install Nav2 dependencies
sudo apt install ros-humble-navigation2 ros-humble-nav2-bringup

# Import source dependencies
pip3 install vcstool
vcs import --input deps.repos src

# Install rosrep dependencies
rosdep install -y -r -i  --from-paths . 

# Make sure ROS2 is sourced (assuming bash, please replace extension as needed)
source /opt/ros/humble/setup.bash

# Build
colcon build

# Make sure the app is sourced (assuming bash, please replace extension as needed)
source install/setup.bash
```

## setup after cloning repo:
create and build ws by running :
```
colcon build
```
source ws using:
```
source install/setup.bash
```
launch naigation + mapping using slam_toolbox :
```
ros2 launch navi_test ar_navi.launch.py
```

launch navigation using amcl and mapserver with premade map:
```
ros2 launch navi_test ar_navi_no_mapping.launch.py
```

Running goal publishing / initial pose setting node:
```
ros2 run navi_test waypoint_test
```
DEMO VIDEO
[![DEMO](https://img.youtube.com/vi/XaN-HiLBtOc/0.jpg)](https://youtu.be/XaN-HiLBtOc)

## Explaination

1. Setting Up Gazebo Fortress and Launch Files:

Challenge: I had to set up Gazebo Fortress on my system, as I was more familiar with Gazebo Classic. Additionally, I had to adjust various launch files to integrate mapping and navigation.
Solution: I found a package online with a differential drive robot simulation, including lidar and a differential drive controller. I used this as a base, editing the Gazebo world to include walls and barriers, and modified the launch files for mapping and navigation. I also created a launch file to launch navigation and slam_toolbox together to test the system.

2. Event Handlers and Launch File Order:

Challenge: Setting up launch files with event handlers to ensure the correct order of execution was difficult.
Solution: I worked through the issues in the launch files, though it was challenging to get the proper order for launching mapping and navigation correctly.

3. AMCL Localization Issue:

Challenge: When trying to use AMCL for localization, the parameter file wasn't reaching the AMCL server, preventing localization from working.
Solution: I identified the problem as a bug in ROS2 Humble’s DDS implementation. I followed a recommendation on GitHub to switch to Cyclone DDS and changed the parameter from robot_model_type: differentialModel to robot_model_type: nav2_amcl::DifferentialMotionModel, which resolved the issue.

4. C++ Node for Initial Pose:

Challenge: I had to create a C++ node to publish the robot's initial pose to /initialpose when it spawns.
Solution: I successfully created the C++ node to publish the initial pose, which worked as expected.

5. Action Client Callback Issue:

Challenge: The callback function for the NavigateToPose action server wasn't being triggered after sending a goal with the async send_goal method.
Solution: Although I was unable to resolve the issue within the available time, I suspect it could have been due to miscommunication between the action client and server or asynchronous handling issues that required deeper debugging.

6. Tuning Navigation Parameters:

Challenge: The robot had trouble passing through narrow gaps due to the inflated cost map radius, which caused obstacles to be treated as more obstructive than they were.
Solution: I reduced the inflation radius of the cost map, allowing the robot to navigate more easily through narrow passages.

7. Controller Tuning:

Challenge: The default controller required significant tuning to achieve good performance.
Solution: I replaced the dwb_core::DWBLocalPlanner controller with nav2_mppi_controller::MPPIController, which performed better and required less tuning to get satisfactory results given the limited time. 

To improve the obstacle avoidance and improve localisation i reduced the robot velocity
amcl localisation did not always provide adequate results and required further tuning
