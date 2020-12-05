#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include "std_msgs/String.h"
#include <nav_msgs/Odometry.h>
#include <string.h>
#include <cmath>

class odomData{
    public:
        double x;
        double y;
        double w;
        void odomCallback(const nav_msgs::Odometry data);
};

void odomData::odomCallback(nav_msgs::Odometry data)
{
    x = data.pose.pose.position.x;
    y = data.pose.pose.position.y;
    w = data.pose.pose.orientation.w;

    // ROS_INFO("x: %1.2f, y: %1.2f, z: $1.2f",(float)x, (float)y ,(float)w);
}

int main( int argc, char** argv )
{
  odomData objodomData;
  ros::init(argc, argv, "add_markers");
  ros::NodeHandle n;
  ros::Rate loop_rate(0.5);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  ros::Subscriber marker_sub = n.subscribe("odom", 1000, &odomData::odomCallback, &objodomData); //nav_msgs/Odometry.msg
  uint32_t shape = visualization_msgs::Marker::CUBE;
  bool first_goal = true;
  double tolerance = 1.7;

    while (ros::ok())
    {
        visualization_msgs::Marker marker;

        // Set the frame ID and timestamp.  See the TF tutorials for information on these.
        marker.header.frame_id = "/map";
        marker.header.stamp = ros::Time::now();

        // Set the namespace and id for this marker.  This serves to create a unique ID
        // Any marker sent with the same namespace and id will overwrite the old one
        marker.ns = "add_markers";
        marker.id = 0;

        // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
        marker.type = shape;

        // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
        marker.action = visualization_msgs::Marker::ADD;

        // Set the scale of the marker -- 1x1x1 here means 1m on a side
        marker.scale.x = 0.2;
        marker.scale.y = 0.2;
        marker.scale.z = 0.2;

        // Set the color -- be sure to set alpha to something non-zero!
        marker.color.r = 0.0f;
        marker.color.g = 0.0f;
        marker.color.b = 1.0f;
        marker.color.a = 1.0;

        // marker.lifetime = ros::Duration();

        marker.lifetime = ros::Duration();

        while (marker_pub.getNumSubscribers() < 1)
        {
            if (!ros::ok())
            {
            return 0;
            }
            ROS_WARN_ONCE("Please create a subscriber to the marker");
            sleep(1);
        }
   
        if (first_goal){
            marker.pose.position.x = -3.0;
            marker.pose.position.y = 0;
            marker.pose.orientation.w = 0.0;

            marker_pub.publish(marker);

            ROS_INFO("pickup error: %1.2f",sqrt(pow((objodomData.x-marker.pose.position.x),2.0) + pow((objodomData.y-marker.pose.position.y),2.0)));
            if (sqrt(pow((objodomData.x-marker.pose.position.x),2.0) + pow((objodomData.y-marker.pose.position.y),2.0)) < tolerance)
            {
                marker.action = visualization_msgs::Marker::DELETE;
                marker_pub.publish(marker);
                first_goal = false;
            }
        }
        else{
            marker.pose.position.x = -3.0;
            marker.pose.position.y = -4.0;
            marker.pose.orientation.w = 0.0;

            ROS_INFO("dropoff error: %1.2f",sqrt(pow((objodomData.x-marker.pose.position.x),2.0) + pow((objodomData.y-marker.pose.position.y),2.0)));
            if (pow((objodomData.x-marker.pose.position.x),2.0) + pow((objodomData.y-marker.pose.position.y),2.0) < tolerance)
            {
                marker.action = visualization_msgs::Marker::ADD;
                marker_pub.publish(marker);
                return 0;
            }
        }

        ros::spinOnce();
        loop_rate.sleep();

    }
    return 0;
}
