# IGV (Intelligent Ground Vehicle)
Source files for Intelligent Ground Vehicle project.

## Dependencies
###1. serial
A serial communication package from https://github.com/wjwwood/serial, was 
installed for globally use after June 27, 2016.

###2. sicktoolbox & sicktoolbox_wrapper
These two packages were installed for globally use instead of being used as 
submodules.

(1)Installation:

	$ sudo apt-get install ros-indigo-sicktoolbox
	$ sudo apt-get install ros-indigo-sicktoolbox-wrapper
	$ rosdep install sicktoolbox_wrapper rviz
	$ rosmake sicktoolbox_wrapper rviz

(2)Tutorial: http://wiki.ros.org/sicktoolbox_wrapper/Tutorials/UsingTheSicklms
