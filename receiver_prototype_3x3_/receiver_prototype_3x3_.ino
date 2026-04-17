#include <RH_RF69.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>

// Set up SPI pins
#define RFM69_CS 10
#define RFM69_INT 3
#define RFM69_RST 9

// Define radio frequency and initialize radio
#define RF69_FREQ 915.0
RH_RF69 rf69(RFM69_CS, RFM69_INT);

// Initialize LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  //Start LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  // Print Welcome Message
  lcd.setCursor(7,0);
  lcd.print("Straight");
  lcd.setCursor(7,1);
  lcd.print("Shot");
  lcd.setCursor(7,2);
  lcd.print("Steel");
  delay(2000);

  // Start serial messages
  Serial.begin(9600);
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

  lcd.clear();

  // Start radio, check for failure
  if (!rf69.init()) {
    Serial.println("RFM69 init failed!");
    lcd.setCursor(0,1);
    lcd.print("Comms failure!");
    lcd.setCursor(0,2);
    lcd.print("Contact tech support");
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

  // Convert array to in and set up message
  int updown = -(buf[0] - 5);
  int rightleft = (buf[1] - 5);
  String updownmsg;
  String rightleftmsg;

  // Updown Message
  if (updown > 0){
    updownmsg = " inches high";
  }
  else if (updown < 0) {
    updownmsg = " inches low";
  }
  else {
    updownmsg = "centered up/down";
  }

  // Rightleft Message
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
  // Run whenever new message arrvies
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
