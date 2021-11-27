#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/TwistStamped.h"

#include <geometry_msgs/TransformStamped.h>
#include <tf/tf.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/Header.h>

double current_phi_rad =60 ; 

	

int main(int argc, char **argv)
{
		
	geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(current_phi_rad);
	printf("x = %f \n", odom_quat.x); 
	printf("y = %f \n", odom_quat.y); 
	printf("z = %f \n", odom_quat.z); 
	printf("w = %f \n", odom_quat.w); 

  return 0;
}
