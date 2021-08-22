             
const int pirPin = D1; 


void setup() {  //Nodered setup 
 pinMode(pirPin, INPUT); 

 Serial.begin(115200);
 
}

void loop(){
  
 if (digitalRead(pirPin) == HIGH) {                // if motion detected
   Serial.println("Hey I got you!!!"); //Send notification
   delay(1000);
  } 
 else {
   Serial.println("Where u at?!!!");   // turn LED OFF if we have no motion
   delay(1000);
  }
 } 
