#include <ros/ros.h>
#include <std_msgs/String.h>
#include <iostream>

#include "igv_motion/MotionGenerator.hpp"
#include "igv_motion/MotionMsg.h"

void MotionCB(const igv_motion::MotionMsg::ConstPtr &Msg)
{
	std::cout << "velocity is:" << Msg->velocity << std::endl;
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

	int i = 0;

	while(ros::ok())
	{
		Msg.velocity = i++;
		IGV.setVelocity_Left(Msg.velocity);

		Pub.publish(Msg);

		ros::spinOnce();
		loop_rate.sleep();
	}

	ros::spin();

	return 0;
}
