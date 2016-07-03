// ros includes
#include <ros/ros.h>

// cpp includes
#include "os5000/OS5000.h"

int count = 10;

int main(int argc, char **argv)
{
	ros::init(argc, argv, "OS5000_Node");
	ros::start();
	ros::Rate loop_rate(10);

	OS5000 Compass;

	while(count)
	{
		Compass.getOrientation();

		count--;
		ros::spinOnce();
		loop_rate.sleep();
	}

	ros::shutdown();

	return 0;
}
