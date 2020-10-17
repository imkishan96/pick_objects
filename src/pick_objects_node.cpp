#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

// Define a client for to send goal requests to the move_base server through a SimpleActionClient
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
bool at_pickup_location = false;
bool at_dropoff_location = false;
bool picked_up = false;
bool dropped_off =false;

int main(int argc, char** argv){
  // Initialize the simple_navigation_goals node
  ros::init(argc, argv, "pick_objects");

  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  // Wait 5 sec for move_base action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal goal;

  //seting rosparam to communicate with add_markers node
  ros::param::set("/at_pickup_location",at_pickup_location);
  ros::param::set("/at_dropoff_location",at_dropoff_location);

  // set up the frame parameters
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();

  // Define a position and orientation for the robot to reach
  goal.target_pose.pose.position.x = 1.0;
  goal.target_pose.pose.position.y = -2.0;
  goal.target_pose.pose.orientation.w = 1.0;

   // Send the goal position and orientation for the robot to reach
  ros::Duration(5).sleep();
  ROS_INFO("Sending pickup location");
  ac.sendGoal(goal);

  // Wait an infinite time for the results
  ac.waitForResult();

  // Check if the robot reached its goal
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
    ROS_INFO("Reached pickup location");
    at_pickup_location = true;
    ros::param::set("/at_pickup_location",at_pickup_location);
  }
  else{
    ROS_INFO("The Robot failed to reach pickup location"); 
  }

  ROS_INFO("picking up ...");
  ros::Duration(5).sleep();
  ROS_INFO("picked up ...");

  goal.target_pose.pose.position.x = -3.0;
  goal.target_pose.pose.position.y = -4.0;
  goal.target_pose.pose.orientation.w = 1.0;

  ROS_INFO("moving to drop off location");
  ac.sendGoal(goal);
  ac.waitForResult();

  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
  {
    ROS_INFO("Robot reached drop off location");
    at_dropoff_location =true;
    ros::param::set("/at_dropoff_location",at_dropoff_location);
  }
  else
    ROS_INFO("The Robot failed to reach drop off location");

  ROS_INFO("Dropped off");  

  return 0;
}