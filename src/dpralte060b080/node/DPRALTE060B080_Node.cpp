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
    ros::Rate loop_rate(10);
    ros::spin();

    int i = 10;
    int j = 2;

    IGV.Msg.velocity_left = -i;
    IGV.Msg.velocity_right = i;

    while(j)
    {
        j--;

        IGV.getWriteAccess_Left();
        IGV.getWriteAccess_Right();
        ROS_INFO_STREAM("Write Access Got.");

        IGV.setVelocity_Left(IGV.Msg.velocity_left);
        IGV.setVelocity_Right(IGV.Msg.velocity_right);
        ROS_INFO_STREAM("Velocity Set.");

        IGV.enableBridge_Left();
        IGV.enableBridge_Right();
        ROS_INFO_STREAM("Bridge Enabled.");
        ros::Duration(3).sleep();

        IGV.disableBridge_Left();
        IGV.disableBridge_Right();
        ROS_INFO_STREAM("Bridge Disabled.");

        IGV.Msg.odometry_left = IGV.getOdometry_Left();
        IGV.Msg.odometry_right = IGV.getOdometry_Right();

        ros::spinOnce();
        loop_rate.sleep();
    }

    ros::shutdown();

    return 0;
}
