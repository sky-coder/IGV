// ros includes
#include <ros/ros.h>
#include "dpralte060b080/DPRALTE060B080_Msg.h"

// cpp includes
#include <iostream>
#include "dpralte060b080/DPRALTE060B080.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>

void setVel()
{

}

int main(int argc, char **argv)
{

//   set_conio_terminal_mode();
 
  
/*    std::cout << "\
Control Your Turtlebot!\n\
---------------------------\n\
Moving around:\n\
   u    speed    o\n\
   j    k    l\n\
   m    ,    .\n\n\
q/z : increase/decrease max speeds by 10%\n\
w/x : increase/decrease only linear speed by 10%\n\
e/c : increase/decrease only angular speed by 10%\n\
space key, k : force stop\n\
anything else : stop smoothly\n\n\
CTRL-C to quit\n\
Input stream: ";
*/
  
std::cout << "\
Control IGV\n\
---------------\n\
Moving around:\n\
        i     \n\
   j    k    l\n\n\
q/z : increase/decrease max speeds by 10%\n\
\
. to quit\n\
Input stream: ";

    ros::init(argc, argv, "igv_DPRALTE060B080_Node");

    DPRALTE060B080 IGV;
    dpralte060b080::DPRALTE060B080_Msg Msg;

    ros::start();
    ros::Rate loop_rate(10);

    int speed = 30;

        IGV.getWriteAccess_Left();
        IGV.getWriteAccess_Right();
 //       ROS_INFO_STREAM("Write Access Obtained.");


	int c;
  	/* use system call to make terminal send all keystrokes directly to stdin */
  	system ("/bin/stty raw");
  	while((c=getchar())!= '.') {
    	/* type a period to break out of the loop, since CTRL-D won't work raw */
   	//putchar(c);
  	int flag = 0;
	// speed = 105, j = 106, k = 107, l = 108
	// Forward
	switch(c)
	{
	case 'i':
        Msg.velocity_left  =  1 * speed;
        Msg.velocity_right =  1 * speed;
	setVel();
	break;
	
	case 'k':
	Msg.velocity_left  =  -1 * speed;
        Msg.velocity_right =  -1 * speed;
	setVel();
	break;

	case 'l':
        Msg.velocity_left  =  1.5 * speed;
        Msg.velocity_right =  0.5 * speed;
	setVel();
	break;

	case 'j':
        Msg.velocity_left  = -1 * speed;
        Msg.velocity_right =  1.5 * speed;
	setVel();
	break;

	case 'q':
	speed += speed*0.1;
	flag++;
	break;

	case 'z':
	speed -= speed*0.1;
	flag++;
	break;

        default :
        Msg.velocity_left = 0;
        Msg.velocity_right = 0;
        }

//        std::cout << "\nLeft velocity is: " << Msg.velocity_left;
//        std::cout << "\nRight velocity is: " << Msg.velocity_right;

        if(!flag){
        IGV.setVelocity_Left(Msg.velocity_left);
        IGV.setVelocity_Right(Msg.velocity_right);
  //      ROS_INFO_STREAM("Velocity Set.");

        IGV.enableBridge_Left();
        IGV.enableBridge_Right();
  //      ROS_INFO_STREAM("Bridge Enabled.");
        ros::Duration(2).sleep();

	IGV.disableBridge_Left();
        IGV.disableBridge_Right();
  //      ROS_INFO_STREAM("Bridge Disabled.");

        Msg.odometry_left = IGV.getOdometry_Left();
        Msg.odometry_right = IGV.getOdometry_Right();
	}
//        std::cout << "Left odometry is: " << Msg.odometry_left << std::endl;
//        std::cout << "Right odometry is: " << Msg.odometry_right << std::endl;

        }

	ros::spinOnce();
        loop_rate.sleep();

  	/* use system call to set terminal behaviour to more normal behaviour */
  	system ("/bin/stty cooked");
 
ros::shutdown();

    return 0;
}
