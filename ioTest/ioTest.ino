//#define MOTOR_DRIVER
#define DUST_SENSOR
//#define RAIN_SENSOR
#define DEBUG

#define MOTOR_CONTROL A0          // MotorDriver
#define IN1 2
#define IN2 3
#define DUST_VALUE A1             // DustSensor
#define DUST_CONTROL 4
#define RAIN_ANALOG A2            // RainSensor
#define RAIN_DIGITAL 5

const int samplingTime = 280;     // DustSensor
const int deltaTime = 40;
const int sleepTime = 9680;
float dustValue = 0;
float calcVoltage = 0;
float dustDensity = 0;

void WindowState(char *order);
int MeasureDust();

void setup() {
  pinMode(IN1,OUTPUT);            // MotorDriver
  pinMode(IN2,OUTPUT);            
  pinMode(MOTOR_CONTROL,INPUT);   
  pinMode(DUST_CONTROL,OUTPUT);   // DustSensor
  pinMode(DUST_VALUE,INPUT);    
  pinMode(RAIN_DIGITAL,OUTPUT);   // ## IN 아님?
  Serial.begin(9600);
}

void loop() {
  #ifdef MOTOR_DRIVER
  int motorVal = analogRead(MOTOR_CONTROL);
  #ifdef DEBUG
  Serial.print(motorVal);
  #endif
  if(motorVal<=342){        // 0~1.67V
    WindowState("close");
    #ifdef DEBUG
    Serial.println(" - close");
    #endif
  }
  else if(motorVal<=684 ){  // 1.67~3.34V
    WindowState("stop");
    #ifdef DEBUG
    Serial.println(" - stop");
    #endif
  }
  else{                                      // 3.34~5V
    WindowState("open");
    #ifdef DEBUG
    Serial.println(" - open");
    #endif
  }
  
  #endif

  #ifdef DUST_SENSOR 
  dustValue = MeasureDust();
  
  calcVoltage = dustValue * 5. / 1024.;
  dustDensity = 0.17 * calcVoltage - 0.1;
  #ifdef DEBUG
  Serial.print("Raw Signal Value (0-1023): ");
  Serial.print(dustValue);
  Serial.print(" - Voltage: ");
  Serial.print(calcVoltage);
  Serial.print(" - Dust Density: ");
  Serial.println(dustDensity); // unit: mg/m3
  #endif
  #endif

  #ifdef RAIN_SENSOR
  //analog output
  int rain = analogRead(RAIN_ANALOG);
  Serial.print("Raw Rain Val (0-1023): ");
  Serial.print(rain);
  if(rain<300)
    #ifdef DEBUG
    Serial.println(" - Rain");
    #endif
  else
    #ifdef DEBUG
    Serial.println(" - No Rain"); 
    #endif
  
  // //digital output
  // if(digitalRead(2) == HIGH) Serial.println("No Rain Detected");
  // else Serial.println("Rain Detected"); 
  //delay(250);
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

void WindowState(char *order){
  if(order=="open"){
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
  }
  else if(order=="close"){
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
  }
  else if(order=="stop"){
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
  }
}
