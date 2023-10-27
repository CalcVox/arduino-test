//test sketch to have the esp32 send data to the console

#include <Arduino.h>
#include <AudioTools.h>
#include "AudioCodecs/CodecMP3Helix.h"
#include "zero.h"
#include <SPI.h>

MemoryStream mp3(zero_mp3, zero_mp3_len);
AnalogAudioStream analog;  // Analog output 
EncodedAudioStream out(&analog, new MP3DecoderHelix()); // output to decoder
StreamCopy copier(out, mp3);    // copy in to out
void setup() {
  Serial.begin(115200);
  Serial.println("Hello World!!");
  pinMode(27, INPUT);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);  

  // update audio info with info from decoder
  out.setNotifyAudioChange(analog);

  // begin processing
  auto cfg = out.defaultConfig();
  out.begin(cfg);
}


void loop() {
  if (digitalRead(27) == HIGH) {
    if (mp3) {
      copier.copy();
      Serial.println("Button is pressed");\
    } else {
      auto info = out.decoder().audioInfo();
      LOGI("The audio rate from the mp3 file is %d", info.sample_rate);
      LOGI("The channels from the mp3 file is %d", info.channels); 
    }
    //delay(3000);
  }
  else {
    Serial.println("Button is not pressed");
  }
  
}
