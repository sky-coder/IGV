#include "os5000/OS5000.h"

OS5000::OS5000()
{

}

OS5000::~OS5000()
{

}

//============================================================================= Part
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
