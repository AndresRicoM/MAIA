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

      Mother Module Firmware.
      Andres Rico - aricom@mit.edu
 */

#include <WiFi.h>
#include <Wire.h>
#include <WiFiUdp.h>


//WIFI Variables
const char* ssid = "MIT"; //Change accordingly to connect to a WIFi network. 
const char* password = "";

//UDP Variables
WiFiUDP Udp; //UDP instance

char packetBuffer[255]; //Buffer to store UDP packets. 

unsigned int port = 1206; //Port chosen arbitrarily. 

///////Includes main modules addresses. Add hacked modules here. 

int temperature_board = 3;  //T[Data]
int power_board = 4;        //P[Data]
int light_board = 5;        //L[Data]

int led = 32; //Debugging Green LED.

int desired_temperature = 0; //Desired temperature in Celcius. Current version's minimum temp will be ambient temperature. 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("Hello, I'm MAIA!");
  
  //WiFi.mode(WIFI_STA); //Optional
  WiFi.begin(ssid, password);
  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial

  Wire.begin();
  Serial.println("Enabled I2C Communication @ Address: 0x00");

  Udp.begin(port);
  Serial.print("Opened UDP Communication @ Port: ");
  Serial.println(port);
  
  pinMode(led, OUTPUT);  
  

}

void loop() {
  //// put your main code here, to run repeatedly:
  getUDP();
  
  
}

void getUDP() {
  
  int packetSize = Udp.parsePacket();

  if (packetSize) {

    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());
    
    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, 255);
    
    if (len > 0) {
      packetBuffer[len] = 0;
    }

    Serial.println("Contents:");
    Serial.println(packetBuffer);

    if (packetBuffer[0] == 'T') { //Checks if Commans is comming to change temperature.
      
      Serial.println("Received New Temperature Command");
      desired_temperature = ((packetBuffer[1] - '0') * 10) + ((packetBuffer[2] - '0'));

      Serial.print("New Desired Temperature Set @ ");
      Serial.println(desired_temperature);
      
      digitalWrite(led, HIGH);
      int message = desired_temperature;
      Wire.beginTransmission(temperature_board);
      Wire.write(message);
      Wire.endTransmission();
      delay(500);
      digitalWrite(led, LOW);
      delay(500);
      
    }

    else if (packetBuffer[0] == 'L') { //Checks if command is comming to change Light.
      
      Serial.println("Received New Light Command");

      switch (packetBuffer[1] - '0') { 
        
        case 1: { //Red Light
          Serial.println("Turning Red Light On");
          digitalWrite(led, HIGH);
          int message = packetBuffer[1] - '0';
          Wire.beginTransmission(light_board);
          Wire.write(message);
          Wire.endTransmission();
          delay(500);
          digitalWrite(led, LOW);
          delay(500);
          }
          break;
          

        case 2: { //Green Light
          Serial.println("Turning Green Light On");
          digitalWrite(led, HIGH);
          int message = packetBuffer[1] - '0';
          Wire.beginTransmission(light_board);
          Wire.write(message);
          Wire.endTransmission();
          delay(500);
          digitalWrite(led, LOW);
          delay(500);
          }
          break;
        

        case 3: { //Blue Light
          Serial.println("Turning Blue Light On");
          digitalWrite(led, HIGH);
          int message = packetBuffer[1] - '0';
          Wire.beginTransmission(light_board);
          Wire.write(message);
          Wire.endTransmission();
          delay(500);
          digitalWrite(led, LOW);
          delay(500);
          }
          break;

        case 4: { //White Light
          Serial.println("Turning White Light On");
          digitalWrite(led, HIGH);
          int message = packetBuffer[1] - '0';
          Wire.beginTransmission(light_board);
          Wire.write(message);
          Wire.endTransmission();
          delay(500);
          digitalWrite(led, LOW);
          delay(500);
          }
          break;

         case 5: { //Lights Off
          Serial.println("Turning Lights Off");
          digitalWrite(led, HIGH);
          int message = packetBuffer[1] - '0';
          Wire.beginTransmission(light_board);
          Wire.write(message);
          Wire.endTransmission();
          delay(500);
          digitalWrite(led, LOW);
          delay(500);
          }
          break;
      }    
    }

    else {
      Serial.println("Packet Error");
    }
  }
}
