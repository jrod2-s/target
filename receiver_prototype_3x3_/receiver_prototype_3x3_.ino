#include <RH_RF69.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>

// Uno
#define RFM69_CS 10
#define RFM69_INT 3
#define RFM69_RST 9

#define RF69_FREQ 915.0

RH_RF69 rf69(RFM69_CS, RFM69_INT);

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  Serial.begin(9600);
//  while (!Serial && millis() < 3000);
  delay(2000);

  Serial.println("Started Serial Comms");
  
  //Start LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();

  
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
    lcd.setCursor(0,1);
    lcd.print("RFM69 init failed!");
    while(1);
  }
  
  Serial.println("RFM69 init OK!");
  

  rf69.setFrequency(RF69_FREQ);
}

void handle_buffer(uint8_t *buf, uint8_t len){
  //TODO: Update this to use bluetooth to send data to phone
  Serial.print("buf: ");
  Serial.println(buf[0]);
  Serial.print("Got a hit at: ");
  
  Serial.print(buf[0]);   // raw print
  Serial.print(",");
  Serial.println(buf[1]);

  // LCD Print

  int updown = -(buf[0] - 5);
  int rightleft = (buf[1] - 5);
  String updownmsg;
  String rightleftmsg;

  if (updown > 0){
    updownmsg = " inches high";
  }
  else if (updown < 0) {
    updownmsg = " inches low";
  }
  else {
    updownmsg = "centered up/down";
  }

  if (rightleft > 0) {
    rightleftmsg = " inches right";
  } 
  else if (rightleft < 0) {
    rightleftmsg = " inches left";
  }
  else {
    rightleftmsg = "centered right/left";
  }

  // Print Message
  lcd.clear();

  lcd.setCursor(0,0);
  if (updown != 0) {
    lcd.print(abs(updown));
  }
  lcd.print(updownmsg);

  lcd.setCursor(0,2);
  if (rightleft != 0) {
    lcd.print(abs(rightleft));
  }
  lcd.print(rightleftmsg);
  }

void loop() {

  if (rf69.available()){
    uint8_t buf[5];
    memset(buf, 0, sizeof(buf));

    uint8_t len;
    if (rf69.recv( (uint8_t *) buf, &len))
    {
      handle_buffer(buf, len);
    }
  }
}
