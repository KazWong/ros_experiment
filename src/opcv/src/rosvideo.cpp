#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <sstream>                                                 // Added this


static const std::string OPENCV_WINDOW = "Image window";
int count = 0;

bool take_picture()
{
  boost::shared_ptr<sensor_msgs::Image const> sharedPtr;
  sharedPtr  = ros::topic::waitForMessage<sensor_msgs::Image>("/cam/image_raw", ros::Duration(10));

  if (sharedPtr == NULL)
  {
     ROS_INFO("No image received");
  }
  else
  {
    cv_bridge::CvImagePtr cv_ptr;
    ROS_INFO("New image captured!");
    try
    {
      cv_ptr = cv_bridge::toCvCopy(sharedPtr, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return -1;
    }
    //ROS_INFO("Showing new image.");
    //cv::imshow(OPENCV_WINDOW, cv_ptr->image);
    //cv::waitKey(3);

    //sstream << "my_image" << image_count << ".png" ;           // added this
    std::string img_name;
    img_name="/tmp/image/" + std::to_string(count++) + ".png";
    //cv::Mat frame2;
    //cv::cvtColor(cv_ptr->image,frame2,CV_BGR2GRAY);
    //ROS_ASSERT( cv::imwrite( "/home/sae/r2d2_navigation_cps/image_ee/mmi_tape.png", frame2 ) );      // added this
    ROS_ASSERT( cv::imwrite( img_name, cv_ptr->image ) );      // added this
   }
  return true;
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ros::NodeHandle n;

  ros::Rate r(1);
  while(ros::ok()) {
    take_picture();
    ros::spinOnce();
    r.sleep();
  }
  return 0;
}
