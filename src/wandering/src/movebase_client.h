#include <ros/ros.h>
#include <tf/tf.h>
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <actionlib_tutorials/FibonacciAction.h>
#include <move_base_msgs/MoveBaseAction.h>

using namespace std;
using namespace ros;

class MB {
private:
  typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
  
  MoveBaseClient ac;
  ros::ServiceClient stop_client;
  tf::TransformListener listener_nav_goal_pose;
  tf::StampedTransform transform_nav_goal_pose;    
  
public:
  MB() : ac("move_base", true), listener_nav_goal_pose(ros::Duration(1.5)) {
    while( !ac.waitForServer(ros::Duration(2.0)) && ros::ok() )
      ROS_INFO("Waiting for the move_base action server to come up");  
  }
  
  bool goToPoint(string const nav_goal) {    
    try {
      //listener_nav_goal_pose.waitForTransform( "map", nav_goal, ros::Time(0), ros::Duration(0.5) );
      listener_nav_goal_pose.lookupTransform( "map", nav_goal, ros::Time(0), transform_nav_goal_pose);
    } catch (tf::TransformException ex) {
      ROS_ERROR("%s",ex.what());
      return false;
    }
    
    move_base_msgs::MoveBaseGoal goal;
    actionlib::SimpleClientGoalState state = actionlib::SimpleClientGoalState::LOST;

    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();
	
	  goal.target_pose.pose.position.x = transform_nav_goal_pose.getOrigin().getX();
    goal.target_pose.pose.position.y = transform_nav_goal_pose.getOrigin().getY();
    goal.target_pose.pose.position.z = transform_nav_goal_pose.getOrigin().getZ();
    goal.target_pose.pose.orientation.x = transform_nav_goal_pose.getRotation().x(); 
    goal.target_pose.pose.orientation.y = transform_nav_goal_pose.getRotation().y(); 
    goal.target_pose.pose.orientation.z = transform_nav_goal_pose.getRotation().z(); 
    goal.target_pose.pose.orientation.w = transform_nav_goal_pose.getRotation().w(); 
    ac.sendGoal(goal);
    
    do {
      state = ac.getState();
      if(state == actionlib::SimpleClientGoalState::SUCCEEDED)
        return true;
      ac.waitForResult(ros::Duration(10.0));
    } while ( ros::ok() && ((state == actionlib::SimpleClientGoalState::PENDING) || 
              (state == actionlib::SimpleClientGoalState::ACTIVE)) );
    
    return false;
  }
};
