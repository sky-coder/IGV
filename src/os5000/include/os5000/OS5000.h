// ros includes
#include <sensor_msgs/Imu.h>
#include <serial/serial.h>
#include <tf/transform_datatypes.h>

// cpp includes
#include <iostream>
#include <sstream>
#include <string>


#ifndef COVARIANCE_INITIALIZER //TODO change the name
#define COVARIANCE_INITIALIZER

#define PI 3.14159265358979323846 // define pi here instead of include math.h
#define IDENTITY 1.0f
#define INVALID -1.0f

#endif // COVARIANCE_INITIALIZER


#ifndef IGV_OS5000_H
#define IGV_OS5000_H

class OS5000
{
    public:
        OS5000();
        ~OS5000();

        // Part 1
        void getOrientation(); // line 19
        float getYaw();        // line 73
        float getPitch();      // line 78
        float getRoll();       // line 83

        // Part 2
        void setupSerialConnection(); // line 89
        // void initializeImu();

    public:
        std::string port;
        uint32_t baudrate;

    private:
        sensor_msgs::Imu ImuMsg;
        serial::Serial ImuSerial;

        std::string RawData; //TODO get a better identifier
        std::stringstream Orientation;
        uint32_t position, length;
        float yaw, pitch, roll;
};

# endif // IGV_OS5000_H
