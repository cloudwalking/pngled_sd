#include <SD.h>

// Format:
// 0x60 0x60 <pixels> <frames> <frames per second>
#define HEADER_SIZE 5

#define LED_PIXEL_COUNT 55

File file;
uint8_t pixels_per_frame;
uint8_t frames;
uint8_t frames_per_second;
double start_ms;

void setup() {
  Serial.begin(9600);
  
  frames = 0;
  pixels_per_frame  = 0;
  frames_per_second = 0;
  
  pinMode(10, OUTPUT);
  if (!SD.begin(8)) {
    Serial.println(F("couldn't start SD"));
  }
  if (!openFile("flame2.dat")) {
    Serial.println(F("error opening file"));
  }
  
  while(file.available() < HEADER_SIZE);
  
  // Format:
  // 0x60 0x60 <pixels> <frames> <frames per second>
  if (!(file.read() == 0x60 && file.read() == 0x60)) {
    Serial.println(F("invalid format"));
  }
  
  pixels_per_frame  = (uint8_t)file.read();
  frames            = (uint8_t)file.read();
  frames_per_second = (uint8_t)file.read();
  
  Serial.print("frames: "); Serial.println(frames);
  Serial.print("pixels: "); Serial.println(pixels_per_frame);
  Serial.print("fps   : "); Serial.println(frames_per_second);
  
  start_ms = millis();
}

void loop() {
  // Choose which frame to show based on the current time.
  double now = millis() - start_ms;
  int frame_num = fmod(now * frames_per_second, frames);
  
  Serial.print("frame_num:  ");
  Serial.println(frame_num);
  
  showFrameAtLocation(frame_num);
  
  delay(40);
}

bool openFile(char *filename) {
  file = SD.open(filename);
  return file != false;
}

void showFrameAtLocation(uint8_t frame_num) {
  uint8_t const colors_per_pixel = 3;
  
  uint8_t red, green, blue;

  double offset = frame_num * pixels_per_frame * colors_per_pixel;
  offset += HEADER_SIZE;
  
  Serial.print("offset:  ");
  Serial.println(offset);

  if (!file.seek(offset)) {
    Serial.println("error seeking file");
    Serial.print("seek: "); Serial.println(offset);
    Serial.print("file size: "); Serial.println(file.size());
  }

  for (uint8_t i = 0; i < pixels_per_frame; i++) {
    if (i >= LED_PIXEL_COUNT) break;
    
    red = (uint8_t)file.read();
    green = (uint8_t)file.read();
    blue = (uint8_t)file.read();
    
    // TODO: light up some LEDs
  }
  Serial.println();
}