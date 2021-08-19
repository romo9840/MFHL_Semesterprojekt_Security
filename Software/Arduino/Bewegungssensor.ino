#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//Interrupts
//https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
            
const int led = BUILTIN_LED;  
const int red = D4;           // Rote led Output zu D4   
const int btn = D3;           // Taster Input von D3  
const int green = D2;         // Gruene led Output zu D2  
const int pirPin = D1;        // Sensor Input von D1

volatile boolean state = true;    //Volatile wegen Verwndung von Interrupt. True: Grün/Home; False: Rot/Away
int buttonpressed = 0;            // Init von weitern Variabeln 
int pirStat = 0; 
int counter = 0;
unsigned long STime = 0;
unsigned long CTime = 0;

//folgende Parameter anpassen. 
//--------------------------------
const char* ssid = "";
const char* password = "";
const char* mqtt_topic_subscribe1 = "MFHLSensor";
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

void setup() {  //Nodered setup noch nötig
 //Pinmodes setzen wie oben erwähnt
 pinMode(led, OUTPUT); 
 pinMode(red, OUTPUT); 
 pinMode(green, OUTPUT);  
 pinMode(pirPin, INPUT); 
 //Button als InteruptPin setzen der bei eine Signalaenderung Low -> High den ISR buttonpressed ausführt
 pinMode(btn, INPUT);
 attachInterrupt(digitalPinToInterrupt(btn), buttonpressed, RISING);
//Initialisiert LEDs. BuiltinLED led ist aktiv LOW
 digitalWrite(led, HIGH);  
 digitalWrite(red, LOW); 
 digitalWrite(green, HIGH);  
   
 Serial.begin(115200);
}

//Method to check whether the button has been pressed.
void buttonpressed(){ 
  state = !state;
}

//Main method
// -- If away/red sensor is activated
// -- If home/green sensor is ignored 
// TO DO: Add timer logic
void loop(){

while(state == false){                    // if away
  digitalWrite(green, LOW);               // Turn off green LED
  digitalWrite(red, HIGH);                // Turn on red LED
  
  if(digitalRead(pirPin) == HIGH && counter == 0) {   // Allows time for the sensor to reset.
    counter = 1;
    STime = millis();
    do {
    CTime = millis() - STime;    
    } while (CTime < 120000) // waits 2 mins.
  }
  
  if (digitalRead(pirPin) == HIGH && counter == 1) {   // if motion detected
   Serial.println("Hey I got you!!!");                 // Send notification
   client.publish("MFHLSensor", "Hey I got you!!!");
   counter = 2;
  } 
  
 
// else {
//   Serial.println("Where u at?!!!");   // turn LED OFF if we have no motion
//   
//   delay(1000);
//  }
 }

 if(state == true){                       // if home
  counter = 0;
  digitalWrite(red, LOW);                 // Turn off red LED
  digitalWrite(green, HIGH);              // Turn on green LED
 }
} 
