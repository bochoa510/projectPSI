

#include "ADS1X15.h"

ADS1115 ADS(0x48);


int sensor = 9;                                    //sensor 
int valve_in = 0;
int valve_out = 1;


int startButton = 8;                               //start
int downButton = 7;                                //subtract
int upButton = 6;                                  //add 
int stopButton = 2;                                //stop

void setup() 
{
  pinMode(valve_in,OUTPUT);  
  pinMode(valve_out,OUTPUT);
  pinMode(sensor, OUTPUT);        
  
  digitalWrite(valve_in,LOW);
  digitalWrite(valve_out,LOW);
  digitalWrite(sensor,LOW);
  
  pinMode(upButton,INPUT);
  pinMode(downButton,INPUT);
  pinMode(startButton,INPUT);
  pinMode(stopButton,INPUT);

  attachInterrupt(digitalPinToInterrupt(stopButton),Stop,FALLING);
}
void Stop(){
  digitalWrite(valve_in,LOW);                       //closes from tank,  
  digitalWrite(valve_out,LOW);                     //and closes exhaust
}

int readSensor(){
  digitalWrite(sensor,HIGH);
  delay(1000);
  Serial.begin(115200);                                                 //initialize serial communication at 115200 bits per second:      
  delay(2000);
  ADS.begin();
  delay(1000);
  
  ADS.setGain(1); // max 4.096V , cannot exceed 130 PSI 
  ADS.setDataRate(5);
  int16_t val_0 = ADS.readADC(0); 
  float V = ADS.toVoltage(1);  // voltage factor
  float Voltage = (val_0 * V);
  int psi = (Voltage * 37.5) - 18.75;
  float fpsi = (Voltage * 37.5) - 18.75;

  Serial.print("voltage: "); Serial.println(Voltage);
  Serial.print("psi: "); Serial.print(psi); Serial.print("   "); Serial.println(fpsi,3);
  Serial.println();
  delay(1000);
}

void loop() 
{
 
}


// -- END OF FILE --
