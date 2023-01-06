
#define USE_USBCON true

#include <ros.h>
#include <std_msgs/Int16.h>

//ultrasonic sensor
void Func_Sensor();
long trig_ultra(int trig, int echo);

#define SONAR_NUM 4
#define MAX_DISTANCE 120*58.2
#define PING_INTERVAL 33

unsigned long pingTimer[SONAR_NUM];
unsigned int now_cm[SONAR_NUM];
uint8_t currentSensor = 0;

//order: front, back, left, right
int trig[SONAR_NUM] = {24, 26, 28, 30};
int echo[SONAR_NUM] = {25, 27, 29, 31};

ros::NodeHandle nh;

std_msgs::Int16 front;
std_msgs::Int16 back;
std_msgs::Int16 left;
std_msgs::Int16 right;

ros::Publisher front_pub("ussensor_front", &front);
ros::Publisher back_pub("ussensor_back", &back);
ros::Publisher left_pub("ussensor_left", &left);
ros::Publisher right_pub("ussensor_right", &right);

void setup() {

  Serial.begin(57600);
  Serial.println("Ultrasonic sensor Arduino");

  for(int i=0; i<SONAR_NUM; i++){
    pinMode(trig[i], OUTPUT);
    pinMode(echo[i], INPUT);
  }
  
  for(int i=1; i<SONAR_NUM; i++){
    pingTimer[i] = pingTimer[i-1] + PING_INTERVAL;
  }

  nh.initNode();
  nh.advertise(front_pub);
  nh.advertise(back_pub);
  nh.advertise(left_pub);
  nh.advertise(right_pub);

}

void loop() {

  Func_Sensor();

  Serial.print("front:");
  Serial.println(now_cm[0]);
  Serial.print("back:");
  Serial.println(now_cm[1]);
  Serial.print("left:");
  Serial.println(now_cm[2]);
  Serial.print("right:");
  Serial.println(now_cm[3]);

  front_pub.publish(&front);
  back_pub.publish(&back);
  left_pub.publish(&left);
  right_pub.publish(&right);

  nh.spinOnce();
  delay(100);

}

void Func_Sensor(){
  for(int i=0; i<SONAR_NUM; i++){
    if(millis() >= pingTimer[i]){
      pingTimer[i] += PING_INTERVAL * SONAR_NUM;
      currentSensor = i;
      now_cm[i] = trig_ultra(trig[i], echo[i]);

      switch(i){
        case 0:
          front.data = trig_ultra(trig[i], echo[i]);
        case 1:
          back.data = trig_ultra(trig[i], echo[i]);
        case 2:
          left.data = trig_ultra(trig[i], echo[i]);
        case 3:
          right.data = trig_ultra(trig[i], echo[i]);
      }
    }
  }
}


long trig_ultra(int trig, int echo){
  long distance;
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  distance = pulseIn(echo, HIGH, MAX_DISTANCE)/58.2;

  return(distance);
}
