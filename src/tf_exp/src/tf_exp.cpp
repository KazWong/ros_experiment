#include <ros/ros.h>
#include <tf/tf.h>
#include <std_msgs/String.h>
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>
#include <iostream>
#include <math.h>
#include "description/StringCmd.h"
#include "simple_trajectory/simple_trajectory_abs.h"
using namespace std;
using namespace tf;
using namespace ros;

typedef geometry_msgs::Twist s_Pose;
typedef geometry_msgs::Twist s_Twist;


const double wait_tf = 0.015; //12Hz max 0.042 each in WaitForTranform
const double listener_buf = 0.7;

string parking_no;
string ref_frame;

NodeHandle *np;
s_Twist zero_twist;
tf::TransformListener *nlistener;

//D control var
Time last_time;
s_Twist last_err;

bool park(string const &park_cmd, string const &parking_no, string const &ref_frame);

/** Pose Functions **/
void setSPose(double const x, double const y, double const th, s_Pose &pose) {
  pose.linear.x = x;
  pose.linear.y = y;
  pose.angular.z = th;
}


void setSTwist(double const vx, double const vy, double const omega_z, s_Twist &v) {
  v.linear.x = vx;
  v.linear.y = vy;
  v.angular.z = omega_z;
}

void convertTFtoPose(const tf::Pose pose, s_Pose &pose_diff)
{
  double x_pose = pose.getOrigin().getX();
  double y_pose = pose.getOrigin().getY();
  double yaw_angle = tf::getYaw(pose.getRotation());
   
  setSPose(x_pose, y_pose, yaw_angle, pose_diff);
}

/** Calculate distance from ARtag **/
bool LookforTransform(string const target_frame, string const source_frame, s_Pose &pose) {
 // tf::TransformListener listener;
  tf::StampedTransform transform;
  ros::Time t = ros::Time::now();
  t -= ros::Duration(0.03);
  try{
    nlistener->waitForTransform( target_frame, source_frame, t, ros::Duration(wait_tf));
    nlistener->lookupTransform( target_frame, source_frame, t, transform);
    //nlistener->lookupTransform( target_frame, Time(0), source_frame, Time(0), "map", transform);
  }
  catch (tf::TransformException ex){
    ROS_ERROR("%s",ex.what());
    return false;
  }

  cout << "tf Time " << transform.stamp_.sec << "." << transform.stamp_.nsec << endl;
  convertTFtoPose(transform, pose);
  return true;
}

bool FramesDistance(string const ref_frame, string const robo_frame, string const target_frame, s_Pose &curr_agv_pose, s_Pose &goal_pose,s_Pose &pose_diff) {
  ros::Time t = ros::Time::now();
  cout << "Timenow1 " << t.sec << "." << t.nsec << endl;
  if (!LookforTransform(ref_frame, robo_frame, curr_agv_pose))
    return false;
  t = ros::Time::now();
  cout << "Timenow2 " << t.sec << "." << t.nsec << endl;
  if (!LookforTransform(ref_frame, target_frame, goal_pose))
    return false;
  cout << "agv_x = " << curr_agv_pose.linear.x << endl;
  cout << "agv_y = " << curr_agv_pose.linear.y << endl;
  cout << "agv_omega = " << curr_agv_pose.angular.z << endl;
  cout << robo_frame << endl;
  cout << "goal_x = " << goal_pose.linear.x << endl;
  cout << "goal_y = " << goal_pose.linear.y << endl;
  cout << "goal_omega = " << goal_pose.angular.z << endl;

  setSPose(curr_agv_pose.linear.x - goal_pose.linear.x, 
            curr_agv_pose.linear.y - goal_pose.linear.y, 
            curr_agv_pose.angular.z - goal_pose.angular.z/* + 3.14*/, pose_diff);
  cout << "dx = " << pose_diff.linear.x << endl;
  cout << "dy = " << pose_diff.linear.y << endl;
  cout << "domega = " << pose_diff.angular.z << endl;
  cout << endl;

  return true;
}

string doubletoString(double num){
     ostringstream os;
     os << num;
     return os.str();
}

bool checkReadyToPark(string const parking_no, string const ref_frame)
{
  s_Pose agv_pose_map, goal_pose_map, pose_diff_map;
 
  bool got_map_pose = false;
  int dir = -1; 
  int count = 0;

    while (count < 10 && ros::ok() ) {
   
      if (FramesDistance(ref_frame, "ur_base", parking_no, agv_pose_map, goal_pose_map, pose_diff_map)) {
      ROS_INFO("AR tag observed!");
      return true;
    }

    ros::Duration(3).sleep();//default:0.2   
    
    count++;
  }  
  return false;
}

bool park()
{
  Time last_time = Time::now();
  //s_Pose curr_agv_pose, goal_pose;
  //s_Pose pose_diff;
  s_Twist v_bc;

  if (!checkReadyToPark(parking_no, ref_frame)) { 
    ROS_INFO("No ARTag, Exit parking");
    return false;
  }

//init K controller var
  //last_time = Time::now();
 // ROS_INFO("Before check base_footprint");
  //FramesDistance(ref_frame, "ur_base", parking_no, curr_agv_pose, goal_pose, pose_diff);
//  ROS_INFO("After check base_footprint");
  //setSTwist(pose_diff.linear.x, pose_diff.linear.y, pose_diff.angular.z, last_err);
  //setSTwist(0, 0, 0, last_err);
 

 // ROS_INFO("1st base_footprint");
  ros::Rate rate(12.0);
  while (ros::ok()) {
    s_Pose curr_agv_pose, goal_pose;
    s_Pose pose_diff;
    bool got_AR_Tf = false;
    
    setSPose(0.0, 0.0, 0.0, pose_diff);
    setSPose(0.0, 0.0, 0.0, goal_pose);
    setSPose(0.0, 0.0, 0.0, curr_agv_pose);

    double dt = (Time::now() - last_time).toSec();
    last_time = Time::now();
    cout << "dt = " << dt << endl;
    got_AR_Tf =  FramesDistance(ref_frame, "ur_base", parking_no, curr_agv_pose, goal_pose, pose_diff);

    spinOnce();
    rate.sleep();
  }
  return true;
}

//ros::Subscriber tool_vel_sub1;
int main(int argc, char **argv) {
  parking_no = argv[1];//"ar_parked_HT";
  ref_frame = argv[2];//"shoulder_link";
  
  init(argc, argv, "tf_exp");
  NodeHandle n;
  np = &n;
  nlistener = new tf::TransformListener(ros::Duration(listener_buf));

  setSTwist(0.0, 0.0, 0.0, zero_twist);
  park();

  return 0;
}

