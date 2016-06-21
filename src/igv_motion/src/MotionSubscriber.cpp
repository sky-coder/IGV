#include <ros/ros.h>
#include <std_msgs/Int32MultiArray.h>

#include <iostream>

void MotionSubCB(const std_msgs::Int32MultiArray::ConstPtr &VelocityArray)
{
	// ROS_INFO("The velocity is: [%s]", VelocityArray.data.c_str());
	std::cout << *VelocityArray << std::endl;
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "igv_MotionSubscriber");
	ros::NodeHandle nh;
	ros::Subscriber Motion_sub = nh.subscribe("MotionSignal", 100, MotionSubCB);

	ros::spin();
}
