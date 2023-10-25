#include <Adafruit_NeoPixel.h>
#define PIN 9                 //pinul digital pentru date la care este conectata banda sau inelul de neopixeli
#define N_LEDS 157       //numarul de LED-uri neopixeli de pe banda sau inel
int SPEED_COEFICIENT = 1.0;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

int totalSteps = 100;
int maxValue = 255;
float base = pow(maxValue, 1.0 / (totalSteps - 1));

int chase_speed = 10;
int in = 1;
int resetDirection = 1;


uint32_t c1 = strip.Color(132, 234, 250);
uint32_t c2 = strip.Color(18, 52, 74);
uint32_t c3 = strip.Color(18, 52, 74);
uint32_t c4 = strip.Color(14, 24, 64);
uint32_t c5 = strip.Color(5, 13, 46);
uint32_t c6 = strip.Color(1, 5, 20);
uint32_t c7 = strip.Color(0, 0, 1);


void setup() {
  strip.begin();
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}
void loop() {
  if (Serial.available() > 0) {
      chase_speed = Serial.read();
  }

  chase_reverse(chase_speed);
  chase(chase_speed);

}

static void chase(int speed_of_chase) {
  
  for(uint16_t i=0; i<strip.numPixels()+19; i++) {

    // read speed update.
    serialFlush();
    while(Serial.available() <= 0){
      delay(2);
    }
    if (Serial.available() > 0) {
      int in = Serial.read();
  
      if (in >= 100 and in < 200) {
        speed_of_chase = 200 - in;
        chase_speed = in;
      } else if (in == 200) {
        activate();
        break;
      } else {
        resetLeds(1);
        break;
      }
    }
    
    strip.setPixelColor(i , c1); // Draw new pixel
    strip.setPixelColor(i-1 , c1); // Draw new 
    strip.setPixelColor(i-2 , c1); // Draw new pixel
    strip.setPixelColor(i-3 , c2); // Draw new pixel
    strip.setPixelColor(i-4 , c2); // Draw new pixel
    strip.setPixelColor(i-5 , c2); // Draw new pixel
    strip.setPixelColor(i-6 , c3); // Draw new pixel
    strip.setPixelColor(i-7 , c3); // Draw new pixel
    strip.setPixelColor(i-8 , c3); // Draw new pixel
    strip.setPixelColor(i-9 , c4); // Draw new pixel
    strip.setPixelColor(i-10 , c4); // Draw new pixel
    strip.setPixelColor(i-11 , c4); // Draw new pixel
    strip.setPixelColor(i-12 , c5); // Draw new pixel
    strip.setPixelColor(i-13 , c5); // Draw new pixel
    strip.setPixelColor(i-14 , c5); // Draw new pixel
    strip.setPixelColor(i-15 , c6); // Draw new pixel
    strip.setPixelColor(i-16 , c6); // Draw new pixel
    strip.setPixelColor(i-17 , c6); // Draw new pixel
    strip.setPixelColor(i-18 , c7); // Draw new pixel
    strip.setPixelColor(i-19 , c7); // Draw new pixel
    strip.setPixelColor(i-20, c7);
    strip.show();
    delay(speed_of_chase*SPEED_COEFICIENT); // speed_of_chase is between 1 and 100 from touch.
  }
}



static void chase_reverse(int speed_of_chase) {
  
  for(uint16_t i=strip.numPixels()+19; i>0; i--) {

    // read speed update.
    serialFlush();
    while(Serial.available() <= 0){
      delay(2);
    }
    if (Serial.available() > 0) {
      Serial.flush();
      int in = Serial.read();
  
      if (in >= 0 and in < 100) {
        speed_of_chase = in;
        chase_speed = in;
      } else if (in == 200) {
        activate();
        break;
      } else {
        resetLeds(0);
        break;
      }
    }
    
    strip.setPixelColor(i , c7); // Draw new pixel
    strip.setPixelColor(i-1 , c7); // Draw new 
    strip.setPixelColor(i-2 , c7); // Draw new pixel
    strip.setPixelColor(i-3 , c6); // Draw new pixel
    strip.setPixelColor(i-4 , c6); // Draw new pixel
    strip.setPixelColor(i-5 , c6); // Draw new pixel
    strip.setPixelColor(i-6 , c5); // Draw new pixel
    strip.setPixelColor(i-7 , c5); // Draw new pixel
    strip.setPixelColor(i-8 , c5); // Draw new pixel
    strip.setPixelColor(i-9 , c4); // Draw new pixel
    strip.setPixelColor(i-10 , c4); // Draw new pixel
    strip.setPixelColor(i-11 , c4); // Draw new pixel
    strip.setPixelColor(i-12 , c3); // Draw new pixel
    strip.setPixelColor(i-13 , c3); // Draw new pixel
    strip.setPixelColor(i-14 , c3); // Draw new pixel
    strip.setPixelColor(i-15 , c2); // Draw new pixel
    strip.setPixelColor(i-16 , c2); // Draw new pixel
    strip.setPixelColor(i-17 , c2); // Draw new pixel
    strip.setPixelColor(i-18 , c1); // Draw new pixel
    strip.setPixelColor(i-19 , c1); // Draw new pixel
    strip.setPixelColor(i-20, c1); 
    strip.show();
    delay(speed_of_chase*SPEED_COEFICIENT); // speed_of_chase is between 1 and 100 from touch.
  }
}

void resetLeds(int r_direction){
 // reset only if previously not reset for this direction
  if (r_direction != resetDirection){
    resetDirection = r_direction;
    for(uint16_t i=0; i<strip.numPixels()+19; i++) {
      strip.setPixelColor(i , 0);
    }
  }
  strip.show();
}

void serialFlush(){
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
}

static void activate() {
  int r = 132;
  int g = 234;
  int b = 250;

  for (int x = 0; x < totalSteps; x++) {
    
    int y = (int)(pow(base, x) + 0.9); // Calculate exponential growth and round it
    y = min(y, maxValue);
    int rr = 1;
    if ((1+y) < 255){
      rr = 1+y;
    } else {
      rr = 255;  
    }

    int gg = 2;
    if ((2+y) < 255){
      gg = 2+y;
    } else {
      gg = 255;  
    }

    int bb = 6;
    if ((6+y) < 255){
      bb = 6+y;
    } else {
      b = 255;  
    }
    
    uint32_t c1 = strip.Color(rr, gg, bb);
    
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i , c1); // Draw new pixel
    }
    strip.show();
    delay(2);
  }

  
  delay(10);

  for (int x = 0; x < totalSteps; x++) {
    int y = (int)(pow(base, x) + 0.9); // Calculate exponential growth and round it
    y = min(y, maxValue);
    uint32_t c1 = strip.Color(255-y, 255-y, 255-y);
    
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i , c1); // Draw new pixel
    }
    strip.show();
    delay(10);
  }

  strip.show();
}
