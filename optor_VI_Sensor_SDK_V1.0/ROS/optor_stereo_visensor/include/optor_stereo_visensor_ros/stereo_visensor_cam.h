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

#ifndef STEREOVISENSORCAM_H
#define STEREOVISENSORCAM_H

#include "ros/ros.h" 
#include "std_msgs/String.h"
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include "sensor_msgs/Imu.h"

#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>

#include<boost/thread/thread.hpp>
#include <sys/time.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "optorcam.h"

class StereoVisensorCam
{
public:
    StereoVisensorCam(ros::NodeHandle nh,char path[]);
    ~StereoVisensorCam();
    bool startCam();
    bool startImu();
    void img_data_stream();
    void imu_data_stream();
    void exectu();
    
    
  boost::thread *cam_thread_;
  boost::thread *imu_thread_;
    
    cv::Mat img_left_, img_right_;
    cv_bridge::CvImage left_bridge_, right_bridge_;
    image_transport::Publisher pub_caml_, pub_camr_;
    
    
    ros::Publisher pub_imu_;
    
    //线程控制
    bool visensor_Close_IMU_viewer_;
    bool close_img_viewer_;
};

#endif // STEREOVISENSORCAM_H
