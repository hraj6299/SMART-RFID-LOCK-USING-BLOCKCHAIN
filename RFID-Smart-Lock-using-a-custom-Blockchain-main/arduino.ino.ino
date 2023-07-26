#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#include <SoftwareSerial.h>

#define RST_PIN   9
#define SS_PIN    10

byte readCard[4];
char* myTags[100] = {};
int tagsCount = 0;
String tagID = "E3 97 C4 1E";
boolean successRead = false;
boolean correctTag = false;
int proximitySensor;
bool doorOpened = false;
int doorTimeout = 25;

// Create instances
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); //Parameters: (rs, enable, d4, d5, d6, d7)
Servo myServo; // Servo motor
SoftwareSerial mySUART(A5, A4);

void setup() {
  // Initiating
  SPI.begin();        // SPI bus
  mfrc522.PCD_Init(); //  MFRC522
  lcd.begin(16, 2);   // LCD screen
  myServo.attach(8);  // Servo motor

  myServo.write(10); // Initial lock position of the servo motor
  // Prints the initial message
  lcd.print("-No Admin Key!-");
  lcd.setCursor(0, 1);
  lcd.print("    SCAN NOW");
  // Waits until a master card is scanned
  while (!successRead) {
    successRead = getID();
    if (successRead == true) {
      myTags[tagsCount] = strdup(tagID.c_str()); // Sets the master tag into position 0 in the array
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Admin Key Set!");
      tagsCount++;
    }
  }
  successRead = false;
  printNormalModeMessage();
  Serial.begin(115200);  
  mySUART.begin(115200);
}

void loop() {
  int proximitySensor = analogRead(A0);
  // If door is closed...
  // if (proximitySensor > 200) {
    if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
      timeoutFunc();
      return;
    }
    if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
      timeoutFunc();
      return;
    }
    tagID = "";
    // The MIFARE PICCs that we use have 4 byte UID
    for ( uint8_t i = 0; i < 4; i++) {  //
      readCard[i] = mfrc522.uid.uidByte[i];
      tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
    }
    tagID.toUpperCase();
    mfrc522.PICC_HaltA(); // Stop reading

    correctTag = false;
    // Checks whether the scanned tag is the master tag
    if (tagID == myTags[0]) {
      lcd.clear();
      lcd.print("Program mode:");
      lcd.setCursor(0, 1);
      // lcd.print("Add/Remove Tag"); //not implementing removal for now
      lcd.print("Add Key");
      while (!successRead) {
        successRead = getID();
        if ( successRead == true) {
          // // removal old code
          // for (int i = 0; i < 100; i++) {
          //   if (tagID == myTags[i]) {
          //     myTags[i] = "";
          //     lcd.clear();
          //     lcd.setCursor(0, 0);
          //     lcd.print("  Tag Removed!");
          //     printNormalModeMessage();
          //     return;
          //   }
          // }
          
          // // Array adding code
          // myTags[tagsCount] = strdup(tagID.c_str());
          // lcd.clear();
          // lcd.setCursor(0, 0);
          // lcd.print("   Tag Added!");
          // printNormalModeMessage();
          // tagsCount++;

          //serial communication for adding a new card
          Serial.println("DEBUG: Sending serially: " + String("R") + String(tagID));
          mySUART.println("R" + tagID);
          mySUART.flush();
          while(mySUART.available()<=0){
            delay(500); //wait till recieve
          }
          delay(2000);
          String a = mySUART.readStringUntil('\n');
          a.trim();          
          Serial.println("Received serially: " + a);
          // if(a.compareTo("1") == 0){
          if(a == "1"){            
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("   Key Added!");
            printNormalModeMessage();
          }
          
          return;
        }
      }
    }
    successRead = false;
    //serial communication for verify
    Serial.println("Sending serially: " + String("V") + String(tagID));
    mySUART.println("V" + tagID);
    mySUART.flush();
    while(mySUART.available()<=0){
      delay(500); //wait till recieve
    }
    delay(2000);
    String a = mySUART.readStringUntil('\n');
    a.trim();
    Serial.println("DEBUG: Received serially: " + a);
    if(a == "1"){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Door opened!");
        myServo.write(170); // Unlocks the door
        doorOpened = true;  
        doorTimeout = 50;
        delay(2000);
        printNormalModeMessage();      
    }else{
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" Access Denied!");
      printNormalModeMessage();
      delay(500);            
    }
    // Checks whether the scanned tag is authorized
    // // old verify code
    // for (int i = 0; i < 100; i++) {
    //   if (tagID == myTags[i]) {
    //     lcd.clear();
    //     lcd.setCursor(0, 0);
    //     lcd.print(" Access Granted!");
    //     myServo.write(170); // Unlocks the door
    //     delay(2000);
    //     printNormalModeMessage();
    //     correctTag = true;
    //   }
    // }
    // if (correctTag == false) {
    //   lcd.clear();
    //   lcd.setCursor(0, 0);
    //   lcd.print(" Access Denied!");
    //   printNormalModeMessage();
    // }
  // }
  // If door is open...
  // else {
        // if(doorOpened){
        //   doorTimeout -= 1;
        //   delay(100);
        //   if(doorTimeout == 0){
        //     doorOpened = false;
        //     myServo.write(10);  //locking the door after timeout
        //     printNormalModeMessage();      
        //   }         
        // }
    
    // while (!doorOpened) {
    //   proximitySensor = analogRead(A0);
    //   if (proximitySensor > 200) {
    //     doorOpened = true;
    //   }
    // }
    // doorOpened = false;
    // delay(500);
    // myServo.write(10); // Locks the door
    // printNormalModeMessage();
  // }
}

void timeoutFunc(){
  if(doorOpened){
    doorTimeout -= 1;
    delay(100);
    if(doorTimeout == 0){
      doorOpened = false;
      myServo.write(10);  //locking the door after timeout
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Closing Door!");
      delay(2000);
      printNormalModeMessage();      
    }         
  }  
}

uint8_t getID() {
  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
    return 0;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
    return 0;
  }
  tagID = "";
  for ( uint8_t i = 0; i < 4; i++) {  // The MIFARE PICCs that we use have 4 byte UID
    readCard[i] = mfrc522.uid.uidByte[i];
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return 1;
}

void printNormalModeMessage() {
  delay(1500);
  lcd.clear();
  lcd.print("-Access Control-");
  lcd.setCursor(0, 1);
  lcd.print(" Scan Your Key!");
}