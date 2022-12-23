
#include "ADS1X15.h"
ADS1115 ADS(0x48);

const byte digit_pattern[10] =
{
  // 74HC595 Outpin Connection with 7segment display.
  // Q0 Q1 Q2 Q3 Q4 Q5 Q6 Q7
  // a  b  c  d  e  f  g  DP
  0b11111100,  // 0
  0b01100000,  // 1
  0b11011010,  // 2
  0b11110010,  // 3
  0b01100110,  // 4
  0b10110110,  // 5
  0b10111110,  // 6
  0b11100000,  // 7
  0b11111110,  // 8
  0b11110110,  // 9
  
};
/*
 * RESET 
 * SCL A5 | 28
 * SDA A4 | 27
 * SER A2
 * LATCH A3
 * CLK A1 | 23
 * INLET 0 | 2
 * STOP 2 | 4
 * OUTLET 1
 * UP 6
 * DOWN 7
 * START 8
 * ADC 9
 */

int latchPin = A3;                                  //Pin connected to ST_CP of 74HC595 pin 12
int clkPin = A1;                                   //Pin connected to SH_CP of 74HC595 pin 11
int dtPin = A2;                                    //Pin connected to DS of 74HC595 pin 14

int startButton = 8;                               //start
int downButton = 7;                                //subtract
int upButton = 6;                                  //add 
int stopButton = 2;                                //stop

int tensPSI = 3;                                             //variables to update display
int onesPSI = 5;
int targetPSI = 35;                                          //variable to store what the PSI is being set to

int *tPSI;
int *oPSI;
int *tarPSI;

volatile bool pressed = false;

int sensor = 9;                                    //sensor 
int valve_in = 0;
int valve_out = 1;

int sensorValue;                                           //variable to store raw sensor data

void setup() {
  pinMode(valve_in,OUTPUT);  
  pinMode(valve_out,OUTPUT);
  pinMode(sensor, OUTPUT);        
  
  digitalWrite(valve_in,LOW);
  digitalWrite(valve_out,LOW);
  digitalWrite(sensor,LOW);
  
   
  pinMode(latchPin, OUTPUT);    //ST_CP of 74HC595
  pinMode(clkPin, OUTPUT);      //SH_CP of 74HC595
  pinMode(dtPin, OUTPUT);       //DS of 74HC595
    
  pinMode(upButton,INPUT);
  pinMode(downButton,INPUT);
  pinMode(startButton,INPUT);
  pinMode(stopButton,INPUT);

  digitalWrite(latchPin, LOW);
  shiftOut(dtPin, clkPin, LSBFIRST, digit_pattern[5]);   
  shiftOut(dtPin, clkPin, LSBFIRST, digit_pattern[3]);
  digitalWrite(latchPin, HIGH);  
  attachInterrupt(digitalPinToInterrupt(stopButton),Stop,FALLING);


  tPSI = &tensPSI;
  oPSI = &onesPSI;
  tarPSI = &targetPSI;  
}

void(* resetFunc) (void) = 0;//declare reset function at address 0
int readSensor(){                                                // function that returns PSI 
    digitalWrite(sensor,HIGH);
    delay(5000);
    Serial.begin(115200);                                                 //initialize serial communication at 9600 bits per second:      
    ADS.begin();
    ADS.setGain(1);      // 4.096 volt
    ADS.setDataRate(7);  // fast
    ADS.setMode(0);      // continuous mode
    ADS.readADC(0);      // read a0 
    int psi = ADS.getValue();
    //do stuff
    digitalWrite(sensor,LOW);
 //   sensorValue = analogRead(sensor);                            // read the input on analog pin 0:
//    float voltage = sensorValue * (aref_voltage / 1023.0);       // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  //  int rounding = (voltage * 100) + .5;                         // to round from x.xxxxxx precision to .1, store in INT variable: (x.xxxx * 100) + .05),  
   // float v = (rounding) / 100.0;                                // which will move decimal over to xx (+.5 insures INT rounds up), then in another float variable, (xx / 100.0 ) to move decimal back over : x.x
           
    
 //   float pa =   ( (v - 0.50) / (3.3335) )  * 1000000 ;
 //   float converting = pa * (.000145161);
 //   psi = converting + .5 ;

     
    return psi;
}

void startInflation(){
 
  bool stopState = digitalRead(stopButton);
  while (stopState != pressed){                         // check stop state at multiple stages to ensure immediate stopping 
      stopState = digitalRead(stopButton);              
      digitalWrite(valve_in,LOW);                       //sequence opens from tank,  
      digitalWrite(valve_out,HIGH);                     //and closes exhaust
      stopState= digitalRead(stopButton);
      if (stopState == pressed){
        digitalWrite(valve_in,HIGH);                  // sequence closes tank 
        digitalWrite(valve_out,HIGH);                 // and exhaust
        break;                                        //back to main function                              
      }
      delay(100);
     }
}

void startDeflation(){
  bool stopState = digitalRead(stopButton);
  while (stopState != pressed){
      digitalWrite(valve_in,HIGH);
      digitalWrite(valve_out,LOW);
      stopState= digitalRead(stopButton);
      if (stopState == pressed){
        digitalWrite(valve_in,HIGH);
        digitalWrite(valve_out,HIGH);
        break;
      }
      delay(100);
     } 
}
//

void Stop(){
  digitalWrite(valve_in,LOW);                       //closes from tank,  
  digitalWrite(valve_out,LOW);                     //and closes exhaust
  
  digitalWrite(latchPin, LOW);
  shiftOut(dtPin, clkPin, LSBFIRST, digit_pattern[*oPSI]);   
  shiftOut(dtPin, clkPin, LSBFIRST, digit_pattern[*tPSI]);
  digitalWrite(latchPin, HIGH);
 }

void loop() {
  bool upState = digitalRead(upButton);                               //check status of buttons
  bool downState = digitalRead(downButton);
  bool startState = digitalRead(startButton);
//
  if(upState == pressed){                                    //add to target pressure
    if (onesPSI != 9 or tensPSI !=9){                       //highest possible diplay 99 -> if 99, do nothing
      *tarPSI += 1;                                       //update target PSI
      *tPSI = *tarPSI / 10 ;                            //extract tens value to push onto shift register
      *oPSI = *tarPSI - (*tPSI * 10) ;                //extract ones value to push onto shift register
//    
      digitalWrite(latchPin,LOW);                      //low pin allows writing to shift register
      shiftOut(dtPin,clkPin,LSBFIRST,digit_pattern[*oPSI]); //push ones 
      shiftOut(dtPin,clkPin,LSBFIRST,digit_pattern[*tPSI]); //push tens
      digitalWrite(latchPin,HIGH);                     //high pin closes writing to shift register
      delay(180);                                           //180ms delay
    }
  }
  else if (downState == pressed){                            //subtract from target pressure     
    if (onesPSI != 0 or tensPSI !=0){                       //lowest possible display 00 -> if 00, do nothing
      *tarPSI -= 1;                                       //update target PSI
      *tPSI = *tarPSI / 10 ;                            //extract tens value to push onto shift register
      *oPSI = *tarPSI - (*tPSI * 10) ;                //extract ones value to push onto shift register
      
      digitalWrite(latchPin,LOW);                      //active low pin allows writing to shift register
      shiftOut(dtPin, clkPin,LSBFIRST,digit_pattern[*oPSI]); //push ones 
      shiftOut(dtPin, clkPin,LSBFIRST,digit_pattern[*tPSI]); //push tens
      digitalWrite(latchPin,HIGH);                     //active high pin closes writing to shift register
      delay(180);                                           //180ms delay 
    }
  }
  else if (startState == pressed){
    int pressure = 40; //readSensor();                            //READ SENSOR
    do {                             //IF SENSOR DATA BELOW TARGET -> START INFLATION      
      digitalWrite(valve_in,HIGH);                       //sequence opens from tank,  
      digitalWrite(valve_out,LOW);                     //and closes exhaust
      delay(2000);
      digitalWrite(valve_in,LOW);                       //closes from tank,  
      digitalWrite(valve_out,LOW);                     //and closes exhaust
  
      pressure = 60;
    } while (pressure < *tarPSI);
      *tPSI = (pressure / 10);                            //extract tens value to push onto shift register
      *oPSI = pressure - (*tPSI * 10);                //extract ones value to push onto shift register
      digitalWrite(latchPin,LOW);                      //active low pin allows writing to shift register
      shiftOut(dtPin, clkPin,LSBFIRST,digit_pattern[*oPSI]); //push ones 
      shiftOut(dtPin, clkPin,LSBFIRST,digit_pattern[*tPSI]); //push tens
      digitalWrite(latchPin,HIGH);                     //active high pin closes writing to shift register
      delay(180);
  }         
}
