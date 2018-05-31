#ifndef _MOVEBASE_
#define _MOVEBASE_

#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <string>
#include <std_msgs/String.h>
#include <ros/ros.h>
#include <vector>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <tf/tf.h>
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>

using namespace std;

namespace movebase {
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> Move_Base_Client;

struct s_Pose {
  double x, y, th;
};

class Movebase{

private:
     ros::NodeHandle _nh;
     ros::ServiceClient stop_client;
     tf::TransformListener listener_nav_goal_pose;
     tf::StampedTransform transform_nav_goal_pose;    
     tf::TransformListener listener_ur_pose;
     tf::StampedTransform transform_ur_pose;

private:
     bool goToPoint(string const nav_goal, Move_Base_Client &ac);  
     void convertTFtoPose(const tf::Pose pose, s_Pose &pose_diff);
     void setSPose(double const x, double const y, double const th, s_Pose *pose);
   
public:
     Movebase();
     ~Movebase();

     int navigation_action(Move_Base_Client &ac, string const cmd_arg);
     void get_cur_agv_pose(s_Pose &curr_agv_pose);
     int move_stop();
     int move_reset();
 
};

};//namespace

#endif
