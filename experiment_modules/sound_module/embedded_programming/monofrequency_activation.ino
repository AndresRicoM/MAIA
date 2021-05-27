/*

███╗   ███╗ █████╗ ██╗ █████╗
████╗ ████║██╔══██╗██║██╔══██╗
██╔████╔██║███████║██║███████║
██║╚██╔╝██║██╔══██║██║██╔══██║
██║ ╚═╝ ██║██║  ██║██║██║  ██║
╚═╝     ╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝

Andres Rico - MIT Media Lab - 2021 - aricom@mit.edu

Code for running a mono frequency sound for running sound experiments on MAIA rector's sound experiment module. 

 */

int soundPin = 3; //Output Pin on ATTINY 412. 
int lightPin = 0; //Debugging LED. 

void setup() {

  tone(soundPin, 8000); //Output Monofrequency of 8kHz
  pinMode(lightPin, OUTPUT);
  digitalWrite(lightPin, HIGH); //Turn off for saving power. 
}

void loop() {

}
