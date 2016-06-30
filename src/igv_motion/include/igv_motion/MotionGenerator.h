#include <serial/serial.h>

#ifndef MOTIONGENERATOR_H
#define MOTIONGENERATOR_H

#define LEFT_PORT "/dev/ttyUSB0"
#define RIGHT_PORT "/dev/ttyUSB1"
#define CFG_CMD_SIZE 12
#define VEL_CMD_SIZE 14

static uint8_t ConfigurationCommand[12];
static uint8_t VelocityCommand[14];

class MotionGenerator
{
    public:
        MotionGenerator();
        virtual ~MotionGenerator();
		
		// Part 1
        void getVelocityArray();

		// Part 2
		void resetCRC();
		void computeCRC(int);
        void getCRCArray();

		// Part 3
		void getWriteAccess_Left();
		void enableBridge_Left();
		void disableBridge_Left();
		void setVelocity_Left(int);

		void getWriteAccess_Right();
		void enableBridge_Right();
		void disableBridge_Right();
		void setVelocity_Right(int);

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

#endif // MOTIONGENERATOR_H
