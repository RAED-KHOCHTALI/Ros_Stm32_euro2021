#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/PoseStamped.h"
#include "std_msgs/Float64.h"
#include "sim_euro2021/goal.h"
#include "nav_msgs/Odometry.h"
#include <sstream>





class Subscriber1publisher
{
   public: 
  Subscriber1publisher(){
        goal_pub = n.advertise<geometry_msgs::PoseStamped>("move_base_simple/goal", 1000);
        odom_pub = n.advertise<sim_euro2021::goal>("better_odom", 1000);
        sub = n.subscribe("better_goal", 1000, &Subscriber1publisher::goal_Callback,this);
        sub_odom = n.subscribe("odom", 1000, &Subscriber1publisher::goal_Callback_odom,this);
  }

  void goal_Callback(const sim_euro2021::goal msg)
{   
    if(msg.orientation*M_PI/180 > M_PI || msg.orientation*M_PI/180 < -M_PI){
    ROS_INFO("Failed to send goal ===>  respect the angles limits 180 & -180");

    }else{
        std_msgs::Header header;        
    header.frame_id="odom";
    header.seq=0;
    
    pose.header=header;
    double qx,qy,qz,qw,yaw,pitch,roll;
    geometry_msgs::Pose pose2;
    pose2.position.x=msg.x;
    pose2.position.y=msg.y;    
    pose2.position.z=0;    
    pose2.orientation.x=qx;
    pose2.orientation.y=qy;
    pose2.orientation.z=qz;
    pose2.orientation.w=qw;

    // conversion
    
    roll=0;
    pitch=0;
    yaw=msg.orientation*M_PI/180;
    qx = sin(roll/2) * cos(pitch/2) * cos(yaw/2) - cos(roll/2) * sin(pitch/2) * sin(yaw/2);
    qy = cos(roll/2) * sin(pitch/2) * cos(yaw/2) + sin(roll/2) * cos(pitch/2) * sin(yaw/2);
    qz = cos(roll/2) * cos(pitch/2) * sin(yaw/2) - sin(roll/2) * sin(pitch/2) * cos(yaw/2);
    qw = cos(roll/2) * cos(pitch/2) * cos(yaw/2) + sin(roll/2) * sin(pitch/2) * sin(yaw/2);
    //end conversion
    pose2.orientation.x=qx;
    pose2.orientation.y=qy;
    pose2.orientation.z=qz;
    pose2.orientation.w=qw;

    pose.pose=pose2;
    goal_pub.publish(pose);
   ROS_INFO("New goal sent");
    }
   

}

void goal_Callback_odom(const nav_msgs::Odometry msg)
{   
  
  odom.x=msg.pose.pose.position.x;
  odom.y=msg.pose.pose.position.y;

   //conversion 
  double x,y,z,w,t3,t4,yaw;
  x=msg.pose.pose.orientation.x;
  y=msg.pose.pose.orientation.y;
  z=msg.pose.pose.orientation.z;
  w=msg.pose.pose.orientation.w;
    t3 = +2.0 * (w * z + x * y);
    t4 = +1.0 - 2.0 * (y * y + z * z);
    yaw=atan2(t3,t4);
    //end conversion
    yaw=yaw*180/M_PI;
  odom.orientation=yaw;
  odom_pub.publish(odom);

    }
    
  private:
  ros::NodeHandle n;
   ros::Publisher goal_pub;
   ros::Publisher odom_pub;
   ros::Subscriber sub ;
   ros::Subscriber sub_odom;
   geometry_msgs::PoseStamped pose;
   sim_euro2021::goal odom;

};
       


int main(int argc, char **argv)
{  
  ros::init(argc, argv, "goal_node");
  Subscriber1publisher sub_pub;
  ros::spin();
  return 0;
}
