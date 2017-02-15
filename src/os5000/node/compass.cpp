#include <ros/ros.h>
#include <signal.h>
#include <geometry_msgs/Twist.h>

#include "os5000/OS5000.h"

void intHandle(int sig)
{
    ros::shutdown();
}

int main(int argc, char **argv)
{
    ros::init(argc, argv,"compass_odom");
    ros::NodeHandle nh;

    ros::Publisher compass_publish = nh.advertise<geometry_msgs::Twist>("odom",100);

    ros::Rate loop_rate(10);
    geometry_msgs::Twist TwistData;
    OS5000 Compass;

    while (ros::ok())
    {
       Compass.getOrientation();
       TwistData.angular.z = Compass.getYaw();
       TwistData.angular.y = Compass.getPitch();
       TwistData.angular.x = Compass.getRoll();


       TwistData.linear.x = 0;
       TwistData.linear.y = 0;
       TwistData.linear.z = 0;

       compass_publish.publish(TwistData);
       ros::spinOnce();

       loop_rate.sleep();
    }
    return 0;
}
