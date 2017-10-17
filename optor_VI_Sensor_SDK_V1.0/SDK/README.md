# vi_sensor_sdk_v2
optor VI Sensor SDK V2

Before you start:
Follow this step to add optor to your udev rules:
1. chmod +x optor-vi-install.sh
2. sudo ./optor-vi-install.sh
3. (maybe a restart is needed)

And... add your current user to dialout user group(Suppose your username is joeuser):
1. groups joeuser
..this will list all the groups you belong to. 
2. If you don't belong to the dialout group then add yourself to it, for example:
sudo gpasswd --add joeuser dialout
3. !!! You need to log out and log back in again for it to be effective. 

Now you can run the test program:
1. mkdir build
2. cd build
3. cmake ..
4. make
5. ./camtest

