#include <SD.h>                      // need to include the SD library
//#define SD_ChipSelectPin 53  //example uses hardware SS pin 53 on Mega2560
#define SD_ChipSelectPin 4  //using digital pin 4 on arduino nano 328, can use other pins
#include <TMRpcm.h>           //  also need to include this library...
#include <SPI.h>
static int alpha = 50;
static int threshold = 255;
unsigned long voltage = 1023; //start high
TMRpcm tmrpcm;   // create an object for use in this sketch

static unsigned int input_pin = A0;


void setup(){

  tmrpcm.speakerPin = 9; //5,6,11 or 46 on Mega, 9 on Uno, Nano, etc

  Serial.begin(9600);
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println("SD fail");
    return;   // don't do anything more if not
  }


  tmrpcm.play("tuesday.wav"); //the sound file "music" will play each time the arduino powers up, or is reset

  //randomSeed(analogRead(0));
	while(tmrpcm.isPlaying()); //wait for sound to stop
	tmrpcm.disable(); //stop the sound generating timer

  pinMode(input_pin,INPUT);
}



void loop(){
  //wait for doorbell to activate, but smooth it out with a lowpass, to avoid noise issues.
  while(voltage>threshold) voltage = ( voltage*alpha + analogRead(input_pin) ) / (alpha+1); //crude lowpass

  tmrpcm.play("zelda.wav");
  while(tmrpcm.isPlaying()); //wait for sound to stop

  tmrpcm.disable(); //stop the sound generating timer

  voltage=1023; //reset

  /*
  delay(500);
  tmrpcm.play("coin.wav");
  while(tmrpcm.isPlaying());
  delay(500);
  tmrpcm.play("coin.wav");
  */

  //here's a thing where a random number between 1-8 is generated and the corresponding sound is played:
  //remember to seed the random generator in the setup routine.
/*
    char rx=random(9);

    if(rx == 1) tmrpcm.play("coin.wav");
    if(rx == 2) tmrpcm.play("croc.wav");
    if(rx == 3) tmrpcm.play("drum.wav");
    if(rx == 4) tmrpcm.play("honk.wav");
    if(rx == 5) tmrpcm.play("listen.wav");
    if(rx == 6) tmrpcm.play("braaaaap.wav");
    if(rx == 7) tmrpcm.play("johnson.wav");
    if(rx == 8) tmrpcm.play("tuesday.wav");
*/


    //here's a thing where a sound is played when a number 1-8 is recieved over serial:
  /*
  if(Serial.available()){
    char rx = Serial.read();
    if(rx == '1') tmrpcm.play("coin.wav");
    if(rx == '2') tmrpcm.play("croc.wav");
    if(rx == '3') tmrpcm.play("drum.wav");
    if(rx == '4') tmrpcm.play("honk.wav");
    if(rx == '5') tmrpcm.play("listen.wav");
    if(rx == '6') tmrpcm.play("braaaaap.wav");
    if(rx == '7') tmrpcm.play("johnson.wav");
    if(rx == '8') tmrpcm.play("tuesday.wav");
  }*/

  while(!digitalRead(input_pin)); //wait for doorbell to finish.

}
