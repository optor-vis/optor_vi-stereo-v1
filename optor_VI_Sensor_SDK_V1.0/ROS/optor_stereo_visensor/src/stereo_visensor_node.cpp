#include "optor_stereo_visensor_ros/stereo_visensor_cam.h"


int main(int argc, char**argv)
{
  ros::init(argc, argv, "optor_stereo_visensor");
  ros::NodeHandle nh;
  char *path = "optor_VISensor_Setups.txt";

  if(argv[1])
  {
    path = argv[1];

  }

  StereoVisensorCam StereoVisensorStart(nh, path);

  
  if(!StereoVisensorStart.startCam())
    std::cout << "ERROR: camera open failed" << std::endl;
  if(!StereoVisensorStart.startImu())
    std::cout << "ERROR: imu open failed" << std::endl;
  
  StereoVisensorStart.exectu();
  

  return 0;
}
