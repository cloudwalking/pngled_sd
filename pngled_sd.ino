#include <Adafruit_NeoPixel.h>
#include <SD.h>

// Format:
// 0x60 0x60 <pixels> <frames> <frames per second>
#define HEADER_SIZE 5

#define LED_COUNT 55
#define LED_DATA_PIN 3
#define LED_DEFAULT_BRIGHTNESS 25

#define DEBUG false

File file1, file2;
uint8_t pixels_per_frame;
uint8_t frames;
uint8_t frames_per_second;
double start_ms;
int current_frame_num;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_DATA_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  if (DEBUG) Serial.begin(9600);
  if (DEBUG) while(!Serial);
  
  frames = 0;
  pixels_per_frame  = 0;
  frames_per_second = 0;
  current_frame_num = -1;
  
  strip.begin();
  strip.setBrightness(LED_DEFAULT_BRIGHTNESS);
  
  pinMode(10, OUTPUT);
  if (!SD.begin(8)) {
    if (DEBUG) Serial.println(F("couldn't start SD"));
  }
  if (!openFile(file1, "rainbow2.dat")) {
    if (DEBUG) Serial.println(F("error opening file"));
  }
  
  while(file.available() < HEADER_SIZE);
  
  // Format:
  // 0x60 0x60 <pixels> <frames> <frames per second>
  if (!(file.read() == 0x60 && file.read() == 0x60)) {
    if (DEBUG) Serial.println(F("invalid format"));
  }
  
  pixels_per_frame  = (uint8_t)file.read();
  frames            = (uint8_t)file.read();
  frames_per_second = (uint8_t)file.read();
  
  if (DEBUG) Serial.print("frames: "); if (DEBUG) Serial.println(frames);
  if (DEBUG) Serial.print("pixels: "); if (DEBUG) Serial.println(pixels_per_frame);
  if (DEBUG) Serial.print("fps   : "); if (DEBUG) Serial.println(frames_per_second);
  
  start_ms = millis();
}

void loop() {
  // Choose which frame to show based on the current time.
  double now = (millis() - start_ms) / 1000;
  int frame_num = fmod(now * frames_per_second, frames);
  
  if (frame_num != current_frame_num) {
    current_frame_num = frame_num;
    if (DEBUG) Serial.print("frame_num:  ");
    if (DEBUG) Serial.println(frame_num);
    showFrameAtLocation(frame_num);
  }
  
  delay(20);
}

bool openFile(File file, char *filename) {
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
    if (DEBUG) Serial.println("error seeking file");
    if (DEBUG) Serial.print("seek: "); Serial.println(offset);
    if (DEBUG) Serial.print("file size: "); Serial.println(file.size());
  }

  for (uint8_t i = 0; i < pixels_per_frame; i++) {
    if (i >= LED_COUNT) break;
    
    red = (uint8_t)file.read();
    green = (uint8_t)file.read();
    blue = (uint8_t)file.read();
    
    strip.setPixelColor(i, strip.Color(red, green, blue));
  }
  
  strip.show();
  
  if (DEBUG) Serial.println();
}