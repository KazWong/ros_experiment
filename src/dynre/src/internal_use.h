#include <ros/ros.h>
#include <stdio.h>
#include <mutex>
#include <dynamic_reconfigure/server.h>
#include <dynre/DynReConfig.h>

class DynRe {
private:
  typedef dynamic_reconfigure::Server<dynre::DynReConfig> drs;
  std::shared_ptr<drs> drs_;

  double _from_me_to_you;
public:
  DynRe() {
    ros::NodeHandle pn("Mother_Config/b");
    
    drs_.reset(new drs(pn));
    drs::CallbackType cb = boost::bind(&DynRe::reconfigureCallback, this, _1, _2);
    drs_->setCallback(cb);
  }
  
  void reconfigureCallback(dynre::DynReConfig& config, uint32_t level) {
    _from_me_to_you = config.from_me_to_you;
  }
  
  void CodeName() {
    printf("%f\n", _from_me_to_you);
  }
};
