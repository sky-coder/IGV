#include <ros/ros.h>
#include <signal.h>
#include <geometry_msgs/Pose.h>
#include <tf/transform_broadcaster.h>
#include "os5000/OS5000.h"

void intHandle(int sig)
{
    ros::shutdown();
}

int main(int argc, char **argv)
{
    ros::init(argc, argv,"compass_odom");
    ros::NodeHandle nh;

    ros::Publisher compass_publish = nh.advertise<geometry_msgs::Pose>("compass",100);

    ros::Rate loop_rate(10);
    geometry_msgs::Pose PoseData;
    OS5000 Compass;
    Compass.setupSerialConnection();
    while (ros::ok())
    {
       Compass.getOrientation();

       PoseData.orientation = tf::createQuaternionMsgFromYaw(Compass.getYaw());
       PoseData.position.x = 0;
       PoseData.position.y = 0;
       PoseData.position.z = 0;

       compass_publish.publish(PoseData);
       ros::spinOnce();

       loop_rate.sleep();
    }
    return 0;
}
