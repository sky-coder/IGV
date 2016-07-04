#include <ros/ros.h>
#include <serial/serial.h>

#include <std_msgs/String.h>
#include <iostream>
#include <string>

#define LEFT_PORT "/dev/ttyUSB0"
#define RIGHT_PORT "/dev/ttyUSB1"

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

	//========================================================================
	uint8_t getTxRight[12] = {0xa5, 0x3f, 0x02, 0x07, 0x00, 0x01, 0xb3, 0xe7,
							  0xff, 0x00, 0x03, 0xff};
	uint8_t enableBridgeRight[12] = {0xa5, 0x3f, 0x02, 0x01, 0x00, 0x01, 0x01, 0x47,
									 0x00, 0x00, 0x00, 0x00};
	uint8_t disableBridgeRight[12] = {0xa5, 0x3f, 0x02, 0x01, 0x00, 0x01, 0x01, 0x47,
									  0x01, 0x00, 0x33, 0x31};	
	uint8_t setVelocityRight[14] = {0xa5, 0x3f, 0x02, 0x45, 0x00, 0x02, 0x96, 0x2b,
								   0xb5, 0x01, 0x00, 0x00, 0x7a, 0xa4};
	
	//TODO Implement setting timeout to main program.
	serial::Serial LeftPort;
	LeftPort.setPort(LEFT_PORT);
	LeftPort.setBaudrate(115200);
	serial::Timeout to = serial::Timeout::simpleTimeout(1000);
	LeftPort.setTimeout(to);
	LeftPort.open();
	ROS_INFO_STREAM("Left Port is opened.");

	// serial::Serial RightPort;
	// RightPort.setPort(RIGHT_PORT);
	// RightPort.setBaudrate(115200);
	// serial::Timeout to = serial::Timeout::simpleTimeout(1000);
	// RightPort.setTimeout(to);
	// RightPort.open();
	// ROS_INFO_STREAM("Right Port is opened.");

	if(ros::ok())
	{
		ros::spinOnce();

		LeftPort.write(getTxLeft, 12);
		ROS_INFO_STREAM("Tx Access Got.");
		LeftPort.write(setVelocityLeft, 14);
		ROS_INFO_STREAM("Velocity Set.");
		LeftPort.write(enableBridgeLeft, 12);
		ROS_INFO_STREAM("Bridge Enabled.");
		ros::Duration(2).sleep();
		LeftPort.write(disableBridgeLeft, 12);
		ROS_INFO_STREAM("Bridge Disabled.");
		
		// RightPort.write(getTxRight, 12);
		// ROS_INFO_STREAM("Tx Access Got.");
		// RightPort.write(setVelocityRight, 14);
		// ROS_INFO_STREAM("Velocity Set.");
		// RightPort.write(enableBridgeRight, 12);
		// ROS_INFO_STREAM("Bridge Enabled.");
		// ros::Duration(2).sleep();
		// RightPort.write(disableBridgeRight, 12);
		// ROS_INFO_STREAM("Bridge Disabled.");
		
		 loop_rate.sleep();
	}

	ros::shutdown();

	return 0;
}
