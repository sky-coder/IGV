#include "igv_motion/MotionGenerator.hpp"
#include <iostream>

MotionGenerator::MotionGenerator()
{
	VelocityArray = new int[4];
	CRCArray = new int[2];

	LeftPort.setPort(LEFT_PORT);
	RightPort.setPort(RIGHT_PORT);
}

MotionGenerator::~MotionGenerator()
{
	if(VelocityArray){
		delete[] VelocityArray;
	}
	if(CRCArray){
		delete[] CRCArray;
	}
}

//============================================================================ Part 1
// void MotionGenerator::setVelocity(int vel_in)
// {
// 	velocity = vel_in;
// }

void MotionGenerator::getVelocityArray()
{
	if(velocity >= 0){
		velocity = velocity * 43.7 + 0.5;
	}
	else{
		velocity = velocity * 43.7 - 0.5;
	}

	for(int i = 0; i < 4; i++){
		VelocityArray[i] = (velocity >> (8*i)) & 0xff;
	}
}


//============================================================================= Part 2
void MotionGenerator::resetCRC()
{
	// call before computing new CRC
	crc_accumulator = 0;
	crc_poly = 0x0810;
}

void MotionGenerator::computeCRC(int crc_input)
{
	CRCBuffer = crc_input;
	// compute CRC using bit-by-bit method
	for(int i = 0; i < 8; i++)
	{
		highest_bit = (CRCBuffer >> 7) & 1;

		if(crc_accumulator & 0x8000){
			crc_accumulator = ((crc_accumulator ^ crc_poly) << 1) + (highest_bit ^ 1);
		}
		else{
			crc_accumulator = (crc_accumulator  << 1) + highest_bit;
		}

		crc_accumulator &= 0x0fffff;
		CRCBuffer <<= 1;
	}
}

void MotionGenerator::getCRCArray()
{
	// getting CRC based on provided velocity
	MotionGenerator::resetCRC();

	MotionGenerator::getVelocityArray();

	for(int i = 0; i < 4; i++){
		MotionGenerator::computeCRC(VelocityArray[i]);
	}
	MotionGenerator::computeCRC(0);
	MotionGenerator::computeCRC(0);

	// storing CRC into array form
	for(int j = 0; j < 2; j++){
		CRCArray[1-j] = ((int)crc_accumulator >> (8*j)) & 0xff;
	}
}

//============================================================================= Part 3
//------------------------------------------------------------------------ Left
void MotionGenerator::getWriteAccess_Left()
{
	MotionGenerator::setConfigurationCommand(0xa5, 0x3e, 0x02, 0x07, 0x00, 0x01, 0x19, 0xb6,
											 0xff, 0x00, 0x03, 0xff);
	LeftPort.write(ConfigurationCommand, CFG_CMD_SIZE);
}

void MotionGenerator::enableBridge_Left()
{
	MotionGenerator::setConfigurationCommand(0xa5, 0x3e, 0x02, 0x01, 0x00, 0x01, 0xab, 0x16,
											 0x00, 0x00, 0x00, 0x00);
	LeftPort.write(ConfigurationCommand, CFG_CMD_SIZE);
}

void MotionGenerator::disableBridge_Left()
{
	MotionGenerator::setConfigurationCommand(0xa5, 0x3e, 0x02, 0x01, 0x00, 0x01, 0xab, 0x16,
											 0x01, 0x00, 0x33, 0x31);
	LeftPort.write(ConfigurationCommand, CFG_CMD_SIZE);
}

void MotionGenerator::setVelocity_Left(int vel_in)
{
	velocity = vel_in;
	MotionGenerator::getVelocityArray();
	MotionGenerator::getCRCArray();

	MotionGenerator::setVelocityCommand(0xa5, 0x3e, 0x02, 0x45, 0x00, 0x02, 0x5a, 0x18,
										VelocityArray[0],
										VelocityArray[1],
										VelocityArray[2],
										VelocityArray[3],
										CRCArray[0],
										CRCArray[1]);
	LeftPort.write(VelocityCommand, VEL_CMD_SIZE);

	std::cout << "vel cmd is: ";
	for(int m = 0; m < 14; m++){
		std::cout << VelocityCommand[m];
	}
	std::cout << std::endl;
}
//----------------------------------------------------------------------- Right
void MotionGenerator::getWriteAccess_Right()
{
	MotionGenerator::setConfigurationCommand(0xa5, 0x3f, 0x02, 0x07, 0x00, 0x01, 0xb3, 0xe7,
											 0xff, 0x00, 0x03, 0xff);
	RightPort.write(ConfigurationCommand, CFG_CMD_SIZE);
}

void MotionGenerator::enableBridge_Right()
{
	MotionGenerator::setConfigurationCommand(0xa5, 0x3f, 0x02, 0x01, 0x00, 0x01, 0x01, 0x47,
											 0x00, 0x00, 0x00, 0x00);
	RightPort.write(ConfigurationCommand, CFG_CMD_SIZE);
}

void MotionGenerator::disableBridge_Right()
{
	MotionGenerator::setConfigurationCommand(0xa5, 0x3f, 0x02, 0x01, 0x00, 0x01, 0x01, 0x47,
											 0x01, 0x00, 0x33, 0x31);
	RightPort.write(ConfigurationCommand, CFG_CMD_SIZE);
}

void MotionGenerator::setVelocity_Right(int vel_in)
{
	velocity = vel_in;
	MotionGenerator::getVelocityArray();
	MotionGenerator::getCRCArray();

	MotionGenerator::setVelocityCommand(0xa5, 0x3f, 0x02, 0x45, 0x00, 0x02, 0x96, 0x2b,
										VelocityArray[0],
										VelocityArray[1],
										VelocityArray[2],
										VelocityArray[3],
										CRCArray[0],
										CRCArray[1]);
	RightPort.write(VelocityCommand, VEL_CMD_SIZE);
}
//============================================================================= Part 4
void MotionGenerator::setConfigurationCommand(
		// header section
		int sof, int address, int controlbyte, int index, int offset, 
		int header_datawords, int headercrc_0, int headercrc_1,
		// data section
		int datafield_0, int datafield_1, 
		int datacrc_0, int datacrc_1)
{
	ConfigurationCommand[0]  = sof;
	ConfigurationCommand[1]  = address;
	ConfigurationCommand[2]  = controlbyte;
	ConfigurationCommand[3]  = index;
	ConfigurationCommand[4]  = offset;
	ConfigurationCommand[5]  = header_datawords;
	ConfigurationCommand[6]  = headercrc_0;
	ConfigurationCommand[7]  = headercrc_1;

	ConfigurationCommand[8]  = datafield_0;
	ConfigurationCommand[9]  = datafield_1;
	ConfigurationCommand[10] = datacrc_0;
	ConfigurationCommand[11] = datacrc_1;
}

void MotionGenerator::setVelocityCommand(
		// header section
		int sof, int address, int controlbyte, int index, int offset, 
		int header_datawords, int headercrc_0, int headercrc_1,
		// data section
		int datafield_0, int datafield_1, int datafield_2, int datafield_3, 
		int datacrc_0, int datacrc_1)
{
	VelocityCommand[0]  = sof;
	VelocityCommand[1]  = address;
	VelocityCommand[2]  = controlbyte;
	VelocityCommand[3]  = index;
	VelocityCommand[4]  = offset;
	VelocityCommand[5]  = header_datawords;
	VelocityCommand[6]  = headercrc_0;
	VelocityCommand[7]  = headercrc_1;
	
	VelocityCommand[8]  = datafield_0;
	VelocityCommand[9]  = datafield_1;
	VelocityCommand[10] = datafield_2;
	VelocityCommand[11] = datafield_3;
	VelocityCommand[12] = datacrc_0;
	VelocityCommand[13] = datacrc_1;
}
//============================================================================= Part 5
//void MotionGenerator::transmitCommand()
//{
//	s;
//}
