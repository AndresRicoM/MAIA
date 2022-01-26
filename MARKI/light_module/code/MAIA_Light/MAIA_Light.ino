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
 
#include <Wire.h>

int rled = 10; //Output Pins on ATTINY 1614
int gled = 0;
int bled = 1;

//float states[] = {};
int x = 0;

float temp_average = 0;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial.println("Hello MAIA Light Module");
  
  pinMode(rled, OUTPUT);
  pinMode(gled, OUTPUT);
  pinMode(bled, OUTPUT);

  digitalWrite(rled, HIGH);
  digitalWrite(gled, HIGH);
  digitalWrite(bled, HIGH);

  Wire.begin(5);
  Wire.onReceive(receiveEvent);
  Serial.println("Ready To Begin!");
}

void loop() {
  
}

void receiveEvent(int bytes) {
  x = Wire.read();
  Serial.print("Received Light Color of: ");
  Serial.println(x);

  switch (x) {

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
