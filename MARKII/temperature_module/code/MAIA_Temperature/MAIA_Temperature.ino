/*
      
      ███╗   ███╗ █████╗ ██╗ █████╗
      ████╗ ████║██╔══██╗██║██╔══██╗
      ██╔████╔██║███████║██║███████║
      ██║╚██╔╝██║██╔══██║██║██╔══██║
      ██║ ╚═╝ ██║██║  ██║██║██║  ██║
      ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝
      
      ╔═╗┬┌┬┐┬ ┬  ╔═╗┌─┐┬┌─┐┌┐┌┌─┐┌─┐       ╔╦╗╦╔╦╗  ╔╦╗┌─┐┌┬┐┬┌─┐  ╦  ┌─┐┌┐
      ║  │ │ └┬┘  ╚═╗│  │├┤ ││││  ├┤   ───  ║║║║ ║   ║║║├┤  │││├─┤  ║  ├─┤├┴┐
      ╚═╝┴ ┴  ┴   ╚═╝└─┘┴└─┘┘└┘└─┘└─┘       ╩ ╩╩ ╩   ╩ ╩└─┘─┴┘┴┴ ┴  ╩═╝┴ ┴└─┘

      Modular, low-cost and portable bio reactor. 

      Temperature Module Firmware.
      Andres Rico - aricom@mit.edu
 */

#include <Wire.h>

//Definitions for thermistor temperature option. 
#define SERIESRESISTOR 10000
#define THERMISTORPIN 3
#define TEMPERATURENOMINAL 25
#define BCOEFFICIENT 3950   
#define THERMISTORNOMINAL 10000

int led1 = 9;
int led2 = 8;
int peltier = 1;

//float states[] = {};
int x = 0;

float temp_average = 0;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial.println("Hello MAIA Temperature Module");
  
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(peltier, OUTPUT);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(peltier, LOW);

  Wire.begin(3);
  Wire.onReceive(receiveEvent);
  Serial.println("Ready To Begin!");
}

void loop() {
  //Serial.println(Wire.read());

  while (temp_average < x) {
  
    digitalWrite(peltier, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led1, LOW);
    get_temperature();
    
  }
  digitalWrite(peltier, LOW);
  digitalWrite(led1, HIGH);
  digitalWrite(led2, LOW);
  get_temperature();
  
}

void receiveEvent(int bytes) {
  x = Wire.read();
  Serial.print("Received Target Temperature of: ");
  Serial.println(x);
}

void get_temperature() {

  //Thermistor Temperature
  temp_average = 0;
  for (int i =0; i < 50 ; i++) {

    float therm_reading;
 
    therm_reading = analogRead(THERMISTORPIN);
   
    // convert the value to resistance
    therm_reading = (1023 / therm_reading)  - 1;     // (1023/ADC - 1) 
    therm_reading = SERIESRESISTOR / therm_reading;  // 10K / (1023/ADC - 1)
  
    float thermTemp;
    thermTemp = therm_reading / THERMISTORNOMINAL;     // (R/Ro)
    thermTemp = log(thermTemp);                  // ln(R/Ro)
    thermTemp /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
    thermTemp += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
    thermTemp = 1.0 / thermTemp;                 // Invert
    thermTemp -= 273.15; // convert absolute temp to C

    temp_average = temp_average + thermTemp;
    
  }

  temp_average = temp_average / 50;
  Serial.print("Current Culture Temperature: ");
  Serial.println(temp_average);
  
}
