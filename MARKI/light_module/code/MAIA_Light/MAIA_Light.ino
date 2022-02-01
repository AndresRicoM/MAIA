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

      Light Module Firmware.
      Andres Rico - aricom@mit.edu
 */
 
#include <Wire.h> //I2C Communication
#include <EEPROM.h> //EEPROM access library.

int rled = 10; //Output Pins on ATTINY 1614
int gled = 0;
int bled = 1;

int x = 0; //Variable for storing incoming i2c message. 

float temp_average = 0; //Float for stroing temperature sensoir value. 

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial.println("Hello MAIA Light Module");
  
  pinMode(rled, OUTPUT); //Sets LED arrays as outputs. 
  pinMode(gled, OUTPUT);
  pinMode(bled, OUTPUT);
  
  digitalWrite(rled, LOW); 
  digitalWrite(gled, LOW);
  digitalWrite(bled, LOW);

  color_stimulation(EEPROM.read(0)); //Check eproom to turn on last value of light that was sent. Essential for when power fluctuates on chip. 

  Wire.begin(5); //I2C address 5. Same address should be set on mother module. 
  Wire.onReceive(receiveEvent); //Listening event for i2c communication. 
  
  Serial.println("Ready To Begin!");
}

void loop() {
  
}

void receiveEvent(int bytes) { //Runs when i2c receives message. 
  
  x = Wire.read(); //Reads i2c message. 
  Serial.print("Received Light Color of: ");
  Serial.println(x);
  
  EEPROM.write(0,x); //Sets new value on internal memory in case module restarts. 
  color_stimulation(x);
    
  }

void color_stimulation(int command) { //Chooses what color to turn on depending on the received command. 

  switch (command) { //Commands are sent by mother module.

    case 1: {
      Serial.print("Turning Red Light On");
      redon();
    } break;

    case 2: {
      Serial.print("Turning Green Light On");
      greenon();
    } break;

    case 3: {
      Serial.print("Turning Blue Light On");
      blueon();
    } break;

    case 4: {
      Serial.print("Turning White Light On");
      whiteon();
    }

    case 5: {
      Serial.print("Turning All Lights Off");
      lights_off();
    }
  }
}

void redon() {
  digitalWrite(rled, HIGH);
  digitalWrite(gled, LOW);
  digitalWrite(bled, LOW);
}

void greenon() {
  digitalWrite(rled, LOW);
  digitalWrite(gled, HIGH);
  digitalWrite(bled, LOW);
}

void blueon() {
  digitalWrite(rled, LOW);
  digitalWrite(gled, LOW);
  digitalWrite(bled, HIGH);
}

void whiteon() {
  digitalWrite(rled, HIGH);
  digitalWrite(gled, HIGH);
  digitalWrite(bled, HIGH);
}


void lights_off() {
  digitalWrite(rled, LOW);
  digitalWrite(gled, LOW);
  digitalWrite(bled, LOW);
}
