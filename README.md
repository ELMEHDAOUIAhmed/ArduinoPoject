# Smart Lock/Bike ArduinoPoject

Our solution aims to make the transport of students, teachers and all users of the USTHB very fast and reliable
Given the shortness of their time between sessions to change classrooms. 
We allow app users to reserve bikes in advance and use NFC technology to unlock them, which is faster than the QR code method.
We also use phone geolocation to track users and keep bikes safe.



# Things to improve in the future
# 1-make the code modular and easy to to edit via functions (80% done)
# 2-make the code more secure using tests before executing functions (90% done)
# 3-make the code dynamique (Via bluetooth Done)
# 4- add password checks before allowing to add or clear uid codes from bluetooth
# 5- change BT Pin code using AT commands ( didnt work fine another way)

#
BTserial.print("AT+NAME=NewName\r\n"); //didnt work fine another way
BTserial.print("AT+PSWD=NewPassword\r\n"); didnt work fine another way

# 6- Encrypt uid using maybe md5 TBD
# 7- add notification in case something happends

# Buy new BUZZER and use code to make sound no opened , closed and denided access



# Black is + ( Ground )
# Red is - (Has power)




##########################################################################