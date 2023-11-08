//test sketch to have the esp32 send data to the console
/*
#include <Arduino.h>
#include <AudioTools.h>
#include "AudioCodecs/CodecMP3Helix.h"
#include "zero.h"
#include <SPI.h>

MemoryStream mp3(zero_mp3, zero_mp3_len);
I2SStream  i2s; 
//VolumeStream volume(i2s);
EncodedAudioStream out(&i2s, new MP3DecoderHelix()); // output to decoder
StreamCopy copier(i2s, mp3);    // copy in to out

//auto vol_config = volume.defaultConfig();
void setup() {
  Serial.begin(115200);
  Serial.println("Hello World!!");

  pinMode(27, INPUT);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);  

  // update audio info with info from decoder
  out.setNotifyAudioChange(i2s);

  auto config = i2s.defaultConfig(TX_MODE);
  config.i2s_format = I2S_LSB_FORMAT;

  i2s.begin(config);


  // begin processing
  auto cfg = out.defaultConfig();
  out.begin(cfg);
  //volume.begin(vol_config);
  //volume.setVolume(0.5);
}


void loop() {
  delay(5000);
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

/**
 * @file streams-generator-i2s.ino
 * @author Phil Schatzmann
 * @brief see https://github.com/pschatzmann/arduino-audio-tools/blob/main/examples/examples-stream/streams-generator-i2s/README.md 
 * @copyright GPLv3
 */
 
 
#include "AudioTools.h"

AudioInfo info(44100, 2, 16);
SineWaveGenerator<int16_t> sineWave(32000);                // subclass of SoundGenerator with max amplitude of 32000
GeneratedSoundStream<int16_t> sound(sineWave);             // Stream generated from sine wave
I2SStream i2s; 
VolumeStream out(i2s);                             // output to i2s
StreamCopy copier(out, sound);                             // copies sound into i2s

// Arduino Setup
void setup(void) {  
  // Open Serial 
  Serial.begin(115200);
  while(!Serial);
  AudioLogger::instance().begin(Serial, AudioLogger::Warning);

  // start I2S
  Serial.println("starting I2S...");
  auto config = i2s.defaultConfig(TX_MODE);
  config.copyFrom(info); 
  config.i2s_format = I2S_LSB_FORMAT;
  config.pin_ws = 26;
  config.pin_bck = 5;
  config.pin_data = 25;
  i2s.begin(config);

  // start output
  Serial.println("starting output...");
  auto cfg = out.defaultConfig();

  out.begin(cfg);
  out.setVolume(0.3);


  // Setup sine wave
  sineWave.begin(info, N_B4);
  Serial.println("started...");
}

// Arduino loop - copy sound to out 
void loop() {
  copier.copy();
  float pan = 0.0;
  float volume = 0.25;
  out.setVolume(volume * (1.0 - pan), 0);
  out.setVolume(volume * pan, 1);
  for (int i=0; i<1000; i++) {
    delay(1);
    pan += 0.001;
    if (pan > 1.0) pan = -1.0;
    out.setVolume(volume * (1.0 - pan), 0);
    out.setVolume(volume * pan, 1);
    copier.copy();
  }  
}