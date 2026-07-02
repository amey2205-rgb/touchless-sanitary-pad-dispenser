/*
 Smart Sanitary Pad Vending Machine
 ---------------------------------
 Human-readable version for final year project.

 Features:
  - Coin Acceptor (8 pulses = Rs.5)
  - QR Payment via Firebase
  - IR Sensor based dispense verification
  - L298N Motor Driver
  - 16x2 I2C LCD
  - WiFi Reconnect
  - Idle LCD Mode
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------------- Pin Definitions ----------------
const byte COIN_PIN = 27;
const byte MOTOR_IN1 = 26;
const byte MOTOR_IN2 = 25;
const byte MOTOR_EN  = 33;
const byte IR_PIN    = 14;

// ---------------- WiFi ----------------
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// ---------------- Firebase ----------------
String firebaseStatusURL = "YOUR_FIREBASE_URL_HERE";
// ---------------- Machine Settings ----------------
int stock = 50;
const int MIN_PULSE = 6;
const int MAX_PULSE = 10;

const unsigned long MOTOR_RUN_TIME = 3000;
const unsigned long COIN_TIMEOUT   = 800;
const unsigned long FIREBASE_TIME  = 3000;
const unsigned long LCD_REFRESH    = 1000;
const unsigned long IDLE_TIME      = 60000;

// ---------------- Variables ----------------
bool motorRunning = false;
bool idleMode = false;
bool padDetected = false;

int pulseCount = 0;
int lastCoinState = HIGH;

unsigned long lastPulse = 0;
unsigned long motorStart = 0;
unsigned long lastFirebase = 0;
unsigned long lastLCD = 0;
unsigned long lastActivity = 0;

// ---------- Helper Functions ----------
void showMessage(String l1, String l2=""){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(l1);
  if(l2!=""){
    lcd.setCursor(0,1);
    lcd.print(l2);
  }
}
void wakeLCD(){
  lastActivity=millis();
  if(idleMode){
    lcd.backlight();
    idleMode=false;
  }
}
void startMotor(){
  digitalWrite(MOTOR_IN1,HIGH);
  digitalWrite(MOTOR_IN2,LOW);
  digitalWrite(MOTOR_EN,HIGH);
  motorRunning=true;
  padDetected=false;
  motorStart=millis();
}
void stopMotor(){
  digitalWrite(MOTOR_EN,LOW);
  motorRunning=false;
  if(padDetected){
    showMessage("Take Pad");
    Serial.println("Dispense Success");
  }else{
    showMessage("Dispense Error");
    Serial.println("No pad detected");
  }
}
void dispensePad(){
  if(motorRunning) return;
  if(stock<=0){ showMessage("OUT OF STOCK"); return; }
  wakeLCD();
  showMessage("Dispensing...");
  startMotor();
}
String readFirebaseStatus(){
  if(WiFi.status()!=WL_CONNECTED) return "";
  HTTPClient http;
  http.begin(firebaseStatusURL);
  String p="";
  if(http.GET()>0) p=http.getString();
  http.end();
  return p;
}
void updateFirebaseStatus(String s){
  if(WiFi.status()!=WL_CONNECTED) return;
  HTTPClient http;
  http.begin(firebaseStatusURL);
  http.addHeader("Content-Type","application/json");
  http.PUT("\"" + s + "\"");
  http.end();
}
void setup(){
  Serial.begin(115200);
  Wire.begin(21,22);
  lcd.init();
  lcd.backlight();
  pinMode(COIN_PIN,INPUT_PULLUP);
  pinMode(IR_PIN,INPUT);
  pinMode(MOTOR_IN1,OUTPUT);
  pinMode(MOTOR_IN2,OUTPUT);
  pinMode(MOTOR_EN,OUTPUT);
  digitalWrite(MOTOR_EN,LOW);
  showMessage("Connecting WiFi");
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED){delay(500);}
  updateFirebaseStatus("WAIT");
}
void loop(){
  if(WiFi.status()!=WL_CONNECTED) WiFi.begin(ssid,password);

  if(!idleMode && millis()-lastActivity>IDLE_TIME){
    lcd.noBacklight();
    idleMode=true;
  }

  if(motorRunning && !padDetected && digitalRead(IR_PIN)==LOW){
    padDetected=true;
    stock--;
    stopMotor();
  }

  if(motorRunning && millis()-motorStart>MOTOR_RUN_TIME){
    stopMotor();
  }

  int state=digitalRead(COIN_PIN);
  if(lastCoinState==HIGH && state==LOW){
    pulseCount++;
    lastPulse=millis();
  }
  lastCoinState=state;

  if(pulseCount>0 && millis()-lastPulse>COIN_TIMEOUT){
    if(pulseCount>=MIN_PULSE && pulseCount<=MAX_PULSE){
      showMessage("Rs 5 Detected");
      dispensePad();
    }else{
      showMessage("Invalid Coin");
    }
    pulseCount=0;
  }

  if(millis()-lastFirebase>FIREBASE_TIME){
    lastFirebase=millis();
    if(readFirebaseStatus()=="\"PAID\""){
      showMessage("Payment Done");
      dispensePad();
      updateFirebaseStatus("WAIT");
    }
  }

  if(millis()-lastLCD>LCD_REFRESH){
    lastLCD=millis();
    lcd.setCursor(0,0);
    lcd.print("Stock:");
    lcd.print(stock);
    lcd.print("   ");
    lcd.setCursor(0,1);
    lcd.print("Coin/QR Ready ");
  }
}
