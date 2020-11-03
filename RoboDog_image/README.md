# RoboDog image for PI4 with 4GB memory and 32GB microSD

This is a microSD image with pre-installed Ubuntu 20.04, ROS noetic, RoboDog V2 and V3 controllers.<br/>
<br/>
!!! This image was created for PI4 with 4GB memory and 32GB microSD card and may not work on others PI configurations !!!<br/>

Link to image in GoogleDrive:
https://drive.google.com/file/d/1mRJEOfFArBO37XiQ0K2ioL9eDoHGEpOX/view?usp=sharing

Unzip the image, then use Win32DiskImager to write the image to microSD card:<br/>
https://wiki.ubuntu.com/Win32DiskImager

Attach monitor and USB keyboard to login and setup the WiFi connection.<br/>
If you don't have micro HDMI cable then plug the PI to your local network, find its IP adddress in your router WebUI and connect to PI over SSH (PuTTY):<br/>
https://www.putty.org/

Login: ubuntu<br/>
Password: ubuntu

To setup WiFi:<br/>
> sudo nmcli d wifi connect <my_wifi> password <password>

To run robodog V2:<br/>
> cd ~/catkin_ws<br/>
> rosrun robodog_v2 robodog_v2_hw<br/>

To run robodog V2 with V3 controller that supports Remote Control (you need also the HC-05 bluetooth module attached to PI com port):<br/>
> roslaunch robodog robodog.launch

Check this link for HC-05 bluetooth wiring to PI:<br/>
https://blog.miguelgrinberg.com/post/a-cheap-bluetooth-serial-port-for-your-raspberry-pi/page/0

To control robot you can use the RC:<br/>
https://github.com/RoboLabHub/RemoteControl

Or the Android application that emulates the RC:<br/>
https://github.com/RoboLabHub/Tips/tree/master/RoboDog_image/Joystick
