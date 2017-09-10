//#define DEBUG

#define IN1 2
#define IN2 3
#define RAIN_DIGITAL 7            // RainSensor

unsigned long pre = 0;
bool isRain = false; // false가 Norain, true가 rain
int rainControl = 0;
void WindowControl(char *order);
#ifdef DEBUG
const int rainLed = 4;
const int sunnyLed = 5;
#endif

void setup() {
  pinMode(IN1,OUTPUT);            // MotorDriver
  pinMode(IN2,OUTPUT);
  pinMode(RAIN_DIGITAL,INPUT);
  #ifdef DEBUG
  Serial.begin(9600);
  pinMode(rainLed,OUTPUT);
  pinMode(sunnyLed,OUTPUT);
  #endif
}

void loop() {
  rainControl = digitalRead(RAIN_DIGITAL);  // rainControl 맑을때 1, 비올때 0
  if(rainControl == isRain){
    pre = millis();
    if(isRain == true){
      #ifdef DEBUG
      digitalWrite(rainLed,HIGH);
      digitalWrite(sunnyLed,LOW);
      #endif
      isRain = false;
      while(millis()-pre<=2200)
        WindowControl("open");
    }
    else if(isRain == false){
      #ifdef DEBUG
      digitalWrite(rainLed,LOW);
      digitalWrite(sunnyLed,HIGH);
      #endif
      isRain = true;
      while(millis()-pre<=2200)
        WindowControl("close");
    }
    WindowControl("stop");  
  }
  #ifdef DEBUG
  digitalWrite(rainLed,LOW);
  digitalWrite(sunnyLed,LOW);
  #endif
}

void WindowControl(char *order){  // 문 여닫는거
  if(order=="open"){
    digitalWrite(IN1,1);
    digitalWrite(IN2,0);
  }
  else if(order=="close"){
    digitalWrite(IN1,0);
    digitalWrite(IN2,1);
  }
  else if(order=="stop"){
    digitalWrite(IN1,0);
    digitalWrite(IN2,0);
  }
}
