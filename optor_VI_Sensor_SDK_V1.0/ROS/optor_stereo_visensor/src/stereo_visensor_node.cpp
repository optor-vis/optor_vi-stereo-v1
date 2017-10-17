#include "optor_stereo_visensor_ros/stereo_visensor_cam.h"


int main(int argc, char**argv)
{
  ros::init(argc, argv, "optor_stereo_visensor");
  ros::NodeHandle nh;
  char *path = "optor_VISensor_Setups.txt";
//  StereoVisensorCam test;
  
/*  if(argc != 2)
	std::cout << "please input the path:Loitor_VISensor_Setups.txt" << std::endl;

  StereoVisensorCam test(nh,argv[1]);*/
  
//  if(argv[1]) test(nh, argv[1]);
//    else 
  if(argv[1])
  {
    path = argv[1];
    //StereoVisensorCam test(nh, argv[1]);
    //visensor_load_settings(argv[1]);
  }

  StereoVisensorCam test(nh, path);
  /*
  //ros::NodeHandle n("~/he");
  message_filters::Subscriber<sensor_msgs::Image> left_sub(nh, "/camera/left/image_raw", 1);
  message_filters::Subscriber<sensor_msgs::Image> right_sub(nh, "camera/right/image_raw", 1);
  
  typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image, sensor_msgs::Image> sync_pol;
  message_filters::Synchronizer<sync_pol> sync(sync_pol(10), left_sub,right_sub);
  sync.registerCallback(&Stereo);
  
  */
  
  if(!test.startCam())
    std::cout << "ERROR: camera open failed" << std::endl;
  if(!test.startImu())
    std::cout << "ERROR: imu open failed" << std::endl;
  
  test.exectu();
  
  //test.img_data_stream();
  //test.imu_data_stream();
  /* close cameras */
  //   visensor_Close_Cameras();
  /* close IMU */
  //  visensor_Close_IMU();
  
  return 0;
}
