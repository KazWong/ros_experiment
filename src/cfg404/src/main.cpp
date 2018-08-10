#include <ros/ros.h>
#include "internal_use.h"
#include <iostream>

using namespace std;

int getValue(vector<unsigned char> obj) {
  int value = 0;
  int size = obj.size();
  unsigned char *byte = (unsigned char*)&value;

  //if ( (obj[size-1] & 0x80) == 0x80 )
    //value = -1;
    
  for (int i=0;i<size;i++)
    byte[i] = obj[i];

  return value;
}

int main (int argc, char **argv) {
  ros::init(argc, argv, "universal___no__config_package");
  KisXX dd;
  
  int i=1000;
  
  cout << sizeof(long) << endl;
  cout << sizeof(unsigned long) << endl;
  cout << sizeof(int) << endl;
  cout << sizeof(unsigned int) << endl;
  cout << sizeof(char) << endl;
  cout << sizeof(unsigned char) << endl;
  
  while (ros::ok() && i > -1000) {
    vector<unsigned char> buf;
    long value=0;
    unsigned char *byte = (unsigned char*)&i;
  
    for (int j=0;j<4;j++)
      buf.push_back(byte[j]);
      
    value = (long)getValue(buf);
    cout << i << " " << ( (double)value*0.1 ) * (2.0 * 3.1415926) / (32.0 * 10000.0 * 1.0) << endl;
    //dd.CodeName();
    //ros::spinOnce();
    i--;
    sleep(0.7);
  }
}
