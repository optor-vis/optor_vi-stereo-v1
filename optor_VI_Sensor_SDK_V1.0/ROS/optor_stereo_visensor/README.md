1.orb_slam_install.txt	提供ORB_SLAM2相关的安装步骤

2.install.sh	创建udev规则
    $ chmod +x install.sh
    $ sudo ./install

3.使用该ros_node
    $ catkin_make
    $ source devel/setup.bash	切换到工作空间根目录
    $ rosrun optor_stereo_visensor_ros stereo_visensor_node [optor_VISensor_Setups.txt的绝对路径]
