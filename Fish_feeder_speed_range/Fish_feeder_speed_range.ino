 #include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>


//-----associte pin number with sensor/actuator 

//----ISTIHAR GPIO PIN ---------------------------------------------------------------------------------

//---=on board------
#define SENSOR_PIN  15 // ESP32 pin GIOP21 connected to DS18B20 sensor's DQ pin
#define Relay01 27   //relay 1
#define Relay02 26   // relay 2
#define buzzer  25     // Buzzer
//--- device/sensor OFF board ------
#define distanceSensor  36
#define tiltSensor  5

#define motorA1  17
#define motorA2  12
#define motorB1  18
#define motorB2  19

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
//----------------------------------- end -------------------------------------------------------------

//----ISTIHAR SENSOR SUHU DS18B20 ---------------------------------------------------------------------
 
OneWire oneWire(SENSOR_PIN);
DallasTemperature DS18B20(&oneWire);
//----------------------------------- end -------------------------------------------------------------


//---- PENETAPAN WIFI ---------------------------------------------------------------------------------
//------WIFI PTSB---
/*const char* ssid = "PTSB STAFF";  
const char* password = ""; */

//------WIFI AZA---
//const char* ssid = "Alias FredoX_2.4GHz@unifi";  
//const char* password = "0135335045"; 

//------WIFIiphone AZA---
const char* ssid = "HaMa iPhone 13";  
const char* password = "1234556790"; 

//-----WiFi KKSBJ --------------------------------
//const char* ssid = "GUEST";  
//const char* password = ""; 

//onst char* ssid = "GalaxyA3";  
//onst char* password = "1234509876"; 

WiFiClient espClient;                                                                          
PubSubClient client(espClient); //MQTT
const char* mqtt_server = "broker.hivemq.com";

//----------------------------------- end -------------------------------------------------------------

//---- PEMBOLEHUBAH DATA---------------------------------------------------------------------------------
int dataDistanceSensor;
float tempC; // temperature in Celsius
char varPotString[8];
long lastMsg = 0;
char msg[50];
int value = 0;

int tiltState = 0;
//----------------------------------- end -------------------------------------------------------------

// Setting PWM properties
const int freq = 5000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 255;
int vSpeed=200;


void setup() {

  pinMode(Relay01,OUTPUT);
  pinMode(Relay02,OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(tiltSensor, INPUT);

  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);

  ledcSetup(3, 5000, 8); // channel number(0-15), frequensi 5000 hz PWM, 8-bit 
  ledcSetup(4, 5000, 8); // 5000 hz PWM, 8-bit resolution

  //--------setting channet bagi setiap nilai PWM------------//
  ledcAttachPin(motorA1, 3); 
  ledcAttachPin(motorA2, 4); 

  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  DS18B20.begin();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("ESP32 Project BOARD");
//  display.setCursor(40, 20);
  display.display();

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  delay(100);  

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("ESP32 Project BOARD");
//  display.setCursor(40, 20);
  display.display();
  delay(500);
  
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();

  if (now - lastMsg > 1000) {  
    lastMsg = now;

//-------------------------------------------------------------------------------
//                     -------------------suhu --------------------------
  Serial.println("-----------------URBAN FARMING ------------------");  
  DS18B20.requestTemperatures();       // send the command to get temperatures
  tempC = DS18B20.getTempCByIndex(0);  // read temperature in °C
  Serial.print("Temperature: ");
  Serial.print(tempC);    // print the temperature in °C
  Serial.println("°C");
   
  dtostrf(tempC, 1, 1, varPotString);
  client.publish("fishfeederR/suhu", varPotString);
    
//---------------------------------------------------end --------------------------
//----------- Sensor Food Level ----------------------------------
Serial.println("-----------------Makanan Ikan ------------------");
  float volts = analogRead(distanceSensor) * 0.0008056640625; // value from sensor * (3.3/4096)
  int distance = 29.988 * pow( volts, -1.173);


  if (distance <= 80) {
    Serial.println(distance);
    dtostrf(distance, 1, 1, varPotString);
    client.publish("fishfeederR/foodtank", varPotString);
  }
 //-----------end ----------------------------------

//----------- Sensor Var ----------------------------------

  tiltState = digitalRead(tiltSensor);
 Serial.println(tiltState);
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (tiltState == HIGH) {
    // turn LED on:
    digitalWrite(buzzer, HIGH);
    
   client.publish("fishfeederR/tanklid", "Food Lid is OPEN");
  } else {
    // turn LED off:
    digitalWrite(buzzer, LOW);
    client.publish("fishfeederR/tanklid", "Food Lid is CLOSE");
  }


}


}

 