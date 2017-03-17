// ros includes
#include <ros/ros.h>
#include <signal.h>
#include <geometry_msgs/Twist.h>
#include "os5000/OS5000.h"

// cpp includes
#include <string>

void intHandle(int sig)
{
    ros::shutdown();
}

int main(int argc, char **argv)
{
    ros::init(argc, argv,"compass_odom");
    ros::Rate loop_rate(10);

    ros::NodeHandle nh;
    ros::Publisher Imu_Pub = nh.advertise<geometry_msgs::Twist>("odom",100);

    OS5000 Imu;
    nh.param("port", Imu.port, string("/dev/ttyUSB0"));
    // nh.param("port", Imu.port, std::string("/dev/ttyUSB0"));
    // nh.param<std::string>("port", Imu.port, "/dev/ttyUSB0");
    nh.param("baudrate", Imu.baudrate, 9600);
    Imu.setupSerialConnection();

    geometry_msgs::Twist TwistData;

    while (ros::ok())
    {
       Imu.getOrientation();
       TwistData.angular.z = Imu.getYaw();
       TwistData.angular.y = Imu.getPitch();
       TwistData.angular.x = Imu.getRoll();


       TwistData.linear.x = 0;
       TwistData.linear.y = 0;
       TwistData.linear.z = 0;

       Imu_Pub.publish(TwistData);

       ros::spinOnce();
       loop_rate.sleep();
    }
    return 0;
}
