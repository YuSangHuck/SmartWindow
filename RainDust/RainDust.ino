#define DEBUG

#define IN1 2					  // Arduino send a signal to MotorDriver. digitalWrite(IN1,HIGH) means MotorDriver send HIGH to Motor
#define IN2 3					  // Arudino send a siganl to MotorDriver
#define RAIN_DIGITAL 7            // Read RainSensor Value(Digital)
#define DUST_VALUE A0             // Read DustSensor Value(Analog)
#define DUST_CONTROL 5			  // Control operating of DustSensor. LOW(start)->HIGH(stop)

unsigned long pre = 0;
bool isRain = false; // false가 Norain, true가 rain
int rainControl = 0;
const int samplingTime = 280;     // DustSensor
const int deltaTime = 40;
const int sleepTime = 9680;
float dustValue = 0;
float calcVoltage = 0;
float dustDensity = 0;
#ifdef DEBUG
const int rainLed = 13;
const int sunnyLed = 12;
const int dustLed = 11;
#endif

void WindowControl(char *order);
int MeasureDust();

void setup() {
  pinMode(IN1,OUTPUT);            // MotorDriver
  pinMode(IN2,OUTPUT);
  pinMode(RAIN_DIGITAL,INPUT);    // RainSensor
  pinMode(DUST_CONTROL,OUTPUT);   // DustSensor
  pinMode(DUST_VALUE,INPUT);    
  #ifdef DEBUG
  Serial.begin(9600);
  #endif
}

void loop() {
  /*
  rainControl = digitalRead(RAIN_DIGITAL);  // rainControl 맑을때 1, 비올때 0
  if(rainControl == isRain){
    pre = millis();
    if(isRain == true){
      #ifdef DEBUG
      digitalWrite(rainLed,HIGH);
      digitalWrite(sunnyLed,LOW);
      #endif
      isRain = false;
      while(millis()-pre<=2000)
        WindowControl("close");
    }
    else if(isRain == false){
      #ifdef DEBUG
      digitalWrite(rainLed,LOW);
      digitalWrite(sunnyLed,HIGH);
      #endif
      isRain = true;
      while(millis()-pre<=2000)
        WindowControl("open");
    }
    WindowControl("stop");  
  }
  #ifdef DEBUG
  digitalWrite(rainLed,LOW);
  digitalWrite(sunnyLed,LOW);
  #endif
  */
  
  
  dustValue = MeasureDust(); 
  calcVoltage = dustValue * 5. / 1024.;
  dustDensity = 0.17 * calcVoltage - 0.1;
  rainControl = digitalRead(RAIN_DIGITAL);  // rainControl 맑을때 1, 비올때 0
  #ifdef DEBUG
  Serial.print("RainSensor: ");
  if(rainControl==1) Serial.print("O\t");
  else Serial.print("/\t");
  Serial.print("\tDust Density: ");
  Serial.println(dustDensity); // unit: mg/m3(0~0.61)
  analogWrite(dustLed,(dustDensity+0.08)*418);
  #endif  
  
}

int MeasureDust(){
  int value = 0;
  digitalWrite(DUST_CONTROL,LOW);   // ### HIGH 아닌가?
  //digitalWrite(DUST_CONTROL,HIGH);   
  delayMicroseconds(samplingTime);
  value = analogRead(DUST_VALUE);
  delayMicroseconds(deltaTime);
  digitalWrite(DUST_CONTROL,HIGH);  // ### LOW 아닌가?
  //digitalWrite(DUST_CONTROL,LOW);  
  delayMicroseconds(sleepTime);

  return value;
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
