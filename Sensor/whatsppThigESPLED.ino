#include <ESP8266WiFi.h>
#include <ThingESP.h>

ThingESP8266 thing("govind", "ESP8266SmartBin", "123456789");

int LED = LED_BUILTIN;
const int trigPin = 2;  //D4
const int echoPin = 0;  //D3
// defines variables
long duration;
int distance;

bool firstTime = true;

unsigned long previousMillis = 0;
const long INTERVAL = 2000;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT); 
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  thing.SetWiFi("POCO X2", "asdfghjkl");
  thing.initDevice();
}

String HandleResponse(String query){
  if(query== "bin status of db321"){
//    digitalWrite(LED, 0);
      float perc = ((float)(22 - (float)distance)/22)*100;
      if(perc<0){
        perc = 100;
      }
    return "bin is " + String(perc) + "% full";
  }
  else if(query== "bin status"){
    float perc = ((float)(22 - (float)distance)/22)*100;
      if(perc<0){
        perc = 100;
      }
    return "bin is " + String(perc) + "% full";
  }
  else 
    return  "query is invalid, ask for bin status with its bin ID"; 
}

void loop() {
    
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    
    // Calculating the distance
    distance= duration*0.034/2;
    // Prints the distance on the Serial Monitor
//    Serial.print("Distance: ");
//    Serial.println(distance);
//    delay(2000);
  
  // put your main code here, to run repeatedly:
    if(millis()-previousMillis >= INTERVAL){
       Serial.print("Distance: ");
       Serial.println(distance);
      previousMillis = millis() ;

      float perc = ((float)(22 - (float)distance)/22)*100;
      Serial.print("percentage: ");
      Serial.print(perc);
      if(perc < 60){
        firstTime = true;
      }

      if(perc>60 && firstTime){
        String msg  = "I am Bin number DB321 in Darbhanga, about to full kindly empty the bin" ;
        thing.sendMsg("+918789400122", msg);
        firstTime = false;
      }
     }
  
  thing.Handle();
}
