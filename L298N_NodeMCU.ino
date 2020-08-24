#include <FirebaseESP8266.h>
#include <FirebaseESP8266HTTPClient.h>
#include <FirebaseJson.h>
#include <jsmn.h>
#include <ESP8266WiFi.h>

#define FIREBASE_HOST "https://drip-cd001.firebaseio.com/" 
#define FIREBASE_AUTH "AKshuVFYYWzHvg0iZplqQEhNLQJ7xJGDYp4OgJrc"
#define WIFI_SSID "black beard" 
#define WIFI_PASSWORD "12345678" 

int ENA = 4; //D2
int IN1 = 0; //D3
int IN2 = 2; //D4
int ENB = 14; //D5
int IN3 = 12; //D6
int IN4 = 13; //D7
int M1 = 16; //D0
int M2 = 5; //D1
int sense_Pin = 0; //A0
int value = 0;
int n = 0; 

FirebaseData firebaseData;
FirebaseJson json;

void setup() {
// set all the motor control pins to outputs
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); 
  Serial.print("connecting"); 
  while (WiFi.status() != WL_CONNECTED) { 
  Serial.print("."); 
  delay(500); 
  }
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  delay(2000);
}

void testOne() {
  Serial.print("MOISTURE LEVEL FOR PLANT 1: ");
  digitalWrite(M1, HIGH);
  value= analogRead(sense_Pin);
  digitalWrite(M1, LOW);
  value= ( 100 - ( (value/1023.00) * 100 ) );
  Serial.println(value);
  json.clear().addInt("moisture1",value);
  if (Firebase.pushJSON(firebaseData, "/Moisture1", json))
  {
    Serial.println("PASSED");
  }
  else
  {
    Serial.println("FAILED");
  }
  if(value<100)
  {
  // turn on motor
    digitalWrite(ENA, 100);  // set speed to 200 out of possible range 0~255
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  }
  else
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }
}

void testTwo() {
  Serial.print("MOISTURE LEVEL FOR PLANT 2: ");
  digitalWrite(M2, HIGH);
  value= analogRead(sense_Pin);
  digitalWrite(M2, LOW);
  value= ( 100 - ( (value/1023.00) * 100 ) );
  Serial.println(value);
  json.clear().addInt("moisture2",value);
  if (Firebase.pushJSON(firebaseData, "/Moisture2", json))
  {
    Serial.println("PASSED");
  }
  else
  {
    Serial.println("FAILED");
  }
  if(value<100)
  {
    digitalWrite(ENB, 100);  // set speed to 200 out of possible range 0~255
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
  else
  {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }
}

void loop() {
  testOne();   
  delay(1000);   
  testTwo();   
  delay(1000);
}
