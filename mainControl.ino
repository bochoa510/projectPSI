
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

int latchPin = 4;                                  //Pin connected to ST_CP of 74HC595
int clkPin = 12;                                   //Pin connected to SH_CP of 74HC595
int dtPin = 14;                                    //Pin connected to DS of 74HC595


int startButton = 5;                               //start
int upButton = 19;                                  //add 
int downButton = 18;                                //subtract
int stopButton = 21;                                //stop

int tensPSI=3;                                             //variables to update display
int onesPSI=5;
int targetPSI=35;                                          //variable to store what the PSI is being set to



bool pressed = false;

int valve_in = 34;
int valve_out = 35;
int sensor = A0;                                    //sensor 
int sensorValue;                                           //variable to store raw sensor data

void setup() {
  Serial.begin(115200);                                                 //initialize serial communication at 9600 bits per second:      
  
  pinMode(valve_in,OUTPUT);  
  pinMode(valve_out,OUTPUT);  
  
  digitalWrite(valve_in,HIGH);
  digitalWrite(valve_out,HIGH);
     
  pinMode(latchPin, OUTPUT);    //ST_CP of 74HC595
  pinMode(clkPin, OUTPUT);      //SH_CP of 74HC595
  pinMode(dtPin, OUTPUT);       //DS of 74HC595

  pinMode(upButton, INPUT);
  pinMode(downButton, INPUT);
  pinMode(startButton, INPUT);
  pinMode(stopButton, INPUT);

  digitalWrite(latchPin, LOW);
  shiftOut(dtPin, clkPin, LSBFIRST, digit_pattern[5]);   
  shiftOut(dtPin, clkPin, LSBFIRST, digit_pattern[3]);
  digitalWrite(latchPin, HIGH);  
}

int readSensor(){                                                // function that returns PSI 
    int psi;
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



void loop() {
  bool upState = digitalRead(upButton);                               //check status of buttons
  bool downState = digitalRead(downButton);
  bool startState = digitalRead(startButton);
  bool stopState = digitalRead(stopButton);

  if      (upState == pressed){                                    //add to target pressure
  
    if (onesPSI != 9 or tensPSI !=9){                       //highest possible diplay 99 -> if 99, do nothing
      targetPSI += 1;                                       //update target PSI
      tensPSI = targetPSI / 10 ;                            //extract tens value to push onto shift register
      onesPSI = targetPSI - (tensPSI * 10) ;                //extract ones value to push onto shift register
    
      digitalWrite(latchPin,LOW);                      //low pin allows writing to shift register
      shiftOut(dtPin,clkPin,LSBFIRST,digit_pattern[onesPSI]); //push ones 
      shiftOut(dtPin,clkPin,LSBFIRST,digit_pattern[tensPSI]); //push tens
      digitalWrite(latchPin,HIGH);                     //high pin closes writing to shift register
      delay(180);                                           //180ms delay
    }
  }
  else if (downState == pressed){                            //subtract from target pressure
      
    if (onesPSI != 0 or tensPSI !=0){                       //lowest possible display 00 -> if 00, do nothing
      targetPSI -= 1;                                       //update target PSI
      tensPSI = targetPSI / 10 ;                            //extract tens value to push onto shift register
      onesPSI = targetPSI - (tensPSI * 10) ;                //extract ones value to push onto shift register
      
      digitalWrite(latchPin,LOW);                      //active low pin allows writing to shift register
      shiftOut(dtPin, clkPin,LSBFIRST,digit_pattern[onesPSI]); //push ones 
      shiftOut(dtPin, clkPin,LSBFIRST,digit_pattern[tensPSI]); //push tens
      digitalWrite(latchPin,HIGH);                     //active high pin closes writing to shift register
      delay(180);                                           //180ms delay 
    }
  }
  else if (startState == pressed){
    int pressure = readSensor();                            //READ SENSOR
    
    if (pressure < targetPSI){                             //IF SENSOR DATA BELOW TARGET -> START INFLATION
      
      while (stopState != pressed){                         // check stop state at multiple stages to ensure immediate stopping 
        stopState = digitalRead(stopButton);              
        digitalWrite(valve_in,LOW);                       //sequence opens from tank,  
        digitalWrite(valve_out,HIGH);                     //and closes exhaust
        stopState= digitalRead(stopButton);
        delay(3000);
        if (stopState == pressed){
          digitalWrite(valve_in,HIGH);                  // sequence closes tank 
          digitalWrite(valve_out,HIGH);                 // and exhaust
          break;                                        //back to main function                              
      }
      delay(100);
     }  
    }
    if (pressure > targetPSI){                             //IF SENSOR DATA ABOVE TARGET -> START DEFLATION
     // newPressure = startDeflation(targetPSI);
    }
  }
}
int main(){
  void();
  return 0;
}
