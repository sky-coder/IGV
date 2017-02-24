// cpp includes
#include "dpralte060b080/DPRALTE060B080.h"

DPRALTE060B080::DPRALTE060B080()
    :home_left(0), home_right(0)
{
    VelocityArray = new int[4];
    CRCArray = new unsigned int[2];
    DistanceArray = new uint8_t[4];

    DPRALTE060B080::setupSerialCommunication();

    // DPRALTE060B080::getReadAccess_Left();
    // DPRALTE060B080::getReadAccess_Right();
    home_left = DPRALTE060B080::getDistance_Left();
    home_right = DPRALTE060B080::getDistance_Right();
    // std::cout << "home of left is: " << home_left << std::endl;
    // std::cout << "home of right is: " << home_right << std::endl;
}

DPRALTE060B080::~DPRALTE060B080()
{
    if(VelocityArray){
        delete[] VelocityArray;
    }
    if(CRCArray){
        delete[] CRCArray;
    }
    if(DistanceArray){
        delete[] DistanceArray;
    }

    if(TxBuffer){
        delete[] TxBuffer;
    }
    if(RxBuffer){
        delete[] RxBuffer;
    }
}

//============================================================================= Part 1
//------------------------------------------------------------------------ Left
void DPRALTE060B080::getWriteAccess_Left()
{
    TxBuffer = new uint8_t[12];
    RxBuffer = new uint8_t[8];

    DPRALTE060B080::setTxBuffer(0xa5, 0x3e, 0x02, 0x07, 0x00, 0x01, 0x19, 0xb6,
                                0xff, 0x00, 0x03, 0xff);
    LeftPort.write(TxBuffer, 12);
    LeftPort.read(RxBuffer, 8);

    delete[] TxBuffer;
    delete[] RxBuffer;
}

void DPRALTE060B080::enableBridge_Left()
{
    TxBuffer = new uint8_t[12];
    RxBuffer = new uint8_t[8];

    DPRALTE060B080::setTxBuffer(0xa5, 0x3e, 0x02, 0x01, 0x00, 0x01, 0xab, 0x16,
                                0x00, 0x00, 0x00, 0x00);
    LeftPort.write(TxBuffer, 12);
    LeftPort.read(RxBuffer, 8);

    delete[] TxBuffer;
    delete[] RxBuffer;
}

void DPRALTE060B080::disableBridge_Left()
{
    TxBuffer = new uint8_t[12];
    RxBuffer = new uint8_t[8];

    DPRALTE060B080::setTxBuffer(0xa5, 0x3e, 0x02, 0x01, 0x00, 0x01, 0xab, 0x16,
                                0x01, 0x00, 0x33, 0x31);
    LeftPort.write(TxBuffer, 12);
    LeftPort.read(RxBuffer, 8);

    delete[] TxBuffer;
    delete[] RxBuffer;
}

void DPRALTE060B080::setVelocity_Left(int vel_in)
{
    velocity = vel_in;
    DPRALTE060B080::getCRCArray();
    TxBuffer = new uint8_t[14];
    RxBuffer = new uint8_t[8];

    DPRALTE060B080::setTxBuffer(0xa5, 0x3e, 0x02, 0x45, 0x00, 0x02, 0x5a, 0x18,
                                VelocityArray[0],
                                VelocityArray[1],
                                VelocityArray[2],
                                VelocityArray[3],
                                CRCArray[0],
                                CRCArray[1]);
    LeftPort.write(TxBuffer, 14);
    LeftPort.read(RxBuffer, 8);

    delete[] TxBuffer;
    delete[] RxBuffer;
}

// void DPRALTE060B080::getReadAccess_Left()
// {
//     TxBuffer = new uint8_t[8];
//     RxBuffer = new uint8_t[14];
//
//     DPRALTE060B080::setTxBuffer(0xa5, 0x3e, 0x01, 0x07, 0x00, 0x01, 0x82, 0x6a);
//     LeftPort.write(TxBuffer, 8);
//     LeftPort.read(RxBuffer, 14);
// }

int DPRALTE060B080::getDistance_Left()
{
    TxBuffer = new uint8_t[8];
    RxBuffer = new uint8_t[14];

    DPRALTE060B080::setTxBuffer(0xa5, 0x3e, 0x01, 0x12, 0x00, 0x02, 0x1a, 0x9a);
    LeftPort.write(TxBuffer, 8);
    LeftPort.read(RxBuffer, 14);

    // std::cout << "The Odo Buffer is: " << std::endl;
    // for(int i = 0; i < ODO_BUF_LENGTH; i++){
    //     std::cout << std::hex << signed(OdoBuffer[i]) << " ";
    // }
    // std::cout << std::endl;

    DPRALTE060B080::converttoDistance();

    delete[] TxBuffer;
    delete[] RxBuffer;

    return distance - home_left;
}
//----------------------------------------------------------------------- Right
void DPRALTE060B080::getWriteAccess_Right()
{
    TxBuffer = new uint8_t[12];
    RxBuffer = new uint8_t[8];

    DPRALTE060B080::setTxBuffer(0xa5, 0x3f, 0x02, 0x07, 0x00, 0x01, 0xb3, 0xe7,
                                0xff, 0x00, 0x03, 0xff);
    RightPort.write(TxBuffer, 12);
    RightPort.read(RxBuffer, 8);

    delete[] TxBuffer;
    delete[] RxBuffer;
}

void DPRALTE060B080::enableBridge_Right()
{
    TxBuffer = new uint8_t[12];
    RxBuffer = new uint8_t[8];

    DPRALTE060B080::setTxBuffer(0xa5, 0x3f, 0x02, 0x01, 0x00, 0x01, 0x01, 0x47,
                                0x00, 0x00, 0x00, 0x00);
    RightPort.write(TxBuffer, 12);
    RightPort.read(RxBuffer, 8);

    delete[] TxBuffer;
    delete[] RxBuffer;
}

void DPRALTE060B080::disableBridge_Right()
{
    TxBuffer = new uint8_t[12];
    RxBuffer = new uint8_t[8];

    DPRALTE060B080::setTxBuffer(0xa5, 0x3f, 0x02, 0x01, 0x00, 0x01, 0x01, 0x47,
                                0x01, 0x00, 0x33, 0x31);
    RightPort.write(TxBuffer, 12);
    RightPort.read(RxBuffer, 8);

    delete[] TxBuffer;
    delete[] RxBuffer;
}

void DPRALTE060B080::setVelocity_Right(int vel_in)
{
    velocity = vel_in;
    DPRALTE060B080::getCRCArray();
    TxBuffer = new uint8_t[14];
    RxBuffer = new uint8_t[8];

    DPRALTE060B080::setTxBuffer(0xa5, 0x3f, 0x02, 0x45, 0x02, 0x02, 0x96, 0x2b,
                                VelocityArray[0],
                                VelocityArray[1],
                                VelocityArray[2],
                                VelocityArray[3],
                                CRCArray[0],
                                CRCArray[1]);
    RightPort.write(TxBuffer, 14);
    RightPort.read(RxBuffer, 8);

    delete[] TxBuffer;
    delete[] RxBuffer;
}

// void DPRALTE060B080::getReadAccess_Right()
// {
//     DPRALTE060B080::setTxBuffer(0xa5, 0x3f, 0x01, 0x07, 0x00, 0x01, 0x28, 0x3b);
//     RightPort.write(TxBuffer, 8);
//     RightPort.read(RxBuffer, 14);
// }

int DPRALTE060B080::getDistance_Right()
{
    TxBuffer = new uint8_t[8];
    RxBuffer = new uint8_t[14];

    DPRALTE060B080::setTxBuffer(0xa5, 0x3f, 0x01, 0x12, 0x00, 0x02, 0xb0, 0xcb);
    RightPort.write(TxBuffer, 8);
    RightPort.read(RxBuffer, 14);

    DPRALTE060B080::converttoDistance();

    delete[] TxBuffer;
    delete[] RxBuffer;

    return distance - home_right;
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
void DPRALTE060B080::convertToDistance()
{
    distance = 0;

    for(int i = 0; i < 4; i++){
        DistanceArray[i] = RxBuffer[8+i];

        Dis_ss.str(std::string());
        Dis_ss.clear();
        Dis_ss /*<< std::hex*/ << (DistanceArray[i] << (8*i));
        Dis_ss >> temp;
        // std::cout << "temp is: " << temp << std::endl;

        distance += temp;
    }
}

//============================================================================= Part 5
void DPRALTE060B080::setTxBuffer(
        // header section
        int sof, int address, int controlbyte, int index, int offset, int header_datawords,
        unsigned int headercrc_0, unsigned int headercrc_1,
        // data section
        int datafield_0, int datafield_1,
        unsigned int datacrc_0, unsigned int datacrc_1)
{
    TxBuffer[0]  = sof;
    TxBuffer[1]  = address;
    TxBuffer[2]  = controlbyte;
    TxBuffer[3]  = index;
    TxBuffer[4]  = offset;
    TxBuffer[5]  = header_datawords;
    TxBuffer[6]  = headercrc_0;
    TxBuffer[7]  = headercrc_1;

    TxBuffer[8]  = datafield_0;
    TxBuffer[9]  = datafield_1;
    TxBuffer[10] = datacrc_0;
    TxBuffer[11] = datacrc_1;
}

void DPRALTE060B080::setTxBuffer(
        // header section
        int sof, int address, int controlbyte, int index, int offset, int header_datawords,
        unsigned int headercrc_0, unsigned int headercrc_1,
        // data section
        int datafield_0, int datafield_1, int datafield_2, int datafield_3,
        unsigned int datacrc_0, unsigned int datacrc_1)
{
    TxBuffer[0]  = sof;
    TxBuffer[1]  = address;
    TxBuffer[2]  = controlbyte;
    TxBuffer[3]  = index;
    TxBuffer[4]  = offset;
    TxBuffer[5]  = header_datawords;
    TxBuffer[6]  = headercrc_0;
    TxBuffer[7]  = headercrc_1;

    TxBuffer[8]  = datafield_0;
    TxBuffer[9]  = datafield_1;
    TxBuffer[10] = datafield_2;
    TxBuffer[11] = datafield_3;
    TxBuffer[12] = datacrc_0;
    TxBuffer[13] = datacrc_1;
}

void DPRALTE060B080::setTxBuffer(
        // header section
        int sof, int address, int controlbyte, int index, int offset, int header_datawords,
        unsigned int headercrc_0, unsigned int headercrc_1)
{
    TxBuffer[0]  = sof;
    TxBuffer[1]  = address;
    TxBuffer[2]  = controlbyte;
    TxBuffer[3]  = index;
    TxBuffer[4]  = offset;
    TxBuffer[5]  = header_datawords;
    TxBuffer[6]  = headercrc_0;
    TxBuffer[7]  = headercrc_1;
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
