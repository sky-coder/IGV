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

#define LEFT_PORT  "/dev/ttyUSB0"
#define RIGHT_PORT "/dev/ttyUSB1"
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
        void getWriteAccess_Left();  // line 40
        void enableBridge_Left();    // line 47
        void disableBridge_Left();   // line 54
        void setVelocity_Left(int);  // line 61

        // void getReadAccess_Left();   // line 77
        int getOdometry_Left();      // line 83

        // ---------- right
        void getWriteAccess_Right(); // line
        void enableBridge_Right();   // line
        void disableBridge_Right();  // line
        void setVelocity_Right(int); // line

        // void getReadAccess_Right();  // line
        int getOdometry_Right();     // line

        // Part 2
        void getVelocityArray();     // line 

        // Part 3
        void resetCRC();             // line 
        void computeCRC(int);        // line 
        void getCRCArray();          // line 

        // Part 4
        void converttoOdometry();    // line

        // Part 5
        void setTxBuffer(int, int, int, int, int, int,
                         unsigned int, unsigned int,
                         int, int,
                         unsigned int, unsigned int);

        void setTxBuffer(int, int, int, int, int, int,
                         unsigned int, unsigned int,
                         int, int, int, int,
                         unsigned int, unsigned int);

        void setTxBuffer(int, int, int, int, int, int,
                         unsigned int, unsigned int);

    private:
        // Part 6
        serial::Serial LeftPort;
        serial::Serial RightPort;
        void setupSerialCommunication();
        uint8_t *TxBuffer, *RxBuffer;

        int velocity;
        int *VelocityArray;

        unsigned int crc_accumulator, crc_poly, crc_buffer, crc_highest_bit;
        unsigned int *CRCArray;

        int home_left, home_right, odometry, temp;
        uint8_t *OdometryArray;
        std::stringstream Odo_ss;
};

#endif // IGV_DPRALTE060B080R_H
