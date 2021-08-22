#include <ESP8266WiFi.h>
#include <PubSubClient.h>
      
const int red = D4;           // Rote led Output zu D4   
// Taster Input von D3 später in Button Klasse 
const int green = D2;         // Gruene led Output zu D2  
const int pirPin = D1;        // Sensor Input von D1
// Init von weitern Variabeln 
boolean state = true;    
int pirStat = 0; 
int counter = 0;
unsigned long timeToWait = 1200000;
unsigned long STime = 0;
unsigned long CTime = 0;

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

//---------------------------------------------
typedef struct Buttons {
  const byte pin = D3;
  const int debounce = 100;
  const unsigned long shortPress = 100;
  const unsigned long doublePress = 600;
  const unsigned long  longPress = 1000;

  unsigned long counter = 0;
  int shortPressAmount = 0;
  bool previousState = HIGH;
  bool currentState;
} Button;

// create a Button variable type
Button button;

void setup() {  //Nodered setup noch nötig
 //Pinmodes setzen wie oben erwähnt 
 pinMode(red, OUTPUT); 
 pinMode(green, OUTPUT);  
 pinMode(pirPin, INPUT);
 pinMode(button.pin, INPUT); //Taster als Eingang setzten 
 digitalWrite(red, LOW); 
 digitalWrite(green, HIGH);

 Serial.begin(115200);
 setup_wifi();  
 client.setServer(mqtt_server, mqtt_port);
}



//Main method
// -- If away/red sensor is activated
// -- If home/green sensor is ignored 
// TO DO: Add timer logic
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
    //Status des Tasters auslesen und als Wert (HIGH / LOW) setzen.

button.currentState = digitalRead(button.pin);

  if (button.currentState != button.previousState) {
    delay(button.debounce);
    // update status in case of bounce
    button.currentState = digitalRead(button.pin);
    if (button.currentState == LOW) {
      button.counter = millis();
    }
    else if (button.currentState == HIGH) {
      unsigned long currentMillis = millis();

      if ((currentMillis - button.counter >= button.shortPress) && !(currentMillis - button.counter >= button.longPress)) {
        // short press detected, add press to amount
        button.shortPressAmount++;
      }
      else if ((currentMillis - button.counter >= button.longPress)) {
        // long press was detected
        handleLongPress();
      }
    }
    button.previousState = button.currentState;
  }

  else if (button.shortPressAmount == 1) {
    unsigned long currentMillis = millis();
    if (currentMillis - button.counter >= button.doublePress) {
      handleShortPress();
      button.shortPressAmount = 0;
    }
  }

    if(state == true){
      counter = 0;
      digitalWrite(green, HIGH);
      digitalWrite(red, LOW);
   
    }

    if(state == false){
      digitalWrite(green, LOW);
      digitalWrite(red, HIGH);
      Serial.println(counter);
      if(counter == 0){   // Allows time for the sensor to reset.
        STime = millis();
        counter = 1;
      }
      if(counter == 1){ 
        CTime = millis() - STime; 
        if (CTime >= timeToWait){
          counter = 2;
          Serial.println("Counter = 2");
         }
      }

    if (digitalRead(pirPin) == HIGH && counter == 2) {   // if motion detected
    Serial.println("Hey I got you!!!");                 // Send notification
    publishString("Intrusion detected by " + String(mqtt_topic_publish) + " on ");
    counter = 3;
     } 
  }
}

  void handleShortPress() {
   Serial.println("short Press"); 
   state = !state;
 }


  void handleLongPress() {
  Serial.println("Long Press"); 
  
  }
 

    void publishString(String payload){
    char payload_buff[payload.length() + 1];
    payload.toCharArray(payload_buff, payload.length() + 1);
    client.publish(mqtt_topic_publish, payload_buff);
  }
