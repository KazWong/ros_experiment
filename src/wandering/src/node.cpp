#include <ros/ros.h>
#include "movebase_client.h"

using namespace std;
using namespace ros;

int main (int argc, char **argv) {
  ros::init(argc, argv, "navi");
  
  MB move;
  
  while (ros::ok()) {
    while ( !move.goToPoint("nav_goal_P_MMI_004") && ros::ok() )
      ROS_INFO("Retry nav_goal_P_MMI_004");
    sleep(1);
    while ( !move.goToPoint("nav_goal_N_MMI_014") && ros::ok() )
      ROS_INFO("Retry nav_goal_N_MMI_014");
    sleep(1);
    while ( !move.goToPoint("darling_in_store") && ros::ok() )
      ROS_INFO("Retry darling_in_store");
    sleep(1);
    while ( !move.goToPoint("darling_in_franxx") && ros::ok() )
      ROS_INFO("Retry darling_in_franxx");
    sleep(1);
    while ( !move.goToPoint("darling_in_washroom") && ros::ok() )
      ROS_INFO("Retry darling_in_washroom");
    sleep(1);
  
    ros::spinOnce();
  }
}
