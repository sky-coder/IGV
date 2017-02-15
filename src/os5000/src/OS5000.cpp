#include "os5000/OS5000.h"
#include <ros/ros.h>

OS5000::OS5000()
    :yaw(-1.0f), pitch(-1.0f), roll(-1.0f)
{
    OS5000::setupSerialConnection();
}

OS5000::~OS5000()
{
    if(OSPort.isOpen())
    {
        OSPort.close();
    }
}

//============================================================================= Part 1
void OS5000::getOrientation()
{
    while(1)
    {
        RawData.clear();
        RawData = OSPort.readline(23, "\n");

        if(RawData.find("$") == std::string::npos || RawData.find("$") != 0)
        {
            std::cout << "--> Frame skipped: wrong/no start bit.\n" << std::endl;
            std::cout << RawData << std::endl;
            continue;
        }
        else if(!RawData.find("C") || !RawData.find("P") || 
                !RawData.find("R") || !RawData.find("*"))
        {
            std::cout << "--> Frame skipped: incomplete frame." << std::endl;
            std::cout << RawData << std::endl;
            continue;
        }

        std::cout << "--> New data:" << std::endl;

        Orientation.str(std::string());
        Orientation.clear();
        position = RawData.find("C") + 1;
        length = RawData.find("P") - RawData.find("C") - 1;
        Orientation << RawData.substr(position, length);
        Orientation >> yaw;
       // ROS_INFO("Yaw is: %f", yaw);

        Orientation.str(std::string());
        Orientation.clear();
        position = RawData.find("P") + 1;
        length = RawData.find("R") - RawData.find("P") - 1;
        Orientation << RawData.substr(position, length);
        Orientation >> pitch;
        //ROS_INFO("Pitch is: %f", pitch);

        Orientation.str(std::string());
        Orientation.clear();
        position = RawData.find("R") + 1;
        length = RawData.find("*") - RawData.find("R") - 1;
        Orientation << RawData.substr(position, length);
        Orientation >> roll;
        //ROS_INFO("Roll is: %f\n", roll);

        ROS_INFO("Data: (%f, %f, %f)\n",yaw, pitch, roll);

        break;
    }
    //ros::shutdown();
}

float OS5000::getYaw()
{
    return yaw;
}

float OS5000::getPitch()
{
    return pitch;
}

float OS5000::getRoll()
{
    return roll;
}

//============================================================================= Part
void OS5000::setupSerialConnection()
{
    OSPort.setPort("/dev/ttyUSB1");
    OSPort.setBaudrate(9600);
    serial::Timeout to = serial::Timeout::simpleTimeout(1000);
    OSPort.setTimeout(to);

    while(!OSPort.isOpen()){
        std::cout << "\n--> Trying to connect..." << std::endl;
        OSPort.open();
    }
    std::cout << "\n--> OS5000 is connected!\n" << std::endl;
}

void OS5000::initializeIMU()
{
    
    ImuMsg.orientation_covariance[0] = IDENTITY;
    ImuMsg.orientation_covariance[4] = IDENTITY;
    ImuMsg.orientation_covariance[8] = IDENTITY;

    ImuMsg.angular_velocity_covariance[0] = INVALID;
    ImuMsg.angular_velocity_covariance[4] = INVALID;
    ImuMsg.angular_velocity_covariance[8] = INVALID;

    ImuMsg.linear_acceleration_covariance[0] = INVALID;
    ImuMsg.linear_acceleration_covariance[4] = INVALID;
    ImuMsg.linear_acceleration_covariance[8] = INVALID;
}
