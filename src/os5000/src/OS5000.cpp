#include "os5000/OS5000.h"
#include <ros/ros.h>

OS5000::OS5000()
{
    port = "/dev/ttyUSB0";
    baudrate = 9600;
    yaw   = 0.0f;
    pitch = 0.0f;
    roll  = 0.0f;
}

OS5000::~OS5000()
{
    if(ImuSerial.isOpen())
    {
        ImuSerial.close();
    }
}

//============================================================================= Part 1
void OS5000::getOrientation()
{
    while(1)
    {
        RawData.clear();
        RawData = ImuSerial.readline(23, "\n");

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

//============================================================================= Part 2
void OS5000::setupSerialConnection()
{
    ImuSerial.setPort(port);
    ImuSerial.setBaudrate(baudrate);
    serial::Timeout To = serial::Timeout::simpleTimeout(1000);
    ImuSerial.setTimeout(To);

    while(!ImuSerial.isOpen()){
        std::cout << "\n--> Trying to connect..." << std::endl;
        ImuSerial.open();
    }
    std::cout << "\n--> OS5000 is connected!\n" << std::endl;
}
/*
void OS5000::initializeImu()
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
*/
