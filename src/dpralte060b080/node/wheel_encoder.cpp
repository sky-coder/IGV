#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/Pose.h>
#include <tf/transform_broadcaster.h>

#include <iostream>
#include <math.h>
#include "dpralte060b080/DPRALTE060B080.h"

geometry_msgs::Quaternion odom_quat;

void compassCallback(const geometry_msgs::Pose::ConstPtr& compass)
{
 
    odom_quat = compass->orientation;
}

int main(int argc, char **argv)
{
    ros::init(argc,argv,"wheel_encoder_Node");

    DPRALTE060B080 IGV;
    //dprlate060b080::DPRLATE060B080_Msg Msg;

    ros::NodeHandle n;
    ros::NodeHandle c; //c - compass

    ros::start();
    ros::Rate loop_rate(10);

    //ros::Publisher wheel_pub = n.advertise<dpralte060b080::DPRALTE060B080_Msg>("wheel",10);
    ros::Publisher wheelOdom_pub = n.advertise<nav_msgs::Odometry>("wheelOdom",10);
    ros::Subscriber sub = c.subscribe("compass", 1000, compassCallback);

    int ticks_left = 0;
    int ticks_right = 0;
    int initial_flag = 1;
    double x=0.0;
    double y=0.0;
    double th=0.0;

    double length = 0.72;

    double last_time, current_time;
    double deltaLeft, deltaRight;
    double previousEncoderTick_left, previousEncoderTick_right;
    double v_left,v_right;
    double vx,vy,vth;

    while(ros::ok())
    {
        ros::spinOnce();
        ticks_left = IGV.getDistance_Left();
        ticks_right = IGV.getDistance_Right();

        current_time = ros::Time::now().toSec();
        nav_msgs::Odometry odom;
        odom.header.stamp = ros::Time::now();
        odom.header.frame_id = "odom";       

        if (initial_flag){
            previousEncoderTick_left = ticks_left;
            previousEncoderTick_right = ticks_right;
            last_time = current_time;
            initial_flag = 0;
        }
        //std::cout<<ticks_left<<" "<<ticks_right<<"\n";

        deltaLeft = ticks_left - previousEncoderTick_left;
        deltaRight = ticks_right - previousEncoderTick_right;

        v_left = deltaLeft*67/4000;
        v_right = deltaRight*67/4000;
        
        vx = (v_left + v_right)/2;
        vy = 0;
        vth = ((v_left - v_right)/length);

        double dt = (current_time - last_time);
        double delta_x = (vx*cos(th))*dt; 
        double delta_y = (vy*sin(th))*dt; 
        double delta_th = vth*dt;

        x += delta_x;
        y += delta_y;
        th += delta_th;
        //std::cout<<vx<<" "<<vth<<"\n";
        //geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(th);

        //Odometry
        //nav_msgs::Odometry odom;
        //odom.header.stamp = current_time;
        //odom.header.frame_id = "odom";

        //set the position
        odom.pose.pose.position.x = x;
        odom.pose.pose.position.y = y;
        odom.pose.pose.position.z = 0.0;
        odom.pose.pose.orientation = odom_quat;
        std::cout<<"x: "<<x<<" y: "<<y<<" o: "<<odom_quat<<"\n";
        //set the velocity
        odom.child_frame_id = "base_link";
        odom.twist.twist.linear.x = vx;
        odom.twist.twist.linear.y = vy;
        odom.twist.twist.linear.z = vth;
        
        //publish odom
        wheelOdom_pub.publish(odom);

        // Update current params to previous params
        previousEncoderTick_left = ticks_left;
        previousEncoderTick_right = ticks_right;
        last_time = current_time;
        
        

    loop_rate.sleep();
    }
    
    
    return 0;
}
