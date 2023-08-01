# Smart Lock/Bike ArduinoPoject

Our solution aims to make the transport of students, teachers and all users of the USTHB very fast and reliable
Given the shortness of their time between sessions to change classrooms. 
We allow app users to reserve bikes in advance and use NFC technology to unlock them, which is faster than the QR code method.
We also use phone geolocation to track users and keep bikes safe.



#Future improvements

1- add password checks before allowing to add or clear uid codes from bluetooth
2- change BT Pin code using AT commands

example: change rate to 
BTserial.print("AT+NAME=NewName\r\n"); 
BTserial.print("AT+PSWD=NewPassword\r\n");




#Documentation for RFID Door Lock System Code:

The code is written in C++ and it is used to control an RFID Door Lock System with the help of an Arduino board. It reads the unique IDs (UID) of RFID tags and compares them to a list of accepted UIDs. If the scanned UID is in the list, it unlocks the door. Additionally, the system can be controlled through Bluetooth by adding new UIDs to the list of accepted UIDs, clearing the list or getting the status of the lock.

#The code makes use of the following libraries:

SPI.h - library used for communication with the RFID reader module
MFRC522.h - library used for interfacing with the RFID reader module
SoftwareSerial.h - library used to create a new serial port on pins that do not have built-in serial communication
Wire.h - library used for I2C communication
HardwareSerial.h - library used to communicate with the Bluetooth module

#Global variables:

SS_PIN: Slave Select Pin used for the RFID reader module
RST_PIN: Reset Pin used for the RFID reader module
LOCK_PIN: Digital Pin connected to a lock-style solenoid 12VDC
BUZZER_PIN: Digital Pin connected to a buzzer
MAX_UIDS: maximum number of accepted UIDs
mfrc522: object used to interact with the RFID reader module
bthc05: object used to interact with the Bluetooth module
unlockAttempts: variable used to keep track of unlock attempts
uid: string variable used to store the scanned UID
BT_uid: string variable used to store the UID received from Bluetooth
BT_String: string variable used to store the Bluetooth input data
allowedUIDs_count: variable used to keep track of the number of accepted UIDs
uidByte: byte array used to store the UID as a byte array
allowedUIDs: string variable used to store the list of accepted UIDs
AdminUIDs: string array variable used to store the static UIDs for admins

#Functions:

setup(): initializes the communication with the RFID reader module, the Bluetooth module, and the lock pin.
loop(): continuously runs the program and checks for new cards. If a new card is detected, it checks if the UID is in the list of accepted UIDs. If it is, it opens the lock. If not, it denies access.
sendStatusAndRecords(): sends the status of the lock via Bluetooth.
getUIDAsString(): function that takes in a byte array and its size and returns a string of the UID.
openLock(): function that opens the lock and displays the UID that was scanned.
closeLock(): function that closes the lock and displays the UID that was scanned.
denidedAccess(): function that plays a buzzer sound to signal access denial.
convertUidStringToBytes(): function that converts a UID string to a byte array.
The code has a few areas that could be improved in the future, such as adding password protection before allowing the addition of a new UID, and adding the ability to open multiple locks, which would require a change in the code.