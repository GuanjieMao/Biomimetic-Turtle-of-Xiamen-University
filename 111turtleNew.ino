#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "SoftwareSerial.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <avr/pgmspace.h>

SoftwareSerial BT(8, 9);           // pin8接蓝牙TXD，pin9接蓝牙RXD
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
int servoMax=513;
int servoMin=60;        // 舵机最小PWM计数值（out of 4096）
int X='A';
int Mode = 'A';  // 默认模式
int lastType2Value = 0;  // 上一次类型2值

// PROGMEM arrays for movement data
// Using uint8_t for small integers to save space
// walk1_straight arrays
const uint8_t PROGMEM initialPos_walk1_straight[] = {90,125,120,120,100,90,100,110,90,80};
const uint8_t PROGMEM channels_walk1_straight_1[] = {1,2,3,7,8};
const float PROGMEM startPos_walk1_straight_1[] = {125,120,120,110,90};
const float PROGMEM endPos_walk1_straight_1[] = {95,90,135,90,60};
const float PROGMEM step_walk1_straight_1[] = {0.3,0.3,0.15,0.2,0.3};

const uint8_t PROGMEM channels_walk1_straight_2[] = {3,4,7,8};
const float PROGMEM startPos_walk1_straight_2[] = {135,100,90,60};
const float PROGMEM endPos_walk1_straight_2[] = {90,45,110,65};
const float PROGMEM step_walk1_straight_2[] = {1.8,2.2,0.8,0.2};

const uint8_t PROGMEM channels_walk1_straight_3[] = {2,3,4,7,8};
const float PROGMEM startPos_walk1_straight_3[] = {90,90,45,110,65};
const float PROGMEM endPos_walk1_straight_3[] = {60,60,70,90,70};
const float PROGMEM step_walk1_straight_3[] = {1.2,1.2,1.0,0.8,0.2};

const uint8_t PROGMEM channels_walk1_straight_4[] = {2,3,4,7,8};
const float PROGMEM startPos_walk1_straight_4[] = {60,60,70,90,70};
const float PROGMEM endPos_walk1_straight_4[] = {45,90,100,120,90};
const float PROGMEM step_walk1_straight_4[] = {0.3,0.6,0.6,0.6,0.4};

const uint8_t PROGMEM channels_walk1_straight_5[] = {1,2,7,8};
const float PROGMEM startPos_walk1_straight_5[] = {95,45,120,90};
const float PROGMEM endPos_walk1_straight_5[] = {150,90,115,70};
const float PROGMEM step_walk1_straight_5[] = {1.1,0.9,0.1,0.4};

const uint8_t PROGMEM channels_walk1_straight_6[] = {1,2,3,7,8};
const float PROGMEM startPos_walk1_straight_6[] = {150,90,90,115,70};
const float PROGMEM endPos_walk1_straight_6[] = {125,120,120,110,90};
const float PROGMEM step_walk1_straight_6[] = {0.5,0.6,0.6,0.1,0.4};

// walk2_straight arrays
const uint8_t PROGMEM channels_walk2_straight_1[] = {0,2,3,5};
const float PROGMEM startPos_walk2_straight_1[] = {90,90,90,90};
const float PROGMEM endPos_walk2_straight_1[] = {140,130,50,40};
const float PROGMEM step_walk2_straight_1[] = {0.50,0.40,0.40,0.50};

const uint8_t PROGMEM channels_walk2_straight_2[] = {1,4};
const float PROGMEM startPos_walk2_straight_2[] = {135,55};
const float PROGMEM endPos_walk2_straight_2[] = {85,105};
const float PROGMEM step_walk2_straight_2[] = {0.50,.50};

const uint8_t PROGMEM channels_walk2_straight_3[] = {0,2,3,5};
const float PROGMEM startPos_walk2_straight_3[] = {140,130,50,40};
const float PROGMEM endPos_walk2_straight_3[] = {160,30,150,20};
const float PROGMEM step_walk2_straight_3[] = {0.20,1.00,1.00,0.20};

const uint8_t PROGMEM channels_walk2_straight_4[] = {0,1,2,3,4,5};
const float PROGMEM startPos_walk2_straight_4[] = {160,85,30,150,105,20};
const float PROGMEM endPos_walk2_straight_4[] = {90,135,90,90,55,90};
const float PROGMEM step_walk2_straight_4[] = {0.70,0.50,0.6,0.6,0.50,0.70};

// swim1_straight arrays
const uint8_t PROGMEM channels_swim1_straight_1[] = {2,3,6,7,8,9};
const float PROGMEM startPos_swim1_straight_1[] = {70,120,90,80,90,90};
const float PROGMEM endPos_swim1_straight_1[] = {130,60,130,120,50,50};
const float PROGMEM step_swim1_straight_1[] = {1.00,1.00,0.66,0.66,0.66,0.66};

const uint8_t PROGMEM channels_swim1_straight_2[] = {0,2,3,5,6,7,8,9};
const float PROGMEM startPos_swim1_straight_2[] = {90,130,60,90,130,120,50,50};
const float PROGMEM endPos_swim1_straight_2[] = {130,115,75,50,120,70,100,60};
const float PROGMEM step_swim1_straight_2[] = {1.00,0.325,0.325,1.00,0.42,2.10,2.10,0.42};

const uint8_t PROGMEM channels_swim1_straight_3[] = {1,2,3,4,6,7,8,9};
const float PROGMEM startPos_swim1_straight_3[] = {140,115,75,55,120,70,100,60};
const float PROGMEM endPos_swim1_straight_3[] = {100,75,115,95,90,80,90,90};
const float PROGMEM step_swim1_straight_3[] = {1.43,1.43,1.43,1.43,1.07,0.36,0.36,1.07};

const uint8_t PROGMEM channels_swim1_straight_4[] = {0,1,2,3,4,5};
const float PROGMEM startPos_swim1_straight_4[] = {130,100,75,115,95,50};
const float PROGMEM endPos_swim1_straight_4[] = {90,140,70,120,55,90};
const float PROGMEM step_swim1_straight_4[] = {2.00,2.00,0.25,0.25,2.00,2.00};

// swim2_straight arrays
const uint8_t PROGMEM initialPos_swim2_straight[] = {100,150,125,55,45,80,100,90,90,80};
const uint8_t PROGMEM channels_swim2_straight_1[] = {0,1,4,5};
const float PROGMEM startPos_swim2_straight_1[] = {100,150,45,80};
const float PROGMEM endPos_swim2_straight_1[] = {90,140,55,90};
const float PROGMEM step_swim2_straight_1[] = {1.0,1.0,1.0,1.0};

const uint8_t PROGMEM channels_swim2_straight_2[] = {1,4,7,8};
const float PROGMEM startPos_swim2_straight_2[] = {140,55,90,90};
const float PROGMEM endPos_swim2_straight_2[] = {95,100,80,100};
const float PROGMEM step_swim2_straight_2[] = {4.5,4.5,1.0,1.0};

const uint8_t PROGMEM channels_swim2_straight_3[] = {0,2,3,5};
const float PROGMEM startPos_swim2_straight_3[] = {90,125,55,90};
const float PROGMEM endPos_swim2_straight_3[] = {100,110,70,80};
const float PROGMEM step_swim2_straight_3[] = {1.0,1.5,1.5,1.0};

const uint8_t PROGMEM channels_swim2_straight_4[] = {0,1,4,5,7,8};
const float PROGMEM startPos_swim2_straight_4[] = {100,95,100,80,80,100};
const float PROGMEM endPos_swim2_straight_4[] = {90,120,75,90,100,80};
const float PROGMEM step_swim2_straight_4[] = {1.0,2.5,2.5,1.0,1.0,1.0};

const uint8_t PROGMEM channels_swim2_straight_5[] = {0,1,2,3,4,5};
const float PROGMEM startPos_swim2_straight_5[] = {90,120,110,70,75,90};
const float PROGMEM endPos_swim2_straight_5[] = {80,140,135,45,55,100};
const float PROGMEM step_swim2_straight_5[] = {0.5,1.0,1.25,1.25,1.0,0.5};

const uint8_t PROGMEM channels_swim2_straight_6[] = {0,1,2,3,4,5,7,8};
const float PROGMEM startPos_swim2_straight_6[] = {80,140,135,45,55,100,100,80};
const float PROGMEM endPos_swim2_straight_6[] = {100,150,125,55,45,80,90,90};
const float PROGMEM step_swim2_straight_6[] = {1.0,0.5,0.5,0.5,0.5,1.0,0.5,0.5};

// walk1_left arrays
const uint8_t PROGMEM initialPos_walk1_left[] = {90,95,60,120,100,90,100,90,90,80};
const uint8_t PROGMEM channels_walk1_left_1[] = {3,8};
const float PROGMEM startPos_walk1_left_1[] = {120,90};
const float PROGMEM endPos_walk1_left_1[] = {135,60};
const float PROGMEM step_walk1_left_1[] = {0.15,0.3};

const uint8_t PROGMEM channels_walk1_left_2[] = {3,4,8};
const float PROGMEM startPos_walk1_left_2[] = {135,100,60};
const float PROGMEM endPos_walk1_left_2[] = {90,45,65};
const float PROGMEM step_walk1_left_2[] = {1.8,2.2,0.2};

const uint8_t PROGMEM channels_walk1_left_3[] = {3,4,8};
const float PROGMEM startPos_walk1_left_3[] = {90,45,65};
const float PROGMEM endPos_walk1_left_3[] = {60,70,70};
const float PROGMEM step_walk1_left_3[] = {1.2,1.0,0.2};

const uint8_t PROGMEM channels_walk1_left_4[] = {3,4,8};
const float PROGMEM startPos_walk1_left_4[] = {60,70,70};
const float PROGMEM endPos_walk1_left_4[] = {90,100,90};
const float PROGMEM step_walk1_left_4[] = {0.6,0.6,0.4};

const uint8_t PROGMEM channels_walk1_left_5[] = {8};
const float PROGMEM startPos_walk1_left_5[] = {90};
const float PROGMEM endPos_walk1_left_5[] = {70};
const float PROGMEM step_walk1_left_5[] = {0.4};

const uint8_t PROGMEM channels_walk1_left_6[] = {3,8};
const float PROGMEM startPos_walk1_left_6[] = {90,70};
const float PROGMEM endPos_walk1_left_6[] = {120,90};
const float PROGMEM step_walk1_left_6[] = {0.6,0.4};

// walk1_right arrays
const uint8_t PROGMEM initialPos_walk1_right[] = {90,125,120,120,100,90,100,110,90,80};
const uint8_t PROGMEM channels_walk1_right_1[] = {1,2,7};
const float PROGMEM startPos_walk1_right_1[] = {125,120,110};
const float PROGMEM endPos_walk1_right_1[] = {95,90,90};
const float PROGMEM step_walk1_right_1[] = {0.3,0.3,0.2};

const uint8_t PROGMEM channels_walk1_right_2[] = {7};
const float PROGMEM startPos_walk1_right_2[] = {90};
const float PROGMEM endPos_walk1_right_2[] = {110};
const float PROGMEM step_walk1_right_2[] = {0.8};

const uint8_t PROGMEM channels_walk1_right_3[] = {2,7};
const float PROGMEM startPos_walk1_right_3[] = {90,110};
const float PROGMEM endPos_walk1_right_3[] = {60,90};
const float PROGMEM step_walk1_right_3[] = {1.2,0.8};

const uint8_t PROGMEM channels_walk1_right_4[] = {2,7};
const float PROGMEM startPos_walk1_right_4[] = {60,90};
const float PROGMEM endPos_walk1_right_4[] = {45,120};
const float PROGMEM step_walk1_right_4[] = {0.3,0.6};

const uint8_t PROGMEM channels_walk1_right_5[] = {1,2,7};
const float PROGMEM startPos_walk1_right_5[] = {95,45,120};
const float PROGMEM endPos_walk1_right_5[] = {150,90,115};
const float PROGMEM step_walk1_right_5[] = {1.1,0.9,0.1};

const uint8_t PROGMEM channels_walk1_right_6[] = {1,2,7};
const float PROGMEM startPos_walk1_right_6[] = {150,90,115};
const float PROGMEM endPos_walk1_right_6[] = {125,120,110};
const float PROGMEM step_walk1_right_6[] = {0.5,0.6,0.1};


void setup() {
  Serial.begin(115200);
  Serial.println("蓝牙连接正常");     // 串口监视器显示蓝牙状态
  BT.begin(115200); 
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);
  // 初始化舵机为initialPos角度
  int initialPos[] = {90,140,70,120,55,90,90,80,90,90};
  for (int i = 0; i < 10; i++) {
    pwm.setPWM(i, 0, map(initialPos[i], 0, 180, servoMin, servoMax));
  }
}

/*void loop(){
  int i=0;
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
  delay(1000);

  if (X==0&&Serial.available())     //检测：【串口】如果数据写入，则执行。
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
  if(X=='B')  walk1_straight();
  if(X=='C')    left();
  if(X=='D')  right();

}*/

void loop() {
  if (X==0&&Serial.available())     //检测：【串口】如果数据写入，则执行。
  {
    X = Serial.read();              //把写入的数据给到自定义变量  X
    BT.print(X);                    //把数据给蓝牙
  }
  if (BT.available())               //检测：【蓝牙】如果数据写入，则执行。
  {
    X = BT.read();                  //把检测到的数据给到自定义变量 X
    Serial.print(X);                //把从蓝牙得到的数据显示到串口监视器
  }
    
  if (X >= 'A' && X <= 'D') {       // 检查是否为类型1值（"'A','B','C','D'"）
      Mode = X;                     // 存储类型1值到Mode变量
      return;                       // 结束当前循环，下一轮将使用新的Mode值
  }
    

  if (X >= 2 && X <= 8) {           // 检查是否为类型2值（后2,左4,右6,前8）
      if (X != lastType2Value) {    // 检查是否与上一次类型2值相同
          lastType2Value = X;       // 不同值，更新lastType2Value
      }
      executeType2Function();       // 持续执行当前类型2值的功能
  } 
  else {                            // 无效值，重置lastType2Value
      lastType2Value = 0;
  }
  X = 0;
}

// 根据当前模式执行类型2功能
void executeType2Function() {
    // 根据Mode值确定当前模式
    switch (Mode) {
        case 'A':  // 模式walk1
            switch (X) {
                case 8:
                    walk1_straight();// 模式1下类型2值5的功能
                    break;
                case 4:
                    walk1_left();// 模式1下类型2值6的功能
                    break;
                case 6:
                    walk1_right();// 模式1下类型2值7的功能
                    break;
                case 2:
                    // 模式1下类型2值8的功能
                    break;
            }
            break;
        case 'B':  // 模式walk2
            switch (X) {
                case 8:
                    walk2_straight();// 模式2下类型2值5的功能
                    break;
                case 4:
                    // 模式2下类型2值6的功能
                    break;
                case 6:
                    // 模式2下类型2值7的功能
                    break;
                case 2:
                    // 模式2下类型2值8的功能
                    break;
            }
            break;
        case 'C':  // 模式swim
            switch (X) {
                case 8:
                    swim1_straight();// 模式3下类型2值5的功能
                    break;
                case 4:
                    // 模式3下类型2值6的功能
                    break;
                case 6:
                    // 模式3下类型2值7的功能
                    break;
                case 2:
                    // 模式3下类型2值8的功能
                    break;
            }
            break;
        case 'D':  // 模式4
            switch (X) {
                case 8:
                    swim2_straight();// 模式4下类型2值5的功能
                    break;
                case 4:
                    // 模式4下类型2值6的功能
                    break;
                case 6:
                    // 模式4下类型2值7的功能
                    break;
                case 2:
                    // 模式4下类型2值8的功能
                    break;
            }
            break;
        default:
            // 无效模式，使用默认模式1
            Mode = 'A';
            break;
    }
}



void moveServos(const uint8_t* channels, const float* startPos, const float* endPos, int count, const float* step, int delayMs) {
  float pos[10];
  
  // Read data from PROGMEM
  for (int i = 0; i < count; i++) {
    pos[i] = pgm_read_float_near(startPos + i);
  }
  
  bool condition = true;
  while (condition) {
    condition = false;
    for (int i = 0; i < count; i++) {
      float start = pgm_read_float_near(startPos + i);
      float end = pgm_read_float_near(endPos + i);
      float stepVal = pgm_read_float_near(step + i);
      uint8_t channel = pgm_read_byte_near(channels + i);
      
      if (start < end ? pos[i] <= end : pos[i] >= end) {
        pwm.setPWM(channel, 0, map(pos[i], 0, 180, servoMin, servoMax));
        pos[i] += start < end ? stepVal : -stepVal;
        condition = true;
      }
    }
    delay(delayMs);
  }}

// Helper function to set initial positions from PROGMEM
void setInitialPositions(const uint8_t* initialPos, int count) {
  for (int i = 0; i < count; i++) {
    uint8_t pos = pgm_read_byte_near(initialPos + i);
    pwm.setPWM(i, 0, map(pos, 0, 180, servoMin, servoMax));
  }
}

void testServo(int channel){
  /*while(1){
    for(int i = 0; i<18; i++){
      pwm.setPWM(channel, 0, map(10*i, 0, 180, servoMin, servoMax));
      delay(1000);
    }
  }*/
  pwm.setPWM(channel, 0, map(0, 0, 180, servoMin, servoMax));
  delay(1000);
}

/*void walk1_straight(){
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

}*/

void walk1_straight(){

  setInitialPositions(initialPos_walk1_straight, 10);

  moveServos(channels_walk1_straight_1, startPos_walk1_straight_1, endPos_walk1_straight_1, 5, step_walk1_straight_1, 5);
  moveServos(channels_walk1_straight_2, startPos_walk1_straight_2, endPos_walk1_straight_2, 4, step_walk1_straight_2, 5);
  moveServos(channels_walk1_straight_3, startPos_walk1_straight_3, endPos_walk1_straight_3, 5, step_walk1_straight_3, 5);
  moveServos(channels_walk1_straight_4, startPos_walk1_straight_4, endPos_walk1_straight_4, 5, step_walk1_straight_4, 5);
  moveServos(channels_walk1_straight_5, startPos_walk1_straight_5, endPos_walk1_straight_5, 4, step_walk1_straight_5, 5);
  moveServos(channels_walk1_straight_6, startPos_walk1_straight_6, endPos_walk1_straight_6, 5, step_walk1_straight_6, 5);

}

void walk2_straight(){
  moveServos(channels_walk2_straight_1, startPos_walk2_straight_1, endPos_walk2_straight_1, 4, step_walk2_straight_1, 5);
  moveServos(channels_walk2_straight_2, startPos_walk2_straight_2, endPos_walk2_straight_2, 2, step_walk2_straight_2, 5);
  moveServos(channels_walk2_straight_3, startPos_walk2_straight_3, endPos_walk2_straight_3, 4, step_walk2_straight_3, 5);
  moveServos(channels_walk2_straight_4, startPos_walk2_straight_4, endPos_walk2_straight_4, 6, step_walk2_straight_4, 5);
}

void swim1_straight(){
  moveServos(channels_swim1_straight_1, startPos_swim1_straight_1, endPos_swim1_straight_1, 6, step_swim1_straight_1, 25);
  moveServos(channels_swim1_straight_2, startPos_swim1_straight_2, endPos_swim1_straight_2, 8, step_swim1_straight_2, 25);
  moveServos(channels_swim1_straight_3, startPos_swim1_straight_3, endPos_swim1_straight_3, 8, step_swim1_straight_3, 25);
  moveServos(channels_swim1_straight_4, startPos_swim1_straight_4, endPos_swim1_straight_4, 6, step_swim1_straight_4, 25);
}

void swim2_straight(){

  setInitialPositions(initialPos_swim2_straight, 10);

  moveServos(channels_swim2_straight_1, startPos_swim2_straight_1, endPos_swim2_straight_1, 4, step_swim2_straight_1, 25);
  moveServos(channels_swim2_straight_2, startPos_swim2_straight_2, endPos_swim2_straight_2, 4, step_swim2_straight_2, 25);
  moveServos(channels_swim2_straight_3, startPos_swim2_straight_3, endPos_swim2_straight_3, 4, step_swim2_straight_3, 25);
  moveServos(channels_swim2_straight_4, startPos_swim2_straight_4, endPos_swim2_straight_4, 6, step_swim2_straight_4, 25);
  moveServos(channels_swim2_straight_5, startPos_swim2_straight_5, endPos_swim2_straight_5, 6, step_swim2_straight_5, 25);
  moveServos(channels_swim2_straight_6, startPos_swim2_straight_6, endPos_swim2_straight_6, 8, step_swim2_straight_6, 25);

}

void walk1_left(){

  setInitialPositions(initialPos_walk1_left, 10);

  moveServos(channels_walk1_left_1, startPos_walk1_left_1, endPos_walk1_left_1, 2, step_walk1_left_1, 5);
  moveServos(channels_walk1_left_2, startPos_walk1_left_2, endPos_walk1_left_2, 3, step_walk1_left_2, 5);
  moveServos(channels_walk1_left_3, startPos_walk1_left_3, endPos_walk1_left_3, 3, step_walk1_left_3, 5);
  moveServos(channels_walk1_left_4, startPos_walk1_left_4, endPos_walk1_left_4, 3, step_walk1_left_4, 5);
  moveServos(channels_walk1_left_5, startPos_walk1_left_5, endPos_walk1_left_5, 1, step_walk1_left_5, 5);
  moveServos(channels_walk1_left_6, startPos_walk1_left_6, endPos_walk1_left_6, 2, step_walk1_left_6, 5);

}

void walk1_right(){

  setInitialPositions(initialPos_walk1_right, 10);

  moveServos(channels_walk1_right_1, startPos_walk1_right_1, endPos_walk1_right_1, 3, step_walk1_right_1, 5);
  moveServos(channels_walk1_right_2, startPos_walk1_right_2, endPos_walk1_right_2, 1, step_walk1_right_2, 5);
  moveServos(channels_walk1_right_3, startPos_walk1_right_3, endPos_walk1_right_3, 2, step_walk1_right_3, 5);
  moveServos(channels_walk1_right_4, startPos_walk1_right_4, endPos_walk1_right_4, 2, step_walk1_right_4, 5);
  moveServos(channels_walk1_right_5, startPos_walk1_right_5, endPos_walk1_right_5, 3, step_walk1_right_5, 5);
  moveServos(channels_walk1_right_6, startPos_walk1_right_6, endPos_walk1_right_6, 3, step_walk1_right_6, 5);

}

/*void moveint(){
  for (int i = 0; i < 10; i++) {
    pwm.setPWM(i, 0, map(initialPos[i], 0, 90, servoMin, servoMax));
  }
}*/