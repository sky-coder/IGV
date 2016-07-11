// cpp includes
#include "dpralte060b080/DPRALTE060B080.h"

DPRALTE060B080::DPRALTE060B080()
{
    VelocityArray = new int[4];
    CRCArray = new unsigned int[2];
    OdometryArray = new uint8_t[4];

    DPRALTE060B080::setupSerialCommunication();
    // DPRALTE060B080::setupMsgMonitoring();
}

DPRALTE060B080::~DPRALTE060B080()
{
    if(VelocityArray){
        delete[] VelocityArray;
    }
    if(CRCArray){
        delete[] CRCArray;
    }
    if(OdometryArray){
        delete[] OdometryArray;
    }
}

//============================================================================= Part 1
//------------------------------------------------------------------------ Left
void DPRALTE060B080::getWriteAccess_Left()
{
    DPRALTE060B080::setWriteToDriver(0xa5, 0x3e, 0x02, 0x07, 0x00, 0x01, 0x19, 0xb6,
                                     0xff, 0x00, 0x03, 0xff);
    LeftPort.write(WriteToDriver, WRITE_TO_LENGTH);
    LeftPort.read(FlagBuffer, FLAG_BUF_LENGTH);
}

void DPRALTE060B080::enableBridge_Left()
{
    DPRALTE060B080::setWriteToDriver(0xa5, 0x3e, 0x02, 0x01, 0x00, 0x01, 0xab, 0x16,
                                     0x00, 0x00, 0x00, 0x00);
    LeftPort.write(WriteToDriver, WRITE_TO_LENGTH);
    LeftPort.read(FlagBuffer, FLAG_BUF_LENGTH);
}

void DPRALTE060B080::disableBridge_Left()
{
    DPRALTE060B080::setWriteToDriver(0xa5, 0x3e, 0x02, 0x01, 0x00, 0x01, 0xab, 0x16,
                                     0x01, 0x00, 0x33, 0x31);
    LeftPort.write(WriteToDriver, WRITE_TO_LENGTH);
    LeftPort.read(FlagBuffer, FLAG_BUF_LENGTH);
}

void DPRALTE060B080::setVelocity_Left(int vel_in)
{
    velocity = vel_in;
    DPRALTE060B080::getVelocityArray();
    DPRALTE060B080::getCRCArray();

    DPRALTE060B080::setVelocityTarget(0xa5, 0x3e, 0x02, 0x45, 0x00, 0x02, 0x5a, 0x18,
                                      VelocityArray[0],
                                      VelocityArray[1],
                                      VelocityArray[2],
                                      VelocityArray[3],
                                      CRCArray[0],
                                      CRCArray[1]);
    LeftPort.write(VelocityTarget, VEL_CMD_LENGTH);
    LeftPort.read(FlagBuffer, FLAG_BUF_LENGTH);
}

void DPRALTE060B080::getReadAccess_Left()
{
    DPRALTE060B080::setReadFromDriver(0xa5, 0x3e, 0x01, 0x07, 0x00, 0x01, 0x82, 0x6a);
    LeftPort.write(ReadFromDriver, READ_FROM_LENGTH);
    LeftPort.read(FlagBuffer, FLAG_BUF_LENGTH);
}

int DPRALTE060B080::getOdometry_Left()
{
    DPRALTE060B080::setReadFromDriver(0xa5, 0x3e, 0x01, 0x12, 0x00, 0x02, 0x1a, 0x9a);
    LeftPort.write(ReadFromDriver, READ_FROM_LENGTH);
    LeftPort.read(OdoBuffer, ODO_BUF_LENGTH);


    std::cout << "The Odo Buffer is: " << std::endl;
    for(int i =0; i < ODO_BUF_LENGTH; i++){
        std::cout << std::hex << signed(OdoBuffer[i]) << " ";
    }
    std::cout << std::endl;

    DPRALTE060B080::converttoOdometry();
    return odometry;
}
//----------------------------------------------------------------------- Right
void DPRALTE060B080::getWriteAccess_Right()
{
    DPRALTE060B080::setWriteToDriver(0xa5, 0x3f, 0x02, 0x07, 0x00, 0x01, 0xb3, 0xe7,
                                     0xff, 0x00, 0x03, 0xff);
    RightPort.write(WriteToDriver, WRITE_TO_LENGTH);
    RightPort.read(FlagBuffer, FLAG_BUF_LENGTH);
}

void DPRALTE060B080::enableBridge_Right()
{
    DPRALTE060B080::setWriteToDriver(0xa5, 0x3f, 0x02, 0x01, 0x00, 0x01, 0x01, 0x47,
                                     0x00, 0x00, 0x00, 0x00);
    RightPort.write(WriteToDriver, WRITE_TO_LENGTH);
    RightPort.read(FlagBuffer, FLAG_BUF_LENGTH);
}

void DPRALTE060B080::disableBridge_Right()
{
    DPRALTE060B080::setWriteToDriver(0xa5, 0x3f, 0x02, 0x01, 0x00, 0x01, 0x01, 0x47,
                                     0x01, 0x00, 0x33, 0x31);
    RightPort.write(WriteToDriver, WRITE_TO_LENGTH);
    RightPort.read(FlagBuffer, FLAG_BUF_LENGTH);
}

void DPRALTE060B080::setVelocity_Right(int vel_in)
{
    velocity = vel_in;
    DPRALTE060B080::getVelocityArray();
    DPRALTE060B080::getCRCArray();
	
    DPRALTE060B080::setVelocityTarget(0xa5, 0x3f, 0x02, 0x45, 0x02, 0x02, 0x96, 0x2b,
                                      VelocityArray[0],
                                      VelocityArray[1],
                                      VelocityArray[2],
                                      VelocityArray[3],
                                      CRCArray[0],
                                      CRCArray[1]);
    RightPort.write(VelocityTarget, VEL_CMD_LENGTH);
    RightPort.read(FlagBuffer, FLAG_BUF_LENGTH);
}

void DPRALTE060B080::getReadAccess_Right()
{
    DPRALTE060B080::setReadFromDriver(0xa5, 0x3f, 0x01, 0x07, 0x00, 0x01, 0x28, 0x3b);
    RightPort.write(ReadFromDriver, READ_FROM_LENGTH);
    RightPort.read(FlagBuffer, FLAG_BUF_LENGTH);
}

int DPRALTE060B080::getOdometry_Right()
{
    DPRALTE060B080::setReadFromDriver(0xa5, 0x3f, 0x01, 0x12, 0x00, 0x02, 0xb0, 0xcb);
    RightPort.write(ReadFromDriver, READ_FROM_LENGTH);
    RightPort.read(OdoBuffer, ODO_BUF_LENGTH);

    DPRALTE060B080::converttoOdometry();
    return odometry;
}


//============================================================================= Part 2
void DPRALTE060B080::getVelocityArray()
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


//============================================================================= Part 3
void DPRALTE060B080::resetCRC()
{
    // call before computing new CRC
    crc_accumulator = 0;
    crc_poly = 0x0810;
}

void DPRALTE060B080::computeCRC(int crc_input)
{
    crc_buffer = crc_input;
    // compute CRC using bit-by-bit method
    for(int i = 0; i < 8; i++)
    {
        crc_highest_bit = (crc_buffer >> 7) & 1;

        if(crc_accumulator & 0x8000){
            crc_accumulator = ((crc_accumulator ^ crc_poly) << 1) + (crc_highest_bit ^ 1);
        }
        else{
            crc_accumulator = (crc_accumulator  << 1) + crc_highest_bit;
        }

        crc_accumulator &= 0x0fffff;
        crc_buffer <<= 1;
    }
}

void DPRALTE060B080::getCRCArray()
{
    // getting CRC based on provided velocity
    DPRALTE060B080::resetCRC();

    DPRALTE060B080::getVelocityArray();

    for(int i = 0; i < 4; i++){
        DPRALTE060B080::computeCRC(VelocityArray[i]);
    }
    DPRALTE060B080::computeCRC(0);
    DPRALTE060B080::computeCRC(0);

    // storing CRC into array form
    for(int j = 0; j < 2; j++){
        CRCArray[1-j] = (crc_accumulator >> (8*j)) & 0xff;
    }
}

//============================================================================= Part 4
void DPRALTE060B080::converttoOdometry()
{
    odometry = 0;

    for(int i = 0; i < 4; i++){
        OdometryArray[i] = OdoBuffer[8+i];

        Odo_ss.str(std::string());
        Odo_ss.clear();
        Odo_ss << std::hex << (OdometryArray[i] << (8*i));
        Odo_ss >> temp;

        odometry += temp;
    }
}

//============================================================================= Part 5
void DPRALTE060B080::setWriteToDriver(
        // header section
        int sof, int address, int controlbyte, int index, int offset, int header_datawords,
        unsigned int headercrc_0, unsigned int headercrc_1,
        // data section
        int datafield_0, int datafield_1, 
        unsigned int datacrc_0, unsigned int datacrc_1)
{
    WriteToDriver[0]  = sof;
    WriteToDriver[1]  = address;
    WriteToDriver[2]  = controlbyte;
    WriteToDriver[3]  = index;
    WriteToDriver[4]  = offset;
    WriteToDriver[5]  = header_datawords;
    WriteToDriver[6]  = headercrc_0;
    WriteToDriver[7]  = headercrc_1;

    WriteToDriver[8]  = datafield_0;
    WriteToDriver[9]  = datafield_1;
    WriteToDriver[10] = datacrc_0;
    WriteToDriver[11] = datacrc_1;
}

void DPRALTE060B080::setVelocityTarget(
        // header section
        int sof, int address, int controlbyte, int index, int offset, int header_datawords,
        unsigned int headercrc_0, unsigned int headercrc_1,
        // data section
        int datafield_0, int datafield_1, int datafield_2, int datafield_3, 
        unsigned int datacrc_0, unsigned int datacrc_1)
{
    VelocityTarget[0]  = sof;
    VelocityTarget[1]  = address;
    VelocityTarget[2]  = controlbyte;
    VelocityTarget[3]  = index;
    VelocityTarget[4]  = offset;
    VelocityTarget[5]  = header_datawords;
    VelocityTarget[6]  = headercrc_0;
    VelocityTarget[7]  = headercrc_1;
    
    VelocityTarget[8]  = datafield_0;
    VelocityTarget[9]  = datafield_1;
    VelocityTarget[10] = datafield_2;
    VelocityTarget[11] = datafield_3;
    VelocityTarget[12] = datacrc_0;
    VelocityTarget[13] = datacrc_1;
}

void DPRALTE060B080::setReadFromDriver(
        // header section
        int sof, int address, int controlbyte, int index, int offset, int header_datawords,
        unsigned int headercrc_0, unsigned int headercrc_1)
{
    ReadFromDriver[0]  = sof;
    ReadFromDriver[1]  = address;
    ReadFromDriver[2]  = controlbyte;
    ReadFromDriver[3]  = index;
    ReadFromDriver[4]  = offset;
    ReadFromDriver[5]  = header_datawords;
    ReadFromDriver[6]  = headercrc_0;
    ReadFromDriver[7]  = headercrc_1;
}

//============================================================================= Part 6
void DPRALTE060B080::setupSerialCommunication()
{
    serial::Timeout to = serial::Timeout::simpleTimeout(1000);

    LeftPort.setPort(LEFT_PORT);
    LeftPort.setBaudrate(115200);
    LeftPort.setTimeout(to);

    RightPort.setPort(RIGHT_PORT);
    RightPort.setBaudrate(115200);
    RightPort.setTimeout(to);

    LeftPort.open();
    RightPort.open();
}

// void DPRALTE060B080::setupMsgMonitoring()
// {
//     Pub = nh.advertise<dpralte060b080::DPRALTE060B080_Msg>("MotorMsg", 100);
//     Sub = nh.subscribe("MotorMsg", 100, &DPRALTE060B080::CallBack, this);
//     Pub.publish(Msg);
//     ROS_INFO_STREAM("Message Monitoring has been set up.");
// }
// 
// void DPRALTE060B080::CallBack(const dpralte060b080::DPRALTE060B080_Msg::ConstPtr &Msg)
// {
//     ROS_INFO("left velocity is: %d", Msg->velocity_left);
//     ROS_INFO("right velocity is: %d", Msg->velocity_right);
//     ROS_INFO("left odometry is: %d", Msg->odometry_left);
//     ROS_INFO("right odometry is: %d", Msg->odometry_right);
// }
