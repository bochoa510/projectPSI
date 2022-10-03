# projectPSI ; Automatic Tire Inflator

arduino code  : mainControl.ino 

schematic & layout files : pPSI.zip

# Materials
1: Stepdown transformer 120V -> 24V (https://www.digikey.com/en/products/detail/triad-magnetics/TCT40-01E07K/666127)

2: 24V AC N/C Solenoid Valve 1/2" (https://www.electricsolenoidvalves.com/1-2-24v-ac-electric-brass-solenoid-valve/)

2: AC->DC converter, 5V (https://www.amazon.com/gp/product/B07V5XP92F/ref=ppx_yo_dt_b_asin_title_o03_s00?ie=UTF8&psc=1)

 ![image](https://user-images.githubusercontent.com/65479298/192938053-ce143546-a3d1-410d-ae2a-fe97e1dd2196.png) 
 
    - C1: 47μF/25V electrolytic capacitor
    - C2: 1μF/50V ceramic capacitor (105)
    - D1: TVS 7A diode 

2: 5V relay modules (https://www.amazon.com/dp/B00LW15A4W?ref=nb_sb_ss_w_as-reorder-t1_ypp_rep_k0_1_6&amp=&crid=W0WI06CL6J4E&amp=&sprefix=5v+rel) 

1: Atmega328pu (https://www.digikey.com/en/products/detail/microchip-technology/ATMEGA328P-PU/1914589)

    - 2: 22pF ceramic capacitor (220)
    - 1: 16MHz crystal oscillator

1: 150 PSI Pressure Transducer (https://www.amazon.com/dp/B08G4YRZT8/ref=twister_B08G4XH5PK?_encoding=UTF8&psc=1)

1: ADS1115 16bit ADC board  (https://www.amazon.com/dp/B08HLWLR77?psc=1&ref=ppx_yo2ov_dt_b_product_details)

4: NPN Transistors 

2: 74HC595N Shift Register (https://www.digikey.com/en/products/detail/texas-instruments/SN74HC595N/277246)

    - 2: .1μF ceramic capacitor (104)

2: 1" 7 Segment LED Display (https://www.mouser.com/ProductDetail/604-SC10-21SRWA)


