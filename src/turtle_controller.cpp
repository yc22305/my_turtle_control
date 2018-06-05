//(Publisher) Program to control the turtle on turtlesim
#include "ros/ros.h" // ROS Default Header File
#include "turtlesim/Pose.h"
#include "geometry_msgs/Twist.h"
#include <cstdlib> // Library for using the "atoll" function
#include <cmath>
#include <unistd.h>

#define PI 3.14159265

turtlesim::Pose msg_in;
geometry_msgs::Twist msg_out_position;
geometry_msgs::Twist msg_out_orientation;

void msgCallback(const turtlesim::Pose::ConstPtr&);

int main(int argc, char **argv) // Node Main Function
{
	ros::init(argc, argv, "turtle_controller"); // Initializes Node Name
	ros::NodeHandle nh; // Node handler

	// Declares publisher name as 'turtle_controller'
	// The topic name is 'turtleX/cmd_vel'
	ros::Publisher turtle_controller = nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 100);
	
	// Declares subsriber name as 'turtle_position'
	// The topic name is '/turtle1/pose'
	ros::Subscriber turtle_position = nh.subscribe("/turtle1/pose", 100, msgCallback);

	float goal_x; // to store the user input
	float goal_y; // to store the user input

	ros::Rate loop_rate(1); // less than turtlesim_node
	while (ros::ok())
	{
		ROS_INFO("enter the x-position you want:");	
		std::cin >> goal_x;
		ROS_INFO("enter the y-position you want:");	
		std::cin >> goal_y;

		ros::spinOnce();

		msg_out_orientation.linear.x = 0;
		msg_out_orientation.linear.y = 0;
		msg_out_orientation.linear.z = 0;
		msg_out_orientation.angular.x = 0;
		msg_out_orientation.angular.y = 0;
		msg_out_orientation.angular.z = atan2(goal_y-msg_in.y, goal_x-msg_in.x);
		if (msg_out_orientation.angular.z < 0)
			msg_out_orientation.angular.z += 2*PI;
		msg_out_orientation.angular.z -= msg_in.theta;
		turtle_controller.publish(msg_out_orientation);

		sleep(1);

		msg_out_position.linear.x = sqrt(pow(goal_y-msg_in.y, 2) + pow(goal_x-msg_in.x, 2));
		msg_out_position.linear.y = 0;
		msg_out_position.linear.z = 0;
		msg_out_position.angular.x = 0;
		msg_out_position.angular.y = 0;
		msg_out_position.angular.z = 0;
		turtle_controller.publish(msg_out_position);

		loop_rate.sleep(); // Goes to sleep according to the loop rate defined above.
	}
	return 0;
}

void msgCallback(const turtlesim::Pose::ConstPtr& msg)
{
	msg_in.x = msg->x;
	msg_in.y = msg->y;
	msg_in.theta = msg->theta;
}

