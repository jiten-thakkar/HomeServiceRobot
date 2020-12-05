# Home_Service_Robot 

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
