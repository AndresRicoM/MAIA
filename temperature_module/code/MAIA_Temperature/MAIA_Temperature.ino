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

#include <Wire.h> //I2C Communication
#include <EEPROM.h> //EEPROM access library.

//Definitions for thermistor temperature option. 
#define SERIESRESISTOR 10000
#define THERMISTORPIN 3
#define TEMPERATURENOMINAL 25
#define BCOEFFICIENT 3950   
#define THERMISTORNOMINAL 10000

int led1 = 9; //Blue LED
int led2 = 8; //Red LED
int peltier = 1; //Peltier pin. 

int x = 0; //Variable for storing incoming i2c message. 

float temp_average = 0; //Float for stroing temperature sensoir value. 

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

  x = EEPROM.read(0); //Check eproom to turn on last value of temperature that was sent. Essential for when power fluctuates on chip.

  Wire.begin(3);
  Wire.onReceive(receiveEvent);
  Serial.println("Ready To Begin!");
}

void loop() {
  //Serial.println(Wire.read());

  while (temp_average < x) { //Peltier will heat up anytime that temperature is below target. 
  
    digitalWrite(peltier, HIGH);
    digitalWrite(led2, HIGH); //Red led on for heating up. 
    digitalWrite(led1, LOW);
    get_temperature();
    
  }
  digitalWrite(peltier, LOW);
  digitalWrite(led1, HIGH); //Blue led on when temperature is correct or reactor is cooling down. 
  digitalWrite(led2, LOW);
  get_temperature();
  
}

void receiveEvent(int bytes) { //Runs when i2c receives message. 
  
  x = Wire.read(); //Reads i2c message. 
  Serial.print("Received Target Temperature of: ");
  Serial.println(x);

  EEPROM.write(0,x); //Sets new value on internal memory in case module restarts. 
}

void get_temperature() { //Oversamples NTC analog pin to get average of temperature. 

  //Thermistor Temperature
  temp_average = 0;
  for (int i =0; i < 50 ; i++) { //50 samples per average. 

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

  temp_average = temp_average / 50; //Define temperature value. 
  Serial.print("Current Culture Temperature: ");
  Serial.println(temp_average);
  
}
