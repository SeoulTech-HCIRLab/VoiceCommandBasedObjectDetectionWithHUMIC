# VoiceCommandBasedObjectDetectionWithHUMIC

## github structure
```bash
├── Arduino
│   ├── Rot 
│   ├── USensor  
│   ├── librareis/yj  
│   └── README.md  
├── opencr 
│   ├── README.md 
│   └── position_mode.ino  
├── ros_pkg
│   ├── yj  
│   └── README.md
└── README.md
``` 

## start system
#### terminal1: start ros
```
$ roscore
```
#### terminal2: kinectv2
```
$ roslaunch kinect2_bridge kinect2_bridge.launch
```
#### terminal3: yolov3
```
$ roslaunch darknet_ros yolo_v3.launch
```
#### terminal4: for Mobile platform
```
$ rosrun rosserial_arduino serial_node.py _port:=/dev/ttyUSB0 _baud:=115200 __name:="mobile"
```
#### terminal5: for Ultra Sensors.
```
$ rosrun rosserial_arduino serial_node.py _port:=/dev/ttyACM0 _baud:=57600 __name:="ult"
```
port의 경우 Xavier에 연결하는 순서에 따라 바뀔 수 있다. 
Ultra Sensor의 아두이노를 먼저 연결하고 OpenCR을 연결하면, /dev/ttyACM0. 반대면 /dev/ttyACM1. 
포트 이름 확인은 `$ dmesg | grep tty`
#### terminal6: main
```
$ roslaunch yj main.launch
```
