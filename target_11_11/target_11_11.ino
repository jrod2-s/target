// Teensy Target
// 11 x 11 Target

#include <RH_RF69.h>
#include <SPI.h>

// Teensy Pins
#define RFM69_CS 10
#define RFM69_INT 40
#define RFM69_RST 41

#define RF69_FREQ 915.0

RH_RF69 rf69(RFM69_CS, RFM69_INT);

const int iter = 11;

uint8_t col[iter] = {17,18,19,20,21,22,23,24,25,26,27};
uint8_t row[iter] = {2,3,4,5,6,7,8,9,14,15,16};


void setup() {
  Serial.begin(9600);
  delay(2000);

  Serial.println("Started Serial Comms");

  // Set Up Contact Pins
  for (int i=0; i<iter; i++) {
    pinMode(row[i], OUTPUT);
    digitalWrite(row[i], HIGH);
    
    pinMode(col[i], INPUT_PULLUP);
  }

  // Set Up Radio
  pinMode(RFM69_CS, OUTPUT);
  digitalWrite(RFM69_CS, HIGH);

  digitalWrite(RFM69_RST, LOW);
  delay(10);
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);

  if (!rf69.init()) {
    Serial.println("RFM69 init failed!");
    while(1);
  }
  
  Serial.println("RFM69 init OK!");

  rf69.setFrequency(RF69_FREQ);

  // Optional: higher transmit power (for HCW models)
  rf69.setTxPower(20, true); // 20 dBm, use high-power mode
}

void publish(uint8_t r, uint8_t c) {
  uint8_t msg[2] = {r,c};
  Serial.print("Publishing: ");
  Serial.print(msg[0]);
  Serial.print(",");
  Serial.println(msg[1]);
  Serial.print("len: ");
  Serial.println(sizeof(msg));
  
  rf69.send(msg, sizeof(msg));
  Serial.println("Sending msg");
  rf69.waitPacketSent();
  Serial.println("Package sent");
  delay(500);
}

void loop() {
  for (int i=0; i<iter; i++){   
    digitalWrite(row[i], LOW);
    
    for (int j=0; j<iter; j++){ 
      if (digitalRead(col[j]) == LOW){
        publish((uint8_t)i, (uint8_t)j);
      }
    }
    
    digitalWrite(row[i], HIGH);
  }
}
