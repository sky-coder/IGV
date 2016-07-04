// ros includes
#include <serial/serial.h>

#ifndef IGV_DPRALTE060B080_H
#define IGV_DPRALTE060B080_H

#define LEFT_PORT "/dev/ttyUSB0"
#define RIGHT_PORT "/dev/ttyUSB1"
#define CFG_CMD_SIZE 12
#define VEL_CMD_SIZE 14

static uint8_t ConfigurationCommand[12];
static uint8_t VelocityCommand[14];

class DPRALTE060B080
{
    public:
        DPRALTE060B080();
        ~DPRALTE060B080();
		
        // Part 1
        void getVelocityArray();    // line 34

        // Part 2
        void resetCRC();            // line 50
        void computeCRC(int);       // line 57
        void getCRCArray();         // line 77

        // Part 3
        void getWriteAccess_Left(); // line 98
        void enableBridge_Left();   // line 105
        void disableBridge_Left();  // line 112
        void setVelocity_Left(int); // line 119
        void getReadAccess_Left();  // line 135

        void getWriteAccess_Right();
        void enableBridge_Right();
        void disableBridge_Right();
        void setVelocity_Right(int);
        void getReadAccess_Right();

        // Part 4
        void setConfigurationCommand(int, int, int, int, int, int, int, int,
                                     int, int, int, int);
        void setVelocityCommand(int, int, int, int, int, int, int, int,
                                int, int, int, int, int, int);

    private:
        int velocity, highest_bit, CRCBuffer;
        int *VelocityArray, *CRCArray;
        unsigned int crc_accumulator, crc_poly;

        serial::Serial LeftPort;
        serial::Serial RightPort;
};

#endif // IGV_DPRALTE060B080R_H
