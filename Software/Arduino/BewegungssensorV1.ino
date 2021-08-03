             
const int led = BUILTIN_LED;
const int red = D4;
const int btn = D3;
const int green = D2;
const int pirPin = D1; 

boolean state = true;
int buttonpressed = 0; 
int pirStat = 0;  

void setup() {  //Nodered setup 
 pinMode(led, OUTPUT); 
 pinMode(red, OUTPUT); 
 pinMode(green, OUTPUT);  
 pinMode(pirPin, INPUT); 
 
 digitalWrite(led, HIGH);  
 digitalWrite(red, LOW); 
 digitalWrite(green, HIGH);  
   
 Serial.begin(115200);
 
}

void buttonpressed(){
  buttonpressed = digitalRead(btn);
  if(buttonpressed = HIGH){
  state = false;
  }
}

void loop(){
  
buttonpressed();

if(state == false){
  digitalWrite(green, LOW);
  digitalWrite(red, HIGH);

// if(time < 30000){
// buttonpressed();} else ...   Use timer to wait if time > 30 then continue, else if buttonpressed();

  pirStat = digitalRead(pirPin); 
 if (pirStat == HIGH) {                // if motion detected
   Serial.println("Hey I got you!!!"); //Send notification
   delay(1000);
  } 
 else {
   Serial.println("Where u at?!!!");   // turn LED OFF if we have no motion
   delay(1000);
  }
 }

 if(state == true){
  digitalWrite(green, HIGH);
  digitalWrite(red, LOW);
 }
} 
