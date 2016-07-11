// ros includes
#include <ros/ros.h>
#include "dpralte060b080/DPRALTE060B080_Msg.h"

// cpp includes
#include <iostream>
#include "dpralte060b080/DPRALTE060B080.h"


int main(int argc, char **argv)
{
    ros::init(argc, argv, "igv_DPRALTE060B080_Node");

    DPRALTE060B080 IGV;
    dpralte060b080::DPRALTE060B080_Msg Msg;

    ros::start();
    ros::Rate loop_rate(10);

    int i = 10;
    int j = 2;


    while(j)
    {
        Msg.velocity_left = pow(-1, j) * i;
        Msg.velocity_right = pow(-1, j) * i;

        std::cout << "Left velocity is: " << Msg.velocity_left << std::endl;
        std::cout << "Right velocity is: " << Msg.velocity_right << std::endl;

        IGV.getWriteAccess_Left();
        IGV.getWriteAccess_Right();
        ROS_INFO_STREAM("Write Access Got.");

        IGV.setVelocity_Left(Msg.velocity_left);
        IGV.setVelocity_Right(Msg.velocity_right);
        ROS_INFO_STREAM("Velocity Set.");

        IGV.enableBridge_Left();
        IGV.enableBridge_Right();
        ROS_INFO_STREAM("Bridge Enabled.");
        ros::Duration(3).sleep();

        IGV.disableBridge_Left();
        IGV.disableBridge_Right();
        ROS_INFO_STREAM("Bridge Disabled.");

        Msg.odometry_left = IGV.getOdometry_Left();
        Msg.odometry_right = IGV.getOdometry_Right();

        std::cout << "Left odometry is: " << std::dec << Msg.odometry_left << std::endl;
        std::cout << "Right odometry is: " << std::dec << Msg.odometry_right << std::endl;

        ros::spinOnce();
        loop_rate.sleep();

        j--;
    }

    // ros::shutdown();

    return 0;
}
