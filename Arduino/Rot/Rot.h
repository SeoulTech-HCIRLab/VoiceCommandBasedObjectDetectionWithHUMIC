#include <Kangaroo.h>
#include <TimerOne.h>

#include <ros.h>
#include <std_msgs/Int16.h>
//#include <std_msgs/String.h>

#include <Target.h>

#include <time.h>

#define User_S Serial3

class NewHardware : public ArduinoHardware{
  public:
  NewHardware(): ArduinoHardware(&Serial3, 115200){};
};

void stop();
void resd_position();

int TARGET = false;
int ROT = false;
int FOR_S = false;

double now_pos[4];

//fix the culum and til of humic
long IMG_W = 1920;
long IMG_W_TH = 160;
int xmin = 960-160;
int xmax = 960+160;

//kinectv2
int Degree;
int DIREC;
int DEPTH=299;
int xc;
int START = false;

//ultrasonic sensors
int FRONT = 0;
int BACK = 0;
int LEFT = 0;
int RIGHT = 0;
