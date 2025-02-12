
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

