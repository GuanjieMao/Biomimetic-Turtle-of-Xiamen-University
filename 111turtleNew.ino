#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "SoftwareSerial.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

SoftwareSerial BT(8, 9);           // pin8接蓝牙TXD，pin9接蓝牙RXD
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
int servoMax=513;
int servoMin=60;        // 舵机最小PWM计数值（out of 4096）
int X='A';
int initialPos[] = {90,140,70,120,55,90,90,80,90,90};




void setup() {
  Serial.begin(115200);
  Serial.println("蓝牙连接正常");     // 串口监视器显示蓝牙状态
  BT.begin(115200); 
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);
  // 初始化舵机为initialPos角度
  for (int i = 0; i < 12; i++) {
    pwm.setPWM(i, 0, map(initialPos[i], 0, 180, servoMin, servoMax));
  }
}

void loop(){
  /*int i=0;
  int channels4[] = {i};
  float startPos4[] = {90};
  float endPos4[] = {0};
  float step4[] = {2.00};

  int channels5[] = {i};
  float startPos5[] = { 0};
  float endPos5[] =  {90};
  float step5[] = {2.00};
  
  moveServos(channels4, startPos4, endPos4, 1, step4, 50);
  delay(1000);

  moveServos(channels5, startPos5, endPos5, 1, step5, 50);  
  delay(1000);

   
  pwm.setPWM(1, 0, map(90, 0, 90, servoMin, servoMax));
  delay(1000);

  pwm.setPWM(1, 0, map(50, 0, 90, servoMin, servoMax));
  delay(1000);

  pwm.setPWM(1, 0, map(90, 0, 90, servoMin, servoMax));
  delay(1000);

  pwm.setPWM(1, 0, map(45, 0, 90, servoMin, servoMax));
  delay(1000);*/

/*  if (X==0&&Serial.available())           //检测：【串口】如果数据写入，则执行。
  {
    X = Serial.read();              //把写入的数据给到自定义变量  X
    BT.print(X);                    //把数据给蓝牙
  }
  if (BT.available())               //检测：【蓝牙】如果数据写入，则执行。
  {
    X = BT.read();                  //把检测到的数据给到自定义变量 X
    Serial.print(X);                //把从蓝牙得到的数据显示到串口监视器
  }

  if(X=='A')     moveint();
  if(X=='B')  straight();
  if(X=='C')    left();
  if(X=='D')  right();
*/
swim_straight();
delay(1000);
//testServo(1);

}

void moveServos(const int* channels, const float* startPos, const float* endPos, int count, const float* step, int delayMs) {
  float pos[10];
  memcpy(pos, startPos, count * sizeof(float));
  
  bool condition = true;
  while (condition) {
    condition = false;
    for (int i = 0; i < count; i++) {
      if (startPos[i] < endPos[i] ? pos[i] <= endPos[i] : pos[i] >= endPos[i]) {
        pwm.setPWM(channels[i], 0, map(pos[i], 0, 180, servoMin, servoMax));
        pos[i] += startPos[i] < endPos[i] ? step[i] : -step[i];
        condition = true;
      }
    }
    delay(delayMs);
  }}

void testServo(const int* channel){
  /*while(1){
    for(int i = 0; i<18; i++){
      pwm.setPWM(channel, 0, map(10*i, 0, 180, servoMin, servoMax));
      delay(1000);
    }
  }*/
  pwm.setPWM(channel, 0, map(0, 0, 180, servoMin, servoMax));
  delay(1000);
}

void walk1_straight(){
  int channels1[] = {1,2,3,4};
  float startPos1[] = {135, 90, 90, 70};
  float endPos1[] = {105, 60, 120, 85};
  float step1[] = {0.75, 0.75, 0.75, 0.75};

  int channels2[] = {1,2,3,4};
  float startPos2[] = {105, 60, 120, 85};
  float endPos2[] = {145, 90, 90, 60};
  float step2[] = {1.00, 0.75, 0.75, 1.00};

  int channels3[] = {1,4};
  float startPos3[] = {145, 60};
  float endPos3[] = {135, 70};
  float step3[] = {0.25, 0.25};

  moveServos(channels1, startPos1, endPos1, 4, step1, 25);

  moveServos(channels2, startPos2, endPos2, 4, step2, 12);

  moveServos(channels3, startPos3, endPos3, 2, step3, 12);

}

void walk2_straight(){
  int channels1[] = {0,2,3,5};
  float startPos1[] = {90,90,90,90};
  float endPos1[] = {140,130,50,40};
  float step1[] = {0.50,0.40,0.40,0.50};

  int channels2[] = {1,4};
  float startPos2[] = {135,55};
  float endPos2[] = {85,105};
  float step2[] = {0.50,.50};

  int channels3[] = {0,2,3,5};
  float startPos3[] = {140,130,50,40};
  float endPos3[] = {160,30,150,20};
  float step3[] = {0.20,1.00,1.00,0.20};

  int channels4[] = {0,1,2,3,4,5};
  float startPos4[] = {160,85,30,150,105,20};
  float endPos4[] = {90,135,90,90,55,90};
  float step4[] = {0.70,0.50,0.6,0.6,0.50,0.70};

  moveServos(channels1, startPos1, endPos1, 4, step1, 5);

  moveServos(channels2, startPos2, endPos2, 2, step2, 5);

  moveServos(channels3, startPos3, endPos3, 4, step3, 5);

  moveServos(channels4, startPos4, endPos4, 6, step4, 5);

}

void swim_straight(){
  int channels1[] = {2,3,6,7,8,9};
  float startPos1[] = {70,120,90,80,90,90};
  float endPos1[] = {130,60,130,120,50,50};
  float step1[] = {1.00,1.00,0.66,0.66,0.66,0.66};

  int channels2[] = {0,2,3,5,6,7,8,9};
  float startPos2[] = {90,130,60,90,130,120,50,50};
  float endPos2[] = {130,115,75,50,120,70,100,60};
  float step2[] = {1.00,0.325,0.325,1.00,0.42,2.10,2.10,0.42};

  int channels3[] = {1,2,3,4,6,7,8,9};
  float startPos3[] = {140,115,75,55,120,70,100,60};
  float endPos3[] = {100,75,115,95,90,80,90,90};
  float step3[] = {1.43,1.43,1.43,1.43,1.07,0.36,0.36,1.07};

  int channels4[] = {0,1,2,3,4,5};
  float startPos4[] = {130,100,75,115,95,50};
  float endPos4[] = {90,140,70,120,55,90};
  float step4[] = {2.00,2.00,0.25,0.25,2.00,2.00};

  moveServos(channels1, startPos1, endPos1, 6, step1, 25);

  moveServos(channels2, startPos2, endPos2, 8, step2, 25);

  moveServos(channels3, startPos3, endPos3, 8, step3, 25);

  moveServos(channels4, startPos4, endPos4, 6, step4, 25);

}

void right(){
  int channels1[] = {3,8,9};
  float startPos1[] = { 90, 90, 90};
  float endPos1[] = { 30, 47, 50};
  float step1[] = { 3.00, 2.15, 2.00};

  int channels2[] = {3,5,8,9};
  float startPos2[] = { 30, 45, 47, 50};
  float endPos2[] = { 39, 69,  100, 60};
  float step2[] = { 0.45, 1.20, 2.65, 0.5};

  int channels3[] = {3,5,8,9};
  float startPos3[] = { 39, 69, 100, 60};
  float endPos3[] = {45, 85, 96.36, 70.90};
  float step3[] = { 0.30, 0.80, 0.18, 0.55};

  int channels4[] = {3,4,8,9};
  float startPos4[] = { 45, 0,  96.36, 70.90};
  float endPos4[] = {65, 40, 90, 90};
  float step4[] = { 1.00, 2.00, 0.32, 0.95};

  int channels5[] = {3,4,5};
  float startPos5[] = { 65, 40, 85};
  float endPos5[] = {90, 0, 45};
  float step5[] = {1.25, 2.00, 2.00};
  

  moveServos(channels1, startPos1, endPos1, 3, step1, 75);
  delay(2000);


  moveServos(channels2, startPos2, endPos2, 4, step2, 30);
  delay(2000);


  moveServos(channels3, startPos3, endPos3, 4, step3, 20);

  delay(2000);

  moveServos(channels4, startPos4, endPos4, 4, step4, 35);
  delay(2000);


  moveServos(channels5, startPos5, endPos5, 3, step5, 25);  
  delay(2000);


}

void left(){
  int channels1[] = {2,6,7};
  float startPos1[] = {0, 0, 0,};
  float endPos1[] = {60, 40, 43};
  float step1[] = {3.00, 2.00, 2.15};

  int channels2[] = {0,2,6,7};
  float startPos2[] = {45, 60,40, 43};
  float endPos2[] = {21, 51, 30, -10};
  float step2[] = {1.20, 0.45, 0.50, 2.65};

  int channels3[] = {0,2,6,7};
  float startPos3[] = {21, 51, 30, -10};
  float endPos3[] = {5, 45, 19.09, -6.36};
  float step3[] = {0.80, 0.30, 0.55, 0.18};

  int channels4[] = {1,2,6,7};
  float startPos4[] = {90, 45, 19.09, -6.36};
  float endPos4[] = {50, 25, 0, 0};
  float step4[] = {2.00, 1.00, 0.95, 0.32};

  int channels5[] = {0,1,2};
  float startPos5[] = {5, 50, 25};
  float endPos5[] = {45, 90, 0};
  float step5[] = {2.00, 2.00, 1.25};

  moveServos(channels1, startPos1, endPos1, 3, step1, 75);

  moveServos(channels2, startPos2, endPos2, 4, step2, 30);

  moveServos(channels3, startPos3, endPos3, 4, step3, 20);

  moveServos(channels4, startPos4, endPos4, 4, step4, 35);

  moveServos(channels5, startPos5, endPos5, 3, step5, 25);  

}

void moveint(){
  for (int i = 0; i < 12; i++) {
    pwm.setPWM(i, 0, map(initialPos[i], 0, 90, servoMin, servoMax));
  }
}