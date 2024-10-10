**IoT Smart Lock with Keypad and ThingSpeak Integration**
This project demonstrates a smart lock system using an ESP8266, a servo motor, a keypad, and ThingSpeak for remote monitoring. The lock can be unlocked using a pre-set passcode or a master passcode, and it tracks lock and unlock status remotely via the ThingSpeak platform.



**Features**
**Keypad Unlocking:** Users can unlock the lock using a predefined passcode or master passcode.
**Master Key:** After 3 incorrect attempts, the lock will require the master key to unlock.
**ThingSpeak Integration:** Lock status updates (lock/unlock events and incorrect attempts) are sent to a ThingSpeak channel for remote monitoring.
**Servo Lock Mechanism:** A servo motor controls the physical locking and unlocking mechanism.
**WiFi Connectivity:** Uses the ESP8266 to connect to a WiFi network.


**Components
ESP8266 NodeMCU:** Microcontroller that connects to WiFi and controls the lock.
**Servo Motor:** Controls the physical locking mechanism.
**4x4 Keypad:** Allows user to input a passcode for unlocking the lock.
**ThingSpeak Account:** To monitor lock status remotely.



**Setup Instructions**

**Hardware Requirements**
1.ESP8266 (NodeMCU)
2.Servo Motor
4.4x4 Keypad
5.Breadboard and connecting wires
6.Power supply for ESP8266

**Software Requirements**
1.Arduino IDE
2.ESP8266 board package installed in the Arduino IDE
3.ThingSpeak account with a created channel



**Configuration**
#define SECRET_SSID "your_ssid"
#define SECRET_PASS "your_password"
#define SECRET_CH_ID your_channel_id
#define SECRET_WRITE_APIKEY "your_thingspeak_write_key"


for any help conntect on abhayarya@cyberswipe.in
