//This program provides capabilities to teleop the IGV using
// a keyboard.
// Publishes the distance and velocity data as a topic
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

int main(int argc, char **argv)
{

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


        Msg.distance_left = 0;
        Msg.distance_right = 0;
        Msg.velocity_left = 0;
        Msg.velocity_right = 0;
        //std::cout<<Msg.distance_left<<" "<<Msg.distance_right<<std::endl;
        //wheel_pub.publish(Msg);

	int c;
  	/* use system call to make terminal send all keystrokes directly to stdin */
  	system ("/bin/stty raw");
        
  	while((c=getchar())!= '.') {
    	/* type a period to break out of the loop, since CTRL-D won't work raw */
        int flag = 0; //a check

	switch(c)
	{
	case 'i':
        Msg.velocity_left  =  1 * speed;
        Msg.velocity_right =  1 * speed;
	break;
	
	case 'k':
	Msg.velocity_left  =  -1 * speed;
        Msg.velocity_right =  -1 * speed;
	break;

	case 'l':
        Msg.velocity_left  =  1.5 * speed;
        Msg.velocity_right =  0.5 * speed;
	break;

	case 'j':
        Msg.velocity_left  = -1 * speed;
        Msg.velocity_right =  1.5 * speed;
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
        //ROS_INFO_STREAM("Velocity Set.");

        IGV.enableBridge_Left();
        IGV.enableBridge_Right();
  //      ROS_INFO_STREAM("Bridge Enabled.");
        // The duration determines the distance of travel
        ros::Duration(3).sleep();

	IGV.disableBridge_Left();
        IGV.disableBridge_Right();
  //      ROS_INFO_STREAM("Bridge Disabled.");

        Msg.distance_left = IGV.getDistance_Left();
        Msg.distance_right = IGV.getDistance_Right();
	
        //std::cout << "Left odometry is: " << Msg.distance_left << std::endl;
        //std::cout << "Right odometry is: " << Msg.distance_right << std::endl;
        //wheel_pub.publish(Msg);
        }
        
        }

	ros::spinOnce();
        loop_rate.sleep();

  	/* use system call to set terminal behaviour to more normal behaviour */
  	system ("/bin/stty cooked");
 
ros::shutdown();

    return 0;
}
