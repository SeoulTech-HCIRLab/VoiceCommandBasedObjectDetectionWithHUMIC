#define USE_USBCON true

#include "Rot.h"

KangarooSerial  Front(Serial1);
KangarooSerial  Rear(Serial1);

KangarooChannel K1(Front, '1', 128);  // 모터1, Front Left
KangarooChannel K2(Front, '2', 128);  // 모터2, Front Right
KangarooChannel K3(Rear,  '1', 129);  // 모터3, Rear Left
KangarooChannel K4(Rear,  '2', 129);  // 모터4, Rear Right

KangarooStatus KS;

ros::NodeHandle_<NewHardware> nh;

std_msgs::Int16 done_flag;

void set_front(const std_msgs::Int16& data){ FRONT = data.data; }
void set_back(const std_msgs::Int16& data){ BACK = data.data; }
void set_left(const std_msgs::Int16& data){ LEFT = data.data; }
void set_right(const std_msgs::Int16& data){ RIGHT = data.data; }
void set_target(const yj::Target& data){
  TARGET = data.target;
  Degree = data.degree;
  DIREC = data.direction;
  DEPTH = data.depth;
  xc = data.xc;
  
  START = data.start_flag;
}

ros::Subscriber<std_msgs::Int16> front_sub("/ussensor_front", &set_front);
//ros::Subscriber<std_msgs::Int16> back_sub("/ussensor_back", &set_back);
//ros::Subscriber<std_msgs::Int16> left_sub("/ussensor_left", &set_left);
//ros::Subscriber<std_msgs::Int16> right_sub("/ussensor_right", &set_right);
ros::Subscriber<yj::Target> target_sub("/target/xyxy_depth", &set_target);

void setup() {
  User_S.begin(115200);
  User_S.println("Test Kangaroo");
  Serial1.begin(19200);

  nh.initNode();
  nh.subscribe(front_sub);
  //nh.subscribe(back_sub);
  //nh.subscribe(left_sub);
  //nh.subscribe(right_sub);
  nh.subscribe(target_sub);

  done_flag.data = false;
  
  K1.start();
  K1.home().wait();
  K2.start();
  K2.home().wait();
  K3.start();
  K3.home().wait();
  K4.start();
  K4.home().wait();

  // 타이머 설정
  Timer1.initialize(1000000);   // 분해능 : 1us -> 현재 설정값 : 100ms
  Timer1.attachInterrupt(timerinter);

  User_S.println("Setup OK");
}

void loop() {
  /*
  User_S.println("loop");
  User_S.print("TARGET: ");
  User_S.println(TARGET);
  User_S.print("FOR_S: ");
  User_S.println(FOR_S);
  User_S.print("done: ");
  User_S.println(done_flag.data);
  User_S.print("START: ");
  User_S.println(START);
  read_position();
  print_position();
  */

  if(TARGET){
    if(!FOR_S && !done_flag.data){
      User_S.print("center of x:");
      User_S.println(xc);
      User_S.println("rotation");
      rotation();
      User_S.print("center of x:");
      User_S.println(xc);
      //ROT = true;
      FOR_S = true;
    }
  }

  if(FOR_S){
    forward();
  }

  delay(100);
  if(done_flag.data && START){
    done_flag.data = false;
    //ROT = false;
    User_S.println("init");

    K1.home().wait();
    K2.home().wait();
    K3.home().wait();
    K4.home().wait();
  }
}

void stop() {
  K1.s(0);
  K2.s(0);
  K3.s(0);
  K4.s(0);
}

void rotation(){
  long distance = Degree * 13.77 + 0.5;
  long k13 = DIREC*distance;
  long k24 = DIREC*-1*distance;

  User_S.print(Degree);
  User_S.print("degree");
  if(xc <= xmin || xc >= xmax){
    if(DIREC == 1){
      User_S.println("turn left");
    }
    else{
      User_S.println("turn right");
    }

    K1.p(k13);
    K2.p(k24);
    K3.p(k13);
    K4.p(k24).wait();
  }
  else{
    User_S.println("already the middle of view");
  }
}

void forward() {

  if(FRONT < 1 || FRONT > 50){
    K1.s(-115);
    K2.s(-115);
    K3.s(-115);
    K4.s(-115);
  }
  else{
    User_S.println("going is done");
    User_S.println("stop");
    User_S.print("sensor:");
    User_S.println(FRONT);
    stop();
    FOR_S = false;
    done_flag.data = true;
  }
}

void read_position()
{
  now_pos[0] = K1.getP().value();
  now_pos[1] = K2.getP().value();
  now_pos[2] = K3.getP().value();
  now_pos[3] = K4.getP().value();
}

void print_position()
{
  for(int i=0; i<4; i++){
    User_S.print(now_pos[i]);
    User_S.print(",");
  }
  User_S.println();
}

void timerinter() {
  nh.spinOnce();
}

