// Target Prototype (3x3)
//TODO: Populate the publish function
//TODO: Upgrade digitalwrite with direct port manipulation
//NOTE: I think pin two is shorted and is borking a hole row.

#include <RH_RF69.h>
#include <SPI.h>

// Micro
#define RFM69_CS 17
#define RFM69_INT 7
#define RFM69_RST 6
#define LED 13

#define RF69_FREQ 915.0

RH_RF69 rf69(RFM69_CS, RFM69_INT);

const int iter = 3;

uint8_t col[iter] = {10,11,12};
uint8_t row[iter] = {2,3,4};

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

  // Set up LED
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

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
  digitalWrite(LED, HIGH);
  uint8_t msg[2] = {r,c};
  Serial.print("Publishing: ");
  Serial.print(r);
  Serial.print(",");
  Serial.println(c);
  rf69.send(msg, 2);
  Serial.println("Sending msg");
  rf69.waitPacketSent();
  Serial.println("Package sent");
  delay(500);
  digitalWrite(LED, LOW);
}

void loop() {
  for (int i=0; i<iter; i++){   
    digitalWrite(row[i], LOW);
    
    for (int j=0; j<iter; j++){ 
      if (digitalRead(col[j]) == LOW){
        publish(row[i], col[j]);
      }
    }
    
    digitalWrite(row[i], HIGH);
  }
}
