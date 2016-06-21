#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Int32MultiArray.h>

// #include <iostream>
// #include "igv_motion/MotionGenerator.hpp"

//extern char ConfigurationCommand[12];
//extern char VelocityCommand[14];

int main(int argc, char **argv)
{
	ros::init(argc, argv, "igv_MotionPublisher");
	ros::NodeHandle nh;
	ros::Publisher Motion_pub = nh.advertise<std_msgs::Int32MultiArray>("MotionSignal", 100);
	ros::Rate loop_rate(10);

	// MotionGenerator IGV;

	while(ros::ok())
	{
		std_msgs::Int32MultiArray VelocityArray;

		VelocityArray.data.push_back(10);

		//IGV.setVelocity_Left(velocity);

		//VelocityArray.data.insert(VelocityArray.data.end(), VelocityCommand, VelocityCommand+14);
		
		Motion_pub.publish(VelocityArray);

		ros::spinOnce();
		loop_rate.sleep();
	}

	return 0;

}
