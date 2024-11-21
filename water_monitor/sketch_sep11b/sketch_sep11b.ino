#include <WiFi.h>
#include <HTTPClient.h>


const char* ssid = "UPC_SHIT";
const char* password = "Qa20061990";


const String url = "https://api.telegram.org/bot7361150434:AAF9RUt4nX3mH3UA2OXQY30AZoO4XO2abOM/sendMessage?chat_id=-1002275862214&text=";


const int led1 = 0;
const int led2 = 1;
const int led3 = 2;


unsigned long startMillis; 
unsigned long currentMillis; 


unsigned long firstReminderInterval = 36 * 60 * 60 * 1000;
unsigned long secondReminderInterval = 48 * 60 * 60 * 1000; 
unsigned long delaybetweenReminders =  4 * 60 * 60 * 1000;

bool greetingsSent = false;
bool firstReminder = false;
unsigned long lastMessageTime;

void setup() {

  Serial.begin(115200);
  
  pinMode(led1, OUTPUT); // wifi  ok
  pinMode(led2, OUTPUT); // wifi not ok 
  pinMode(led3, OUTPUT); //status code not ok
  
  connectToWiFi(); 
  startMillis = millis();
}

void loop() {
  currentMillis = millis();
  digitalWrite(led1, HIGH);
  digitalWrite(led2, LOW);
  
  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFi();
  }

  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
  } else {
 
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
  }
  if (!greetingsSent){
    sendMessage("!!!!Hurray WATER was changed!!!!!");
    greetingsSent = true;
  }


  if (currentMillis - startMillis >= firstReminderInterval && !firstReminder) {
    sendMessage("Water was not changed for 36 hours");
    firstReminder = true;
  } else if (currentMillis - startMillis >= secondReminderInterval) {
  
    if ((currentMillis - startMillis) % delaybetweenReminders < 1000) {  
      unsigned long hoursElapsed = (currentMillis - startMillis)/(60*60*1000);
      String message = "Water was not changed for " + String(hoursElapsed) + " hours @weduy_gorn , @YuliaaD " ;
      sendMessage(message);
    }
  }
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  int attempts = 0;
  
  while (WiFi.status() != WL_CONNECTED && attempts < 10) {
    delay(1000);
    digitalWrite(led2, HIGH);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to WiFi");
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
  } else {
    Serial.println("Failed to connect to WiFi");
    digitalWrite(led2, HIGH);
    digitalWrite(led1, LOW);
  }
}

void sendMessage(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String fullUrl = url + message;
    
    http.begin(fullUrl);  
    int httpCode = http.GET();
    
    if (httpCode != 200) {
      blinkLed(led3);
    }
    
    http.end(); 
    delay(10000);
  } else {
    connectToWiFi();
    HTTPClient http;
    String fullUrl = url + message;
    
    http.begin(fullUrl);  
    int httpCode = http.GET();
    
    if (httpCode != 200) {

      blinkLed(led3);
    }

    http.end(); 
    delay(10000);
  }
}


void blinkLed(int ledPin) {
  for (int i = 0; i < 5; i++) {  
    digitalWrite(ledPin, HIGH);  
    delay(500);                  
    digitalWrite(ledPin, LOW);   
    delay(500);  
    digitalWrite(ledPin, HIGH);  //to see unsuccess for a while 
  }
}