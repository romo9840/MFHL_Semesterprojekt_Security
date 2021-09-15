#include <ESP8266WiFi.h>
#include <PubSubClient.h>
      
const int red = D6;           // Rote led Output zu D4   
// Taster Input von D3 später in Button Klasse 
const int green = D2;         // Gruene led Output zu D2  
const int pirPin = D1;        // Sensor Input von D1
// Init von weitern Variabeln 
boolean state = true;              // True = "Home", False = "Away"  
boolean nightMode = false;         // Nightmode mit Long Press (1 Sek -> LEDs ausschalten)
int counter = 0;                   // Bei state = false, regelt timer-logik
unsigned long timeToWait = 120000; // 2min Wartezeit sobald state=false
unsigned long STime = 0;           // Für Zeitmessung
unsigned long CTime = 0;           // "  "


//----------NETWORK SETUP--------------//
//folgende Parameter anpassen. 
//--------------------------------
const char* ssid = "";
const char* password = "";
const char* mqtt_topic_subscribe1 = "MFHLSensor";
const char* mqtt_topic_publish = "MFHLSensor";
//--------------------------------

//folgende vier Parameter nicht ändern
//--------------------------------
const char* mqtt_server = "mqtt.iot.informatik.uni-oldenburg.de";
const int mqtt_port = 2883;
const char* mqtt_user = "sutk";
const char* mqtt_pw = "SoftSkills";
//--------------------------------

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  publishString("Connected");
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    // Create a random client ID: Client ID MUSS inviduell sein, da der MQTT Broker nicht mehrere Clients mit derselben ID bedienen kann
    String clientId = "Client-";
    clientId += String(random(0xffff), HEX);

    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pw)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.subscribe(mqtt_topic_subscribe1);
 
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
//----------NETWORK SETUP ENDE-------------//

//----------BUTTON CLASS-------------------//
//  https://www.hackster.io/Spivey/wemos-d1-mini-esp8266-smart-iot-button-with-messagebird-dbef7e

typedef struct Buttons {
  const byte pin = D3;                    //Button pin definiert
  const int debounce = 100;               //Delay nach Betätigung des Buttons
  const unsigned long shortPress = 100;   //Festlegung der Druckdauer (für normale Betätigungen)
  const unsigned long  longPress = 1000;  //"     " (für den Nachtmodus/LEDs Ausschalten)

  unsigned long counter = 0;      //Zählt Dauer des Drucks        
  bool previousState = HIGH;      //Vorheriger Zustand (Button ist active-low)
  bool currentState;              //Aktueller Zustand
} Button;

//----------BUTTON CLASS ENDE-------------------//

// create a Button variable type
Button button;

//----------EXTRA METHODEN-------------------//
void handleShortPress() {           
    Serial.println("short Press"); 
    state = !state;
 }


  void handleLongPress() {
    Serial.println("Long Press"); 
    digitalWrite(green, LOW);
    digitalWrite(red, LOW);
    nightMode = !nightMode;
  
 }
 
  void publishString(String payload){               
    char payload_buff[payload.length() + 1];
    payload.toCharArray(payload_buff, payload.length() + 1);
    client.publish(mqtt_topic_publish, payload_buff);
 }

//----------EXTRA METHODEN ENDE-------------------//

//----------SETUP-------------------//
void setup() {  
 pinMode(red, OUTPUT);        // Rote LED
 pinMode(green, OUTPUT);      // Grüne LED
 pinMode(pirPin, INPUT);      // Bewegungsensor
 pinMode(button.pin, INPUT);  // Taster als Eingang setzten 
 digitalWrite(red, LOW);      // Rote LED aus
 digitalWrite(green, HIGH);   // Grüne LED an

 Serial.begin(115200);

 setup_wifi();                
 client.setServer(mqtt_server, mqtt_port);
}
//----------SETUP ENDE-------------------//


//----------LOOP METHODE-------------------//
// -- Falls "Away" Sensordaten werden wahrgenommen
// -- Falls "Home" Sensordaten werden ignoriert
// Buttonlogik von Spivey (link oben)

void loop() {

  if (!client.connected()) {        // Erkennt Verbindungsverlust
    reconnect();
  }
  client.loop();
  
//--Buttonlogik--//
button.currentState = digitalRead(button.pin);          // Erfasst Buttonzustand

  if (button.currentState != button.previousState) {    // Prüft ob aktueller Zustand mit dem voherigen Zustand nicht übereinstimmt
    delay(button.debounce);                             // Delay
    button.currentState = digitalRead(button.pin);      // Erfasst Buttonzustand erneut
    if (button.currentState == LOW) {                   // Falls Button gedrückt wird Zähler mit millis gestartet
      button.counter = millis();
    } 
    else if (button.currentState == HIGH) {            // Falls Button nicht gedrückt/losgelassen, wird aktuelle Zeit erfasst
      unsigned long currentMillis = millis();          

      if ((currentMillis - button.counter >= button.shortPress) && !(currentMillis - button.counter >= button.longPress)) {
        // Falls ein Button kurz betätigt wird
        handleShortPress();
      }
      else if ((currentMillis - button.counter >= button.longPress)) {
        // Falls ein Button länger als 1 Sekunde betätigt wird
        handleLongPress();
      }
    }
    button.previousState = button.currentState;      // Voheriger Zustand wird mit dem aktuellen überschrieben
  }
//--Ende Buttonlogik--//

//--Sensorlogik--//
    if(state == true){              // Falls "Home" Sensordaten werden ignoriert; Grüne LED leuchtet            
      counter = 0;                  // Zähler zurückgesetzt
      if(nightMode == false){
        digitalWrite(green, HIGH);
        digitalWrite(red, LOW);
      }
    }

    if(state == false){           // Falls "Away" Sensordaten werden wahrgenommen; Rote LED leuchtet
      if(nightMode == false){
        digitalWrite(green, LOW);
        digitalWrite(red, HIGH);
      }
      if(counter == 0){           // counter = 0 -> Startzeit erfasst für 2min Timer dann counter = 1
        STime = millis();
        counter = 1;  
        Serial.println("Counter = 1; Startzeit erfasst");           
      }
      if(counter == 1){          // counter = 1 -> Berechnet ob 2min überschritten. Falls ja counter = 2
        CTime = millis() - STime;     
        if (CTime >= timeToWait){
          counter = 2;
          Serial.println("Counter = 2; 2min überschritten");
         }
      }
      if(digitalRead(pirPin) == LOW && counter == 2){  //counter = 2 -> wartet bis Bewegungsensor auf Low
          counter = 3;
          Serial.println("Counter = 3 Sensor Calibration Complete");  
      }

    if (digitalRead(pirPin) == HIGH && counter == 3) {   //counter = 3 -> Erfasst Bewegungsensordaten.  
      // Falls Bewegung erfasst wird Nachricht an Node-RED gesendet (weitergeleitet auf Discord und Email)
      Serial.println("Hey I got you!!!; Counter = 4");                 
      publishString("Intrusion detected by " + String(mqtt_topic_publish) + " on ");
      counter = 4;
     } 

     if (digitalRead(pirPin) == LOW && counter == 4){ //counter = 4 -> Falls keine weitere Bewegung erkannt, wird Zähler zurückgesetzt
      counter = 0;
     }
  }
//--Ende Sensorlogik--//
}
//----------ENDE LOOP METHODE-------------------//
