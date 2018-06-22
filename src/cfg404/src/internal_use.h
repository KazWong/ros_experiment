#include <ros/ros.h>
#include <stdio.h>
#include <mutex>
#include <string>
#include <dynamic_reconfigure/server.h>
#include <dynre/confignameConfig.h>

class KisXX {
private:
  typedef dynamic_reconfigure::Server<config404::confignameConfig> drs;
  std::shared_ptr<drs> drs_;

  std::string _WhyWhytellmewhy;
public:
  KisXX() {
    ros::NodeHandle pn("Mother_Config/a");
    
    drs_.reset(new drs(pn));
    drs::CallbackType cb = boost::bind(&KisXX::reconfigureCallback, this, _1, _2);
    drs_->setCallback(cb);
  }
  
  void reconfigureCallback(config404::confignameConfig& config, uint32_t level) {
    _WhyWhytellmewhy = config.WhyWhytellmewhy;
  }
  
  void CodeName() {
    printf("%s\n", _WhyWhytellmewhy.c_str());
  }
};
