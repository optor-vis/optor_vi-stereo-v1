/*
 * Copyright 2017 <copyright holder> <email>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "optor_stereo_visensor_ros/stereo_visensor_cam.h"

#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>

StereoVisensorCam::StereoVisensorCam(ros::NodeHandle nh, char path[]):visensor_Close_IMU_viewer_(false),close_img_viewer_(false)
{
  visensor_load_settings(path);
 
  pub_imu_ = nh.advertise<sensor_msgs::Imu>("imu0", 200);
  
  
  
  image_transport::ImageTransport imageTransL(nh);
  pub_caml_ = imageTransL.advertise("/camera/left/image_raw", 1);
  image_transport::ImageTransport imageTransR(nh);
  pub_camr_ = imageTransR.advertise("/camera/right/image_raw", 1);
  
  img_left_.create(cv::Size(visensor_img_width(),visensor_img_height()),CV_8UC1);
  img_right_.create(cv::Size(visensor_img_width(),visensor_img_height()),CV_8UC1);
  left_bridge_ = cv_bridge::CvImage(std_msgs::Header(), "mono8", img_left_);
  right_bridge_ = cv_bridge::CvImage(std_msgs::Header(), "mono8", img_right_);
  
}

bool StereoVisensorCam::startCam()
{
  return visensor_Start_Cameras() < 0 ? false : true;
}

bool StereoVisensorCam::startImu()
{
  return visensor_Start_IMU() < 0 ? false : true;
}



void StereoVisensorCam::img_data_stream ( )
{
  ros::NodeHandle cam_n;
  
  //如果相机打开成功，创建图片Mat,获取硬件帧率设置为图像发布频率
  ros::Rate loop_rate(visensor_get_hardware_fps());
  std::cout << visensor_get_hardware_fps() << std::endl;
  
  sensor_msgs::ImagePtr msg_l, msg_r;
  ros::Time msg_time;
  double left_timestamp, right_timestamp;
  visensor_imudata img_imudata;
  int i = 500; 
 
  while(cam_n.ok())
  {
    if(visensor_is_leftcam_open())
    {
      if(visensor_is_left_img_new())
      {
	visensor_get_left_latest_img(img_left_.data,&left_timestamp,&img_imudata);
	//printf("L-Time: %8.9f",img_imudata.timestamp);
	
	double  tmp =  floor(img_imudata.timestamp);
	msg_time.sec=(__time_t) tmp;
	msg_time.nsec=(__time_t)((img_imudata.timestamp - tmp)* 1e+9);
	//std::cout << "Lsec: " << msg_time.sec << "  -----nsec: " << msg_time.nsec << std::endl;
	
	left_bridge_.header.stamp = msg_time;
	left_bridge_.header.seq=0;
	
	
	//cv::imshow("left",img_left_);
	//cv::waitKey(1);
      }
    }
    if(visensor_is_rightcam_open())
    {
      if(visensor_is_right_img_new())
      {
	visensor_get_right_latest_img(img_right_.data,&right_timestamp);
	double  tmp =  floor(img_imudata.timestamp);
	msg_time.sec=(__time_t) tmp;
	msg_time.nsec=(__time_t)((img_imudata.timestamp - tmp)* 1e+9);
	
	right_bridge_.header.stamp = msg_time;
	right_bridge_.header.seq=0;
	
	//printf("R-Time: %8.9f",img_imudata.timestamp);
	//std::cout << "Rsec: " << msg_time.sec << "  -----nsec: " << msg_time.nsec << std::endl;
	//cv::imshow("right",img_right_);
	//cv::waitKey(1);
      }
    }
    msg_l = left_bridge_.toImageMsg();
    msg_r = right_bridge_.toImageMsg();
    
    pub_caml_.publish(msg_l);
    pub_camr_.publish(msg_r);
    
    //ros::spinOnce();
    loop_rate.sleep();
  }
  
}


void StereoVisensorCam:: imu_data_stream( )
{
  ros::NodeHandle imu_n;
  visensor_imudata imudata;
  sensor_msgs::Imu imu_msg;
  ros::Time imu_time;
  while(imu_n.ok())
  {
    if(visensor_imu_have_fresh_data())
    {
      visensor_get_imudata_latest(&imudata);
      
      /*
       *       printf("IMUTime:%8.9f, Gyr: %8.4f,%8.4f,%8.4f, Acc: %8.4f,%8.4f,%8.4f, Quat(WXYZ): %8.4f,%8.4f,%8.4f,%8.4f\n",
       *                   imudata.timestamp,
       *                   imudata.rx,imudata.ry,imudata.rz,
       *                   imudata.ax,imudata.ay,imudata.az,
       *                   imudata.qw,imudata.qx,imudata.qy,imudata.qz);
       */
      
      //向下取整
      double  tmp =  floor(imudata.timestamp);
      imu_time.sec=(__time_t) tmp;
      imu_time.nsec=(__time_t)((imudata.timestamp - tmp)* 1e+9);
      //std::cout << "sec: " << imu_time.sec << "   -----nsec:" << imu_time.nsec << std::endl;
      
      imu_msg.header.frame_id = "/imu";
      imu_msg.header.stamp = imu_time;
      imu_msg.header.seq=0;
      imu_msg.linear_acceleration.x=imudata.ax;
      imu_msg.linear_acceleration.y=imudata.ay;
      imu_msg.linear_acceleration.z=imudata.az;
      imu_msg.angular_velocity.x=3.1415926f*imudata.rx/180.0f;
      imu_msg.angular_velocity.y=3.1415926f*imudata.ry/180.0f;
      imu_msg.angular_velocity.z=3.1415926f*imudata.rz/180.0f;
      imu_msg.orientation.w=imudata.qw;
      imu_msg.orientation.x=imudata.qx;
      imu_msg.orientation.y=imudata.qy;
      imu_msg.orientation.z=imudata.qz;
      
      pub_imu_.publish(imu_msg);
    }
    usleep(100);
    
  }
}

void StereoVisensorCam::exectu()
{
  cam_thread_ = new boost::thread(boost::bind(&StereoVisensorCam::img_data_stream, this));
  imu_thread_ = new boost::thread(boost::bind(&StereoVisensorCam::imu_data_stream, this));
  cam_thread_->join();
  imu_thread_->join();
  
}


StereoVisensorCam::~StereoVisensorCam()
{
}

void Stereo ( const sensor_msgs::ImageConstPtr& msgLeft, const sensor_msgs::ImageConstPtr& msgRight )
{
    std::cout << "back" << std::endl;

}

