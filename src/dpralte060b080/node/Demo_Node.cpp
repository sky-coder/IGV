// ros includes
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

// cpp includes
#include <iostream>
#include <exception>
#include "dpralte060b080/DPRALTE060B080.h"

# define M_PI 3.14159265358979323846

void DemoCallback(const geometry_msgs::Twist::ConstPtr &msg)
{
    std::cout << "Linear velocity is: " << msg->linear.x << std::endl;
    std::cout << "Angular velocity is: " << msg->angular.z << std::endl;
}

double  static angular_factor = 0.0;
int32_t static velocity_left  = 0;
int32_t static velocity_right = 0;
int32_t static odometry_left  = 0;
int32_t static odometry_right = 0;

int main(int argc, char **argv)
{
    ros::init(argc, argv, "igv_DPRALTE060B080_Demo_Node");
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 1000);
    ros::Subscriber sub = nh.subscribe("cmd_vel", 1000, DemoCallback);
    ros::Rate loop_rate(10);

    geometry_msgs::Twist Twist_msg;
    std::cout << "Please key in linear velocity (m/s): ";
    std::cin >> Twist_msg.linear.x;
    std::cout << std::endl;
    std::cout << "Please key in angular velocity factor (sec/round): ";
    std::cin >> angular_factor;
    Twist_msg.angular.z = 2*M_PI / angular_factor;
    std::cout << std::endl;

    try
    {
        velocity_left = 
            (int32_t) (400*Twist_msg.linear.x - 1440*Twist_msg.angular.z) / 67;
        velocity_right = 
            (int32_t) (400*Twist_msg.linear.x + 1440*Twist_msg.angular.z) / 67;
    }
    catch(std::exception &e)
    {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }

    DPRALTE060B080 IGV;
    IGV.getWriteAccess_Left();
    IGV.getWriteAccess_Right();
    std::cout << "Write Access Got." << std::endl;
    IGV.setVelocity_Left(velocity_left);
    IGV.setVelocity_Right(velocity_right);
    std::cout << "Velocity Set." << std::endl;

    int count = 1;
    while(count)
    {
        IGV.enableBridge_Left();
        IGV.enableBridge_Right();
        std::cout << "Bridge Enabled." << std::endl;
        ros::Duration(6.2).sleep();

        IGV.disableBridge_Left();
        IGV.disableBridge_Right();
        ROS_INFO_STREAM("Bridge Disabled.");

        odometry_left = IGV.getOdometry_Left();
        odometry_right = IGV.getOdometry_Right();

        std::cout << "Left odometry is: " << odometry_left << std::endl;
        std::cout << "Right odometry is: " << odometry_right << std::endl;

        ros::spinOnce();
        loop_rate.sleep();
        count--;
    }
}

void setupServoDrives()
{
}
