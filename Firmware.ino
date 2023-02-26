#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>
#include <Wire.h>

#define SS_PIN 53
#define RST_PIN 3
#define LOCK_PIN 2 // Lock-style Solenoid 12VDC

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

SoftwareSerial BTSerial(18, 19); // RX | TX inverse between them
HardwareSerial& bthc05(Serial1);

// Table of accepted NFC tag UIDs
String UIDs[] = {"B4 B2 70 1C", "2F 6F 36 7A","80 63 2B 32"};
int unlockAttempts = 0;

void setup() {
  Serial.begin(9600); // Initialize serial communication
  BTSerial.begin(9600); // Initialize bluetooth communication
  SPI.begin(); // Initialize SPI bus
  mfrc522.PCD_Init(); // Initialize MFRC522
  pinMode(LOCK_PIN, OUTPUT); // Initialize lock pin
  bthc05.begin(9600); 
  bthc05.println("Bluetooth On....")
}

void loop() {

  // Look for new cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Check if the card's UID is in the table of accepted UIDs
    String uid = getUIDAsString(mfrc522.uid.uidByte, mfrc522.uid.size);
    bool accepted = false;
    for (int i = 0; i < sizeof(UIDs) / sizeof(UIDs[0]); i++) {
      if (UIDs[i] == uid) {
        accepted = true;
        break;
      }
    }
    if (accepted) {

      unlockAttempts = 0;

      if (digitalRead(LOCK_PIN) == LOW)
      {
      Serial.print(F("UID tag : "));
      Serial.println(uid);
      Serial.println(F("Access granted"));
      digitalWrite(LOCK_PIN, HIGH); // Open the lock
      Serial.print(F("Lock: "));
      Serial.print(LOCK_PIN);
      Serial.println(F(" has been opened"));
      String message = "Access granted, lock "+ LOCK_PIN;
      message += "opened , UID tag :" + uid;
      BTSerial.println(message); // Send message via bluetooth
      delay(1500);
      }   
      else {
      Serial.print(F("UID tag : "));
      Serial.println(uid);
      Serial.println(F("Access granted")); 
      digitalWrite(LOCK_PIN, LOW); // Close the lock     
      Serial.print(F("Lock: "));
      Serial.print(LOCK_PIN);  
      Serial.println(F(" has been closed"));    
      String message = "Access granted, lock "+ LOCK_PIN;
      message +=  " closed, UID tag: " + uid;
      BTSerial.println(message); // Send message via bluetooth
      delay(1500);
      }
    }
     else {
      Serial.print(F("UID tag : "));
      Serial.println(uid);
      Serial.println(F("Access denied"));
      String message = "Access denied, UID tag: " + uid;
      BTSerial.println(message); // Send message via bluetooth
      delay(1500);      
      unlockAttempts++;
      if (unlockAttempts == 4) {
      BTSerial.println("Warning 4 attempts to unlock lock");
      Serial.println(F("Warning 4 attempts to unlock lock"));
      BTSerial.println("Locking down for 30 seconds");
      Serial.println(F("Locking down for 30 seconds"));

      delay(30000);
      }
        
    }
    }

    

}

// Convert the UID byte array to a string
String getUIDAsString(byte *uidBytes, byte uidSize) {
  String uidString = "";
  for (byte i = 0; i < uidSize; i++) {
    if (uidBytes[i] < 0x10) {
      uidString += "0";
    }
    uidString += String(uidBytes[i], HEX);
    uidString.toUpperCase();
    uidString += " ";
  }
  uidString.trim();
  return uidString;
}


// Function to send status and records via bluetooth
void sendStatusAndRecords() {

  // Check if the lock is currently open or closed
  String lockStatus = digitalRead(LOCK_PIN) == HIGH ? "Open" : "Closed";
  // Construct a message with the lock status and some example records
  String record = "Records: \n Lock is " + lockStatus + "\n";
    if(bthc05.available()){
    String received = bthc05Str;
    if(received == "status"){
      bthc05.println(record);

    }
  }
  
  }

