/*
  ███╗   ███╗ █████╗ ██╗ █████╗
  ████╗ ████║██╔══██╗██║██╔══██╗
  ██╔████╔██║███████║██║███████║
  ██║╚██╔╝██║██╔══██║██║██╔══██║
  ██║ ╚═╝ ██║██║  ██║██║██║  ██║
  ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝

  Code for temperature control board for MAIA reactor. The device enables live cell systems to be deployed outside of the lab.
  This code control the peltier based heating system mounted on the base module.  

  NOTE: This code can only heat up the module. Do not use module if outside temperaturws are higher than 37C. 

  Andres Rico - MIT City Science Group - How To Grow Almost Anything 2021
 
 */


//Definitions for thermistor temperature option. 
#define SERIESRESISTOR 10000
#define THERMISTORPIN 1
#define TEMPERATURENOMINAL 25
#define BCOEFFICIENT 3950   
#define THERMISTORNOMINAL 10000 

//Declaring PINs
int greenLED = 9; //Green LED on when not within temperature range. 
int blueLED = 8; //Blue LED on when temperature is within specified temperature range. 
int hot = 7; //Controls peltier - When on hot side is on plate. 
int cold = 6; //Controls peltier - When on cold side is on plate. 

void setup() {

  Serial.begin(115200);
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(hot, OUTPUT);
  pinMode(cold, OUTPUT);

  for (int i = 1; i<5; i++) { //Blynk LEDs to show proper startup. 
    digitalWrite(greenLED, HIGH);
    digitalWrite(blueLED, HIGH);
    delay(100);
    digitalWrite(greenLED, LOW);
    digitalWrite(blueLED, LOW);
    delay(100);
    
  }

}

void loop() {
  
  //Thermistor Temperature
  float therm_reading;
 
  therm_reading = analogRead(THERMISTORPIN); //Read thermistor voltage. 
 
  // Convert the value to resistance
  therm_reading = (1023 / therm_reading)  - 1;     // (1023/ADC - 1) 
  therm_reading = SERIESRESISTOR / therm_reading;  // 10K / (1023/ADC - 1)

  float thermTemp;
  thermTemp = therm_reading / THERMISTORNOMINAL;     // (R/Ro)
  thermTemp = log(thermTemp);                  // ln(R/Ro)
  thermTemp /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  thermTemp += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  thermTemp = 1.0 / thermTemp;                 // Invert
  thermTemp -= 273.15; // convert absolute temp to C
  Serial.println(thermTemp); //Print out temperature.

  if (thermTemp < 37) { //Check if temperature is below 37. 
    digitalWrite(hot, HIGH);
    digitalWrite(greenLED, HIGH);
    digitalWrite(blueLED, LOW);
    Serial.println("Temperature is low, heating plate");
  } else {
    digitalWrite(hot, LOW);
    digitalWrite(greenLED, LOW);
    digitalWrite(blueLED, HIGH);
    Serial.println("Temperature is OK!");
  }
      
}
