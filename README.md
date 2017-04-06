# IGV (Intelligent Ground Vehicle)
Source files for Intelligent Ground Vehicle project.

## Packages
###1. dpralte060b080
A package used for communicating with the Stepper Motor using serial communication. 

###2. os5000
A package used for communicating with the OS5000 Compass using serial communication. 

## Dependencies
### 1. serial
A serial communication package from https://github.com/wjwwood/serial, was 
installed for global use after June 27, 2016.

### 2. sicktoolbox & sicktoolbox_wrapper
These two packages are used to interface with the SICK LiDAR and were installed for global use instead of being used as submodules.

(1)Installation:

	$ sudo apt-get install ros-indigo-sicktoolbox
	$ sudo apt-get install ros-indigo-sicktoolbox-wrapper
	$ rosdep install sicktoolbox_wrapper rviz
	$ rosmake sicktoolbox_wrapper rviz

(2)Tutorial: http://wiki.ros.org/sicktoolbox_wrapper/Tutorials/UsingTheSicklms

### 3. Persistent Serial names
Provides persistent names to devices plugged in the USB ports

Paste 99-usb-serial.rules in /etc/udev/rules.d
