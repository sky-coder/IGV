#include <ros/ros.h>
#include <std_msgs/String.h>
#include <iostream>

#include "igv_motion/MotionGenerator.h"
#include "igv_motion/MotionMsg.h"

void MotionCB(const igv_motion::MotionMsg::ConstPtr &Msg)
{
	std::cout << "left velocity is:" << Msg->velocity_left << std::endl;
	std::cout << "right velocity is:" << Msg->velocity_left << std::endl;
}


int main(int argc, char **argv)
{
	ros::init(argc, argv, "igv_MotionNode");
	ros::NodeHandle nh;
	ros::Publisher Pub = nh.advertise<igv_motion::MotionMsg>("MotionSignal", 100);
	ros::Subscriber Sub = nh.subscribe("MotionSignal", 100, MotionCB);
	ros::Rate loop_rate(10);

	MotionGenerator IGV;
	igv_motion::MotionMsg Msg;

	int i = 10;

	Msg.velocity_left = -i;
	Msg.velocity_right = i;

	if(ros::ok())
	{
		ros::spinOnce();

		IGV.getWriteAccess_Left();
		IGV.getWriteAccess_Right();
		ROS_INFO_STREAM("Write Access Got.");

		IGV.setVelocity_Left(Msg.velocity_left);
		IGV.setVelocity_Right(Msg.velocity_right);
		ROS_INFO_STREAM("Velocity Set.");

		IGV.enableBridge_Left();
		IGV.enableBridge_Right();
		ROS_INFO_STREAM("Bridge Enabled.");
		ros::Duration(2).sleep();

		IGV.disableBridge_Left();
		IGV.disableBridge_Right();
		ROS_INFO_STREAM("Bridge Disabled.");

		Pub.publish(Msg);

		loop_rate.sleep();
	}

	ros::shutdown();

	return 0;
}
