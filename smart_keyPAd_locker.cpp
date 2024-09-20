// CODE:-2
#include <Servo.h>
#include <ESP8266WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros
#include <Keypad.h>

Servo s1;

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

int lockStatusCode;

String myStatus = "";



// KeyPadLock Code

const byte n_rows = 4;
const byte n_cols = 4;
bool master = false;
char keys[n_rows][n_cols] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte colPins[n_rows] = {D3, D2, D1};
byte rowPins[n_cols] = {D7, D6, D5, D4};

Keypad myKeypad = Keypad( makeKeymap(keys), rowPins, colPins, n_rows, n_cols);

const char masterPasscode[] = "0000"; // Master key to unlock for all users
const char passcode[] = "1234";
const char passcode1[] = "2234";
const char passcode2[] = "3234";
const char passcode3[] = "4234";
const char passcode4[] = "5234";
const char passcode5[] = "6234";

char enteredPasscode[5];
byte passcodeIndex = 0;
bool locked = true;

int wrongAttempts = 0;
const int maxWrongAttempts = 3; // Maximum allowed consecutive wrong attempts


void setup() {
  Serial.begin(115200); 
   pinMode(D8, OUTPUT); 
  s1.attach(D0); // Initialize serial
  s1.write(180);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  
  WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}


void unlock() {
  if(master)
  {
    Serial.println("Unlocked");
   master = true;
  digitalWrite(D8, LOW);
  s1.write(180);  // tell s1 to go to position in variable 'pos'
 
  // delay(15);
   // set the fields with the values
  ThingSpeak.setField(1, lockStatusCode);
  
  // set the status
  ThingSpeak.setStatus(myStatus);
  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  
  delay(2000);
 s1.write(0); 

 lockStatusCode = 001;

  wrongAttempts = 0; // Reset the wrong attempts counter
  }
  else {
    Serial.println("Enter the master unl,ock first");
  }
  
}















void wrongAttempt() {
  Serial.println("Wrong Attempt");
  // set the fields with the values
  ThingSpeak.setField(2, lockStatusCode);
  // set the status
  ThingSpeak.setStatus(myStatus);
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
   lockStatusCode = 007;
  wrongAttempts++;
  
  if (wrongAttempts >= maxWrongAttempts) {
    digitalWrite(D8, HIGH);
    master = false;
    Serial.println("Locked , Please Enter The Master Key to Unlock");
    // After 3 consecutive wrong attempts, take additional action if needed
    // For example, sound a continuous alarm or lockout for a period
    // Add your logic here.
  }
}












void evaluatePasscode() {
  enteredPasscode[passcodeIndex] = '\0'; // Null-terminate the entered passcode
  if (strcmp(enteredPasscode, masterPasscode) == 0) {
    // Master key unlocks for all users
    unlock();
   master = true;
    Serial.println("Master Key Unlocked");
     digitalWrite(D8, LOW);
        lockStatusCode = 001;
        s1.write(180);



      // set the fields with the values
 ThingSpeak.setField(1, lockStatusCode);
  
  // set the status
  ThingSpeak.setStatus(myStatus);
  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  delay(2000);
  s1.write(0);




  }
  else if (locked) {
    if (strcmp(enteredPasscode, passcode) == 0 ||
        strcmp(enteredPasscode, passcode1) == 0 ||
        strcmp(enteredPasscode, passcode2) == 0 ||
        strcmp(enteredPasscode, passcode3) == 0 ||
        strcmp(enteredPasscode, passcode4) == 0 ||
        strcmp(enteredPasscode, passcode5) == 0) {
      unlock();
      // Serial.println("User Unlocked");
    } 
    else {
      wrongAttempt();
    }
  }

  passcodeIndex = 0;
  memset(enteredPasscode, 0, sizeof(enteredPasscode));
}

void loop() {
s1.write(0);
  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");

  }

  char myKey = myKeypad.getKey();
 
  if (myKey != NO_KEY) {
    Serial.print("Key pressed: ");
    Serial.println(myKey);
    
    if (locked) {
      if (myKey == '#') {
        evaluatePasscode();
      } else if (passcodeIndex < 4) {
        enteredPasscode[passcodeIndex] = myKey;
        passcodeIndex++;
      }
    }
  } // Wait 20 seconds to update the channel again
}