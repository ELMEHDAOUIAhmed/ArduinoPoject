#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>
#include <Wire.h>

#define SS_PIN 53
#define RST_PIN 3
#define LOCK_PIN 2 // Lock-style Solenoid 12VDC
#define BUZZER_PIN 4 // Buffer PIN
#define MAX_UIDS 1 

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

HardwareSerial& bthc05(Serial1); // Create bthc05 instance

// Table of accepted NFC tag UIDs
int unlockAttempts = 0;
String uid="";
String BT_uid="";
String BT_String="";
int allowedUIDs_count =0;
byte uidByte[4];
String allowedUIDs;
String AdminUIDs[]={"80 63 2B 32"}; // static uids for admins to


//things to fix, only 1 allowed dynamique uid from BT

void setup() {
  Serial.begin(9600); // Initialize serial communication 
  SPI.begin(); // Initialize SPI bus
  mfrc522.PCD_Init(); // Initialize MFRC522
  pinMode(LOCK_PIN, OUTPUT); // Initialize lock pin
  bthc05.begin(9600); // Initialize bluetooth module
}

void loop() {


    if(bthc05.available()) // Looking for Bluetooth Received Data
    {
      
    BT_String = bthc05.readStringUntil('\n'); 
    // BT_uid = BT_String;
    BT_String.trim(); // Remove any leading or trailing white space
    if(BT_String == "status"){
      sendStatusAndRecords();
    }
    else if (BT_String == "clear") // clear table from previous added uids
    {
    allowedUIDs="";
    bthc05.println("Cleared");
    allowedUIDs_count =0;
    }
    else // IMPORTANT after add function to check for password before adding a new uid into allowed uids 
    {
    //convertUidStringToBytes(BT_String,uidByte);     


      if (allowedUIDs_count < MAX_UIDS)
      {
        allowedUIDs=BT_String;
        allowedUIDs_count++;
        Serial.println("Added UID to allowed list: " + BT_String);
        bthc05.println("Added UID to allowed list: " + BT_String);

      }
        else if (allowedUIDs_count==MAX_UIDS)
        {
        Serial.println("Maximum number of allowed UID tags reached. \n");
        bthc05.println("Maximum number of allowed UID tags reached. \n");
        

        }
        
        else if (strcmp(allowedUIDs.c_str(),BT_String.c_str()) == 0) // wont be executed because Max allowed UID tags is 1 , wrote it incase we add more in future 
        {
        Serial.println("UID tag already in allowed list.");
        bthc05.println("UID tag already in allowed list.");
 
        }

      }
     
    }

  // Look for new cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) 
  {
    // Check if the card's UID is in the table of accepted UIDs
    uid = getUIDAsString(mfrc522.uid.uidByte, mfrc522.uid.size);

    bool accepted = false;
    // If uid exists in AdminUIDs accepted=true;
    for (int i = 0; i < sizeof(AdminUIDs) / sizeof(AdminUIDs[0]); i++) {
      if (AdminUIDs[i] == uid) {
        accepted = true;
        break;
      }
    }
    // check if String receiving equals allowedUids String
    if (accepted || (strcmp(allowedUIDs.c_str(),uid.c_str()) == 0)) 
    {
      unlockAttempts = 0;
      if (digitalRead(LOCK_PIN) == LOW)
        {
          openLock(uid);
        }   
      else {
        closeLock(uid);
          }
    }
     else {
          denidedAccess(uid);
          }
    
   }   


                        

 }



void sendStatusAndRecords()
{

// Function to send status and records via bluetooth

  // Check if the lock is currently open or closed
  String lockStatus = digitalRead(LOCK_PIN) == HIGH ? "Open" : "Closed";
  // Construct a message with the lock status and some example records
  String record = "Records: \n Lock is " + lockStatus + "\n";
  bthc05.println(record);

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


// future improvement add lock pin constant as a entry to the function so then you can display which lock was opened , for now we only have one lock

void openLock(String uid)
{

      Serial.print(F("UID tag : ")); // Serial.print() is only used for debugging remvoe after
      Serial.println(uid);
      Serial.println(F("Access granted"));

      digitalWrite(LOCK_PIN, HIGH); // Open the lock

      Serial.print(F("Lock: "));
      Serial.print(LOCK_PIN);
      Serial.println(F("has been opened"));

      String message = "Access granted, lock " + String(LOCK_PIN) + " opened , UID tag :" + uid;
      bthc05.println(message); // Send message via bluetooth
      delay(500);

}

void closeLock(String uid)
{

      Serial.print(F("UID tag : ")); // Serial.print() is only used for debugging remvoe after
      Serial.println(uid);
      Serial.println(F("Access granted")); 

      digitalWrite(LOCK_PIN, LOW); // Close the lock     

      Serial.print(F("Lock: "));
      Serial.print(LOCK_PIN);  
      Serial.println(F("has been closed"));    

      String message = "Access granted, lock " + String(LOCK_PIN) + " closed , UID tag :" + uid;
      bthc05.println(message); // Send message via bluetooth
      delay(500);
}

void denidedAccess(String uid)
{
      Serial.print(F("UID tag : ")); // Serial.print() is only used for debugging remvoe after
      Serial.println(uid);
      Serial.println(F("Access denied"));

      String message ="Access denied, UID tag: " + uid + " is not allowed to open Lock " + String(LOCK_PIN);
      bthc05.println(message); // Send message via bluetooth
      delay(1500);      
      unlockAttempts++;
      if (unlockAttempts == 4) {
      bthc05.println("Warning 4 attempts to unlock lock");
      bthc05.println("Locking down for 30 seconds");

      Serial.println(F("Warning 4 attempts to unlock lock"));
      Serial.println(F("Locking down for 30 seconds"));

      delay(30000);
      }
}

//not used 03/03/2023 10:45 PM
void convertUidStringToBytes(String uidString, byte uidBytes[]) { 
  int byteValue;
  for (int i = 0; i < uidString.length(); i += 2) {
    byteValue = strtoul(uidString.substring(i, i+2).c_str(), NULL, 16);
    uidBytes[i/2] = byteValue;
  }
}


