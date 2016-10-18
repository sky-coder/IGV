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
    DPRALTE060B080 IGV;

    int count = 1;
    int demo_mode = 0;
    int demo_duration = 0;

    while(count)
    {
        std::cout << "Please select demo mode (1/2/3): ";
        std::cin >> demo_mode;
        std::cout << "\n";
        switch(demo_mode)
        {
            case 1:
                Twist_msg.linear.x = 0.5;
                Twist_msg.angular.z = 0.0;
                demo_duration = 2;
                std::cout << "Linear velocity (m/s): " << Twist_msg.linear.x << "\n";
                std::cout << "Angular velocity (rad/s): " << Twist_msg.angular.z << "\n";
                break;
            case 2:
                Twist_msg.linear.x = 0.0;
                Twist_msg.angular.z = M_PI / 3;
                demo_duration = 6;
                std::cout << "Linear velocity (m/s): " << Twist_msg.linear.x << "\n";
                std::cout << "Angular velocity (rad/s): " << Twist_msg.angular.z << "\n";
                break;
            case 3:
                Twist_msg.linear.x = 0.5;
                Twist_msg.angular.z = M_PI / 3;
                demo_duration = 2;
                std::cout << "Linear velocity (m/s): " << Twist_msg.linear.x << "\n";
                std::cout << "Angular velocity (rad/s): " << Twist_msg.angular.z << "\n";
                break;
            default:
                demo_duration = 0;
                std::cout << "Demo mode is not selected." << "\n";
                break;
        }

        // std::cout << "Please key in linear velocity (m/s): ";
        // std::cin >> Twist_msg.linear.x;
        // std::cout << std::endl;
        // std::cout << "Please key in angular velocity factor (sec/round): ";
        // std::cin >> angular_factor;
        // Twist_msg.angular.z = 2*M_PI / angular_factor;
        // std::cout << std::endl;

        try
        {
            velocity_left = 
                (int32_t) (4000*Twist_msg.linear.x - 1440*Twist_msg.angular.z) / 67;
            velocity_right = 
                (int32_t) (4000*Twist_msg.linear.x + 1440*Twist_msg.angular.z) / 67;
        }
        catch(std::exception &e)
        {
            std::cerr << "Exception caught: " << e.what() << std::endl;
        }

        IGV.getWriteAccess_Left();
        IGV.getWriteAccess_Right();
        std::cout << "Write Access Got." << std::endl;

        IGV.setVelocity_Left(velocity_left);
        IGV.setVelocity_Right(velocity_right);
        std::cout << "Positive Velocity Set." << std::endl;

        IGV.enableBridge_Left();
        IGV.enableBridge_Right();
        std::cout << "Bridge Enabled." << std::endl;
        ros::Duration(demo_duration).sleep();

        IGV.setVelocity_Left(0);
        IGV.setVelocity_Right(0);
        std::cout << "Zero Velocity Set." << std::endl;
        ros::Duration(1).sleep();

        IGV.setVelocity_Left(-velocity_left);
        IGV.setVelocity_Right(-velocity_right);
        std::cout << "Negative Velocity Set." << std::endl;
        ros::Duration(demo_duration).sleep();

        IGV.disableBridge_Left();
        IGV.disableBridge_Right();
        ROS_INFO_STREAM("Bridge Disabled.");

        odometry_left = IGV.getOdometry_Left();
        odometry_right = IGV.getOdometry_Right();

        std::cout << "Left odometry is: " << odometry_left << std::endl;
        std::cout << "Right odometry is: " << odometry_right << std::endl;

        ros::spinOnce();
        loop_rate.sleep();

        std::cout << "Another demo? (1/0): ";
        std::cin >> count;
    }
}
