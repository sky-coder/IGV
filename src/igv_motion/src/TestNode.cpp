#include <ros/ros.h>
#include <std_msgs/String.h>
#include <iostream>
#include <string>

#include "serial/serial.h"

#define LEFT_PORT "/dev/pts/27"

int main(int argc, char **argv)
{
	ros::init(argc, argv, "igv_TestNode");
	ros::start();
	ros::Rate loop_rate(10);

	uint8_t getTxLeft[12] = {0xa5, 0x3e, 0x02, 0x07, 0x00, 0x01, 0x19, 0xb6,
	 						 0xff, 0x00, 0x03, 0xff};
	uint8_t enableBridgeLeft[12] = {0xa5, 0x3e, 0x02, 0x01, 0x00, 0x01, 0xab, 0x16,
								 	0x00, 0x00, 0x00, 0x00};
	uint8_t disableBridgeLeft[12] = {0xa5, 0x3e, 0x02, 0x01, 0x00, 0x01, 0xab, 0x16,
									 0x01, 0x00, 0x33, 0x31};	
	uint8_t setVelocityLeft[14] = {0xa5, 0x3e, 0x02, 0x45, 0x00, 0x02, 0x5a, 0x18,
								   0xb5, 0x01, 0x00, 0x00, 0x7a, 0xa4};

	// std::string getTxLeft = "A5 3E 02 07 00 01 19 B6 FF 00 03 FF";
	// std::string getTxLeft = "Hello";
	
	//TODO Implement setting timeout to main program.
	serial::Serial LeftPort;
	LeftPort.setPort(LEFT_PORT);
	LeftPort.setBaudrate(115200);
	serial::Timeout to = serial::Timeout::simpleTimeout(1000);
	LeftPort.setTimeout(to);
	LeftPort.open();

	if(ros::ok())
	{
		ros::spinOnce();

		ROS_INFO_STREAM("Port is opened.");
		LeftPort.write(getTxLeft, 12);
		ROS_INFO_STREAM("Tx Access Got.");
		LeftPort.write(setVelocityLeft, 14);
		ROS_INFO_STREAM("Velocity Set.");
		LeftPort.write(enableBridgeLeft, 12);
		ROS_INFO_STREAM("Bridge Enabled.");
		ros::Duration(5).sleep();
		LeftPort.write(disableBridgeLeft, 12);
		ROS_INFO_STREAM("Bridge Disabled.");
		
		loop_rate.sleep();
	}

	ros::shutdown();

	return 0;
}
