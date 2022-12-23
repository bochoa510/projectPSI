//
//    FILE: ADS_read.ino
//  AUTHOR: Rob.Tillaart
// VERSION: 0.2.1
// PURPOSE: read analog inputs - straightforward.
//

// test
// connect 1 potmeter per port.
//
// GND ---[   x   ]------ 5V
//            |
//
// measure at x (connect to AIN0).
//


#include "ADS1X15.h"

ADS1115 ADS(0x48);


void setup() 
{
  Serial.begin(115200);
  Serial.println(__FILE__);
  Serial.print("ADS1X15_LIB_VERSION: ");
  Serial.println(ADS1X15_LIB_VERSION);

  ADS.begin();
}


void loop() 
{
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


// -- END OF FILE --
