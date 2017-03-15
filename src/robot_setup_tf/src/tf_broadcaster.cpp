#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
//#include "os5000/OS5000.h"
#include <geometry_msgs/Twist.h>
#define PI 3.14159265359
#define FC PI/180

void poseCallback(const geometry_msgs::Twist::ConstPtr& Compass){
   static tf::TransformBroadcaster broadcaster;
   tf::Transform CompassTransform;
   tf::Transform LaserTransform;

   CompassTransform.setOrigin(tf::Vector3(0.0,0.0,0.25));

   tf::Quaternion CompassQuaternion;
   tf::Quaternion LaserQuaternion;
   
//This section is commented as we are already broadcasting this /tf branh in dpralte060b080 
   //CompassQuaternion.setRPY((Compass->angular.x*FC),(Compass->angular.y*FC),(Compass->angular.z*FC)+360);
   //CompassTransform.setRotation(CompassQuaternion);
   //broadcaster.sendTransform(tf::StampedTransform(CompassTransform, ros::Time::now(), "odom", "base_link"));
   
   
   LaserTransform.setOrigin(tf::Vector3(0.0,0.0,0.10));
   LaserQuaternion.setRPY(0,0,0);
   LaserTransform.setRotation(CompassQuaternion);
   broadcaster.sendTransform(tf::StampedTransform(LaserTransform, ros::Time::now(), "base_link", "laser"));

}


int main(int argc, char** argv){
    ros::init(argc,argv, "robot_tf_publisher");
    ros::NodeHandle n;

    ros::Rate r(100);

    tf::TransformBroadcaster broadcaster;
//    OS5000 Compass;

    ros::NodeHandle node;
    ros::Subscriber sub = node.subscribe("odom",10,&poseCallback);
    

    ros::spin();

    return 0;
//    while(n.ok()){
//        Compass.getOrientation();
//        broadcaster.sendTransform(
//            tf::StampedTransform(
//               tf::Transform(tf::Vector3(0,0,0), tf::Vector3(Compass.getYaw(), Compass.getPitch(), Compass.getRoll())), ros::Time::now(), "base_link","laser"));

//        broadcaster.sendTransform(
//            tf::StampedTransform(
//                tf::Transform(tf::Quaternion(0,0,0,1), tf::Vector3(0.0, 0.0, 0.0)), ros::Time::now(), "base_link","odom"));

//        r.sleep();
//    }
};
