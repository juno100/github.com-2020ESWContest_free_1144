/*#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif */
 
#include <Servo.h>
#include <ros.h>
#include <std_msgs/UInt16MultiArray.h>
 
ros::NodeHandle  nh;
 
Servo servo;
 
int a1 = 3;
int a2 = 4;
int pa1= 2;
int a3 = 6;
int a4 = 7;
int pa2 = 5;

int dir1 = 0;
int dir2 = 0;
int pwm1 = 0;

int dir3 = 0;
int dir4 = 0;
int pwm2 = 0;
 
void servo_cb( const std_msgs::UInt16MultiArray& cmd_msg){
  dir1 = cmd_msg.data[0];
  dir2 = cmd_msg.data[1];  //set servo angle, should be from 0-180
  pwm1 = cmd_msg.data[2];

  dir3 = cmd_msg.data[3];
  dir4 = cmd_msg.data[4];
  pwm2 = cmd_msg.data[5];
  
  digitalWrite(a1,dir1);
  digitalWrite(a2,dir2);
  analogWrite(pa1,pwm1);
  
  digitalWrite(a3,dir3);
  digitalWrite(a4,dir4);
  analogWrite(pa2,pwm2);
  
  //servo.write(angle);
  //digitalWrite(13, HIGH-digitalRead(13));  //toggle led  
}
 
 
ros::Subscriber<std_msgs::UInt16MultiArray> sub("servo", servo_cb);
 
void setup(){
  //pinMode(13, OUTPUT);
  pinMode(a1,OUTPUT);
  pinMode(a2,OUTPUT);
  pinMode(pa1,OUTPUT);
  pinMode(a3,OUTPUT);
  pinMode(a4,OUTPUT);
  pinMode(pa2,OUTPUT);
  nh.initNode();
  nh.subscribe(sub);
  //servo.attach(6);
}
 
void loop(){
   
  nh.spinOnce();
  delay(1);
} 
