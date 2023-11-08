#include "AudioTools.h"

AudioInfo info(44100, 2, 16);
SineWaveGenerator<int16_t> sineWave(32000);               
GeneratedSoundStream<int16_t> sound(sineWave);            
I2SStream i2s; 
VolumeStream out(i2s);                             
StreamCopy copier(out, sound);                             

void setup(void) {  
  Serial.begin(115200);
  while(!Serial);
  AudioLogger::instance().begin(Serial, AudioLogger::Warning); // if set to info, not good

  // start I2S
  Serial.println("starting I2S...");
  auto config = i2s.defaultConfig(TX_MODE);
  config.copyFrom(info); 
  // hellish config for this dumb dac and the dumb way i made this audio prototype
  config.i2s_format = I2S_LSB_FORMAT;
  config.pin_ws = 26;
  config.pin_bck = 5;
  config.pin_data = 25;
  i2s.begin(config);

  Serial.println("starting output...");
  auto cfg = out.defaultConfig();

  out.begin(cfg);
  out.setVolume(0.3);


  sineWave.begin(info, N_B4);
  Serial.println("started...");
}

void loop() {
  copier.copy();
  float pan = 0.0;
  float volume = 0.25; // still really fucking loud on my grados, they are 32 ohm i think
  out.setVolume(volume * (1.0 - pan), 0);
  out.setVolume(volume * pan, 1);
  for (int i=0; i<1000; i++) {
    delay(1); //audio does not play during a delay it seems. when i made this super small, it stopped clicking
    pan += 0.001;
    if (pan > 1.0) pan = -1.0;
    out.setVolume(volume * (1.0 - pan), 0);
    out.setVolume(volume * pan, 1);
    copier.copy();
  }  
}