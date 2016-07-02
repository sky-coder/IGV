// ros includes
#include <ros/ros.h>
#include <serial/serial.h>

// cpp includes
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


std::string YPR;
size_t bytes = 0;
int count = 10;
int pos, len;
float yaw, pitch, roll;

std::stringstream Yaw, Pitch, Roll;

int main(int argc, char **argv)
{
	ros::init(argc, argv, "OS5000_Node");
	ros::start();
	ros::Rate loop_rate(10);

	serial::Serial OSPort;
	OSPort.setPort("/dev/ttyUSB0");
	OSPort.setBaudrate(9600);
	serial::Timeout to = serial::Timeout::simpleTimeout(1000);
	OSPort.setTimeout(to);
	OSPort.open();
	ROS_INFO_STREAM("OS5000 Port is opened.");

	while(count)
	{

		YPR.clear();
		Yaw.str(std::string());
		Yaw.clear();
		Pitch.str(std::string());
		Pitch.clear();
		Roll.str(std::string());
		Roll.clear();

		YPR = OSPort.readline(22, "\n");

		// if(YPR.find("$") == std::string::npos || YPR.find("$") != 0){
		if(YPR.find("$") != 0){
			ROS_INFO_STREAM("Skipped 1");
			continue;
		}
		else if(!YPR.find("C") || !YPR.find("P") || !YPR.find("R") || !YPR.find("*")){
			ROS_INFO_STREAM("Skipped 2");
			continue;
		}

		ROS_INFO("Signal length: %d", (int)bytes);
		ROS_INFO("Signal read: %s", YPR.c_str());
		// for (int i = 0 ; i < YPR.length(); i++){
		// 	std::cout << YPR[i];
		// }
		// std::cout << std::endl;

		pos = YPR.find("C") + 1;
		len = YPR.find("P") - YPR.find("C") - 1;
		Yaw << YPR.substr(pos, len);
		Yaw >> yaw;
		ROS_INFO("Yaw is: %f", yaw);

		pos = YPR.find("P") + 1;
		len = YPR.find("R") - YPR.find("P") - 1;
		Pitch << YPR.substr(pos, len);
		Pitch >> pitch;
		ROS_INFO("Pitch is: %f", pitch);

		pos = YPR.find("R") + 1;
		len = YPR.find("*") - YPR.find("R") - 1;
		Roll << YPR.substr(pos, len);
		Roll >> roll;
		ROS_INFO("Roll is: %f", roll);

		ros::spinOnce();
		loop_rate.sleep();
		count--;
	}

	ros::shutdown();

	return 0;
}
