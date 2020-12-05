#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <nav_msgs/Odometry.h>
#include <math.h>

double pickupPosX = -3.0;
double pickupPosY =  0.0;
double dropoffPosX = -3.0;
double dropoffPosY = -4.0;
bool pickedup = false;
float x_pos = 0.0, y_pos = 0.0;

void odomCallback(const nav_msgs::Odometry::ConstPtr& msg)
{
  x_pos = msg->pose.pose.position.x;
  y_pos = msg->pose.pose.position.y;
}


int main( int argc, char** argv )
{
  ros::init(argc, argv, "add_markers");
  ros::NodeHandle n;
  ros::Rate r(1);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  ros::Subscriber odom_sub = n.subscribe("odom", 1000, odomCallback);

  visualization_msgs::Marker marker;

  // Set our initial shape type to be a cube
  uint32_t shape = visualization_msgs::Marker::CUBE;
  
  // Set the frame ID and timestamp.  See the TF tutorials for information on these.
  marker.header.frame_id = "map";
  marker.header.stamp = ros::Time::now();
  marker.ns = "cube";
  marker.id = 0;


  // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
  marker.type = shape;


  // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
  marker.pose.position.x = pickupPosX;
  marker.pose.position.y = pickupPosY;
  marker.pose.position.z = 0;
  marker.pose.orientation.x = 0.0;
  marker.pose.orientation.y = 0.0;
  marker.pose.orientation.z = 0.0;
  marker.pose.orientation.w = 1.57;

  // Set the scale of the marker -- 1x1x1 here means 1m on a side
  marker.scale.x = 0.5;
  marker.scale.y = 0.5;
  marker.scale.z = 0.5;

  // Set the color -- be sure to set alpha to something non-zero!
  marker.color.r = 1.0f;
  marker.color.g = 0.0f;
  marker.color.b = 0.0f;
  marker.color.a = 1.0;

  marker.lifetime = ros::Duration();
  while (ros::ok())
  {

    while (marker_pub.getNumSubscribers() < 1)
    {
      if (!ros::ok())
      {
        return 0;
      }
      ROS_WARN_ONCE("Please create a subscriber to the marker");
      sleep(1);
    }
    if ((x_pos != pickupPosX) && (y_pos != pickupPosY) && (pickedup == false)){
      marker.action = visualization_msgs::Marker::ADD;
      marker_pub.publish(marker);
    }
    ROS_INFO("Odom data: %f, %f", x_pos, y_pos);

    if ((abs(pickupPosX -y_pos) <  0.5 ) && (abs(pickupPosY - x_pos) < 0.5)){
      ROS_INFO("Reached first goal");
      //ros::Duration(5.0).sleep();
      marker.action = visualization_msgs::Marker::DELETE;
      marker_pub.publish(marker);
      pickedup = true;
    }

    if ((pickedup == true) && (abs(dropoffPosX - y_pos) <  0.5 ) && (abs((-1*dropoffPosY) - x_pos) < 0.5)){
      ROS_INFO("Reached second goal");
      //ros::Duration(5.0).sleep();
      marker.pose.position.x = dropoffPosX;
      marker.pose.position.y = dropoffPosY;
      marker.action = visualization_msgs::Marker::ADD;
      marker_pub.publish(marker);
    }
    ros::spinOnce();
  }
}
