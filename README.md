# Home_Service_Robot 

This repository has submodules to use following command to clone it with submodules.

```
git clone --recurse-submodules https://github.com/jiten-thakkar/HomeServiceRobot.git
```
Update amcl launch file as follows to make sure that everything works perfectly.

In catkin_ws/src/turtlebot_simulator/turtlebot_gazebo/launch/amcl_demo.launch file, set the initial orientation of the robot to -1.57 as follows:

```
<arg name="initial_pose_a" default="-1.57"/>
```

To build
```
cd catkin_ws
catkin_make
```
To run

```
source devel/setup.bash
cd src/scripts
./home_service.sh
```


This project used gmapping package to map the environment and amcl package to localize the robot. In the home robot simulation, the robot goes to the marker to pick it up. As soon as the robot reaches near the marker, marker is deleted to mimick pick up of the package. The marker is subscribed to odom of the robot so that it knows when the robot is near the marker and it delets the marker. Once the robot reached the pick up location, it waits there for 5 seconds and then goes to the drop off location where marker reappears to mimick the drop off of the package. 
