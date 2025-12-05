#include <RH_RF69.h>
#include <SPI.h>

// Uno
#define RFM69_CS 10
#define RFM69_INT 2
#define RFM69_RST 9

#define RF69_FREQ 915.0

RH_RF69 rf69(RFM69_CS, RFM69_INT);

//TODO: Having datatype trouble when converting uint to c array and printing out over serial monitor

void setup() {
  Serial.begin(9600);
//  while (!Serial && millis() < 3000);
  delay(2000);

  Serial.println("Started Serial Comms");
  
  // Set Up Radio Settings
  pinMode(RFM69_CS, OUTPUT);
  digitalWrite(RFM69_CS, HIGH);

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
}

void handle_buffer(uint8_t *buf, uint8_t len){
  //TODO: Update this to use bluetooth to send data to phone
  Serial.print("Got a hit at: ");
   for (uint8_t i = 0; i < len; i++) {
    Serial.print(buf[i]);   // raw print
    Serial.print(",");
  }

  Serial.println();
}

void loop() {
  if (rf69.available()){
    uint8_t buf[64];
    uint8_t len;
    if (rf69.recv( (uint8_t *) buf, &len))
    {
      handle_buffer(buf, len);
    }
  }
}
