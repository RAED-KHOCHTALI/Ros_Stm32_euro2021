#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/PoseStamped.h"
#include "std_msgs/Float64.h"
#include "sim_euro2021/goal.h"
#include "nav_msgs/Odometry.h"
#include "tf/transform_broadcaster.h"
#include "reel_euro2021/ta7wil.h"


#include <sstream>





class Subscriber1publisher
{
   public: 
  Subscriber1publisher(){
      
        odom_pub = n.advertise<nav_msgs::Odometry>("/odom", 10000000);
        sub_ta7wil = n.subscribe("/ta7wil", 10000000, &Subscriber1publisher::ta7wil_odom_tf ,this);
  }

 

void ta7wil_odom_tf(const reel_euro2021::ta7wil msg)
{   
  

      transformStamped.header.frame_id ="odom";
	  transformStamped.child_frame_id = "base_link" ;
	  transformStamped.transform.translation.x = msg.x;
	  transformStamped.transform.translation.y = msg.y;
	  transformStamped.transform.rotation = tf::createQuaternionMsgFromYaw(msg.teta);
	  transformStamped.header.stamp = ros::Time::now();
	  broadcaster.sendTransform(transformStamped);


  odom.pose.pose.position.x=msg.x;
  odom.pose.pose.position.y=msg.y;
  odom.pose.pose.position.z=0.0;
    




	  odom.header.stamp = ros::Time::now();
	  odom.header.frame_id= "odom";
	  odom.pose.pose.orientation = tf::createQuaternionMsgFromYaw(msg.teta);


	  //set the velocity
	  odom.child_frame_id = "base_link";
	  odom.twist.twist.linear.x = msg.vx;
	  odom.twist.twist.linear.y = 0;
	  odom.twist.twist.angular.z = msg.vy;

	   //publish the message
	  odom_pub.publish(odom);

    }
    
  private:
  ros::NodeHandle n;
   ros::Publisher odom_pub;
   ros::Subscriber sub_ta7wil ;
   geometry_msgs::TransformStamped transformStamped;
    tf::TransformBroadcaster broadcaster;
    nav_msgs::Odometry odom;

};
       


int main(int argc, char **argv)
{  
  ros::init(argc, argv, "ta7wil_node");
  Subscriber1publisher sub_instance;
  ros::spin();
  return 0;
}
