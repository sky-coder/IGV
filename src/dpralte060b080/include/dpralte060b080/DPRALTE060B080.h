// ros includes
#include <ros/ros.h>
#include <serial/serial.h>
#include "dpralte060b080/DPRALTE060B080_Msg.h"

// cpp includes
#include <sstream>
#include <string>

#ifndef IGV_DPRALTE060B080_H
#define IGV_DPRALTE060B080_H


#ifndef IGV_DPRALTE060B080_SERIAL_PARAM
#define IGV_DPRALTE060B080_SERIAL_PARAM

#define LEFT_PORT  "/dev/ttyUSB1"
#define RIGHT_PORT "/dev/ttyUSB2"
// #define WRITE_TO_LENGTH  12
// #define READ_FROM_LENGTH  8
// #define VEL_CMD_LENGTH   14
// #define FLAG_BUF_LENGTH   8
// #define ODO_BUF_LENGTH   14

#endif // IGV_DPRALTE060B080_SERIAL_PARAM


// static uint8_t WriteToDriver[WRITE_TO_LENGTH];
// static uint8_t ReadFromDriver[READ_FROM_LENGTH];
// static uint8_t VelocityTarget[VEL_CMD_LENGTH];
// static uint8_t FlagBuffer[FLAG_BUF_LENGTH];
// static uint8_t OdoBuffer[ODO_BUF_LENGTH];


class DPRALTE060B080
{
    public:
        DPRALTE060B080();
        ~DPRALTE060B080();

        // Part 1
        // ---------- left
        void getWriteAccess_Left();  // line 43
        void enableBridge_Left();    // line 57
        void disableBridge_Left();   // line 71
        void setVelocity_Left(int);  // line 85

        /*
           The servo drives have read access by default. So far, it is
           unnecessary to get read access again.
        */
        // void getReadAccess_Left(); // line 106

        /*
           The 'distance' here and all the other distances in the package mean
           the odometry of the wheels. It is changed to this identifier to
           distinguish from the 'nav_msg/Odometry' defined by ROS.
        */
        int getDistance_Left();      // line 116

        // ---------- right
        void getWriteAccess_Right(); // line 139
        void enableBridge_Right();   // line 153
        void disableBridge_Right();  // line 167
        void setVelocity_Right(int); // line 181

        // void getReadAccess_Right(); // line 202
        int getDistance_Right();     // line 209

        // Part 2
        void getVelocityArray();     // line 228

        // Part 3
        void resetCRC();             // line 244
        void computeCRC(int);        // line 251

        /*
           This function will call 'getVelocityArray()' when it runs, so be
           aware that this is no need to call 'getVelocityArray()' again when
           setting target velocity.
        */
        void getCRCArray();          // line 271

        // Part 4
        void convertToDistance();    // line 291

        // Part 5
        // line 309
        void setTxBuffer(int, int, int, int, int, int,
                         unsigned int, unsigned int,
                         int, int,
                         unsigned int, unsigned int);

        // line 332
        void setTxBuffer(int, int, int, int, int, int,
                         unsigned int, unsigned int,
                         int, int, int, int,
                         unsigned int, unsigned int);

        // line 357
        void setTxBuffer(int, int, int, int, int, int,
                         unsigned int, unsigned int);

    private:
        // Part 6
        serial::Serial LeftPort;
        serial::Serial RightPort;
        void setupSerialCommunication(); // line 373
        uint8_t *TxBuffer, *RxBuffer;

        int velocity;
        int *VelocityArray;

        unsigned int crc_accumulator, crc_poly, crc_buffer, crc_highest_bit;
        unsigned int *CRCArray;

        int home_left, home_right, distance, temp;
        uint8_t *DistanceArray;
        std::stringstream Dis_ss;
};

#endif // IGV_DPRALTE060B080R_H
