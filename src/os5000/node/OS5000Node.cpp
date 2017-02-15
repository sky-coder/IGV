// ros includes
#include <ros/ros.h>
#include <signal.h>

// cpp includes
#include "os5000/OS5000.h"

//int count = 10;
void intHandle(int sig)
{
    ros::shutdown();
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "OS5000_Node");
    ros::start();
    ros::Rate loop_rate(10);

    OS5000 Compass;

    while(ros::ok)
    {
        Compass.getOrientation();

        //count--;
        //signal(SIGINT, intHandle);
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
