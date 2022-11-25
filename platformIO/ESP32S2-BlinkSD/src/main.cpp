#include <Arduino.h>

extern void printESP32info();
extern void setupSD();

void setup() {

  Serial.begin(115200);
  delay(1000);
  while (!Serial) {delay(100);}

  // put your setup code here, to run once:
  //pinMode(9, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  
  ledcAttachPin(9, 0);    // assign a led pins to a channel
  ledcSetup(0, 4000, 8);  // 12 kHz PWM, 8-bit resolution
  
  printESP32info();
  setupSD();
}

void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(9, HIGH);digitalWrite(11,LOW);digitalWrite(12,LOW);
  //delay(500);
  digitalWrite(11,LOW);digitalWrite(12,LOW);
  for (int iBrt=255;iBrt>0;iBrt-=5) {
    ledcWrite(0, iBrt); // set
    delay(50); 
    }
  delay(500);
  
  digitalWrite(11,HIGH);digitalWrite(12,LOW);
  delay(500);
  digitalWrite(11,LOW);digitalWrite(12,HIGH);
  delay(500);
  Serial.println("Hello S2");
}