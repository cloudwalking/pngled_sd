#include <SD.h>

File file;

uint8_t pixels;
uint8_t frames;
uint8_t fps;

void setup() {
  Serial.begin(9600);
  
  pixels = 0;
  frames = 0;
  fps = 0;
  
  pinMode(10, OUTPUT);
  if (!SD.begin(8)) {
    Serial.println(F("couldn't start SD"));
  }
  if (!openFile("flame2.dat")) {
    Serial.println(F("error opening file"));
  }
  
  while(file.available() < 5);
  
  // Format:
  // 0x60 0x60 <pixels> <frames> <frames per second>
  if (!(file.read() == 0x60 && file.read() == 0x60)) {
    Serial.println(F("invalid format"));
  }
  
  pixels = (int)file.read();
  frames = (int)file.read();
  fps    = (int)file.read();
  
  file.close();
}

void loop() {
  Serial.print("pixels: "); Serial.println(pixels);
  Serial.print("frames: "); Serial.println(frames);
  Serial.print("fps:    "); Serial.println(fps);
  delay(1000);
}

bool openFile(char *filename) {
  file = SD.open(filename);
  return file != false;
}