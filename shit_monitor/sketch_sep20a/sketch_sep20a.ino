#include <WiFi.h>
#include <HTTPClient.h>

// Конфігурація Wi-Fi
const char* ssid = "UPC_SHIT";
const char* password = "Qa20061990";


const String baseUrl = "https://api.telegram.org/bot7361150434:AAF9RUt4nX3mH3UA2OXQY30AZoO4XO2abOM/sendMessage?chat_id=-1002275862214&text=";

// Піни
const int LED_0 = 0;//wifi ok
const int LED_1 = 1;//wifi not ok
const int LED_2 = 2; // clean 
const int LED_3 = 5; //button
const int BUTTON_PIN = 3;
const int PIR_SENSOR_PIN = 4;
const int DELAY_TIME_SEC = 240;
const int RESPONSIBILITY_LEVEL = 4;

// Змінні
int visits = 0;
bool motionDetected = false;
bool shouldSendFlag = true;
unsigned long motionStartTime = 0;
unsigned long totalMotionTime = 0;


void setup() {
  Serial.begin(115200);
  
  // Налаштовуємо піни
  pinMode(LED_0, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(PIR_SENSOR_PIN, INPUT);

  // Підключаємося до Wi-Fi
  if (connectToWiFi()) {
    digitalWrite(LED_0, HIGH);  
    sendLogRequest("Shit Counter is back online");// Підключення успішне
  } else {
    digitalWrite(LED_1, HIGH);  // Підключення не вдалося
  }
}

void loop() {
  
  resetCounters();
  checkMotion();
  // Якщо підійшли до умов для надсилання GET-запиту
  if (visits  >= RESPONSIBILITY_LEVEL  && shouldSendFlag) {
    digitalWrite(LED_2, HIGH);
    digitalWrite(LED_3, HIGH);  
    if (connectToWiFi()) {
      sendGETRequest();
    }
    shouldSendFlag = false;
  }
}

// Функція для підключення до Wi-Fi з 10 спробами
bool connectToWiFi() {
  int attempts = 0;
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED && attempts < 10) {
    delay(1000);
    attempts++;
    Serial.println("Connecting to WiFi...");
  }

  return WiFi.status() == WL_CONNECTED;
}

// Перевіряємо датчик руху
void checkMotion() {
  resetCounters();
  if (digitalRead(PIR_SENSOR_PIN) == HIGH) {
    visits ++;
    shouldSendFlag = true;
    waitAndCheckReset();
    }

  }


// Відправляємо GET-запит
void sendGETRequest() {
  HTTPClient http;
  String message = "You should clean the toilet.  cats visited toilet: " + String(visits) + " times. @weduy_gorn , @YuliaaD";
  String url = baseUrl + message;
  http.begin(url);
  
  int httpResponseCode = http.GET();
  
  if (httpResponseCode > 0) {
    Serial.println("GET request sent successfully");
   
  } else {
    Serial.println("Error sending GET request");
  }
  
  http.end();
}

// Відправляємо GET-запит
void sendLogRequest(String message) {
  HTTPClient http;
  
  String url = baseUrl + message;
  http.begin(url);
  
  int httpResponseCode = http.GET();
  
  if (httpResponseCode > 0) {
    Serial.println("GET request sent successfully");
   
  } else {
    Serial.println("Error sending GET request");
  }
  
  http.end();
}

void waitAndCheckReset() {
  for (int i = 0; i < DELAY_TIME_SEC * 10 ; i++) {  
    delay(100);                  
    resetCounters(); 
  }
}



// Скидання лічильників
void resetCounters() {
  if (digitalRead(BUTTON_PIN) == LOW) {
  visits = 0;
  sendLogRequest("HURRAY. Toilet was cleaned");
  digitalWrite(LED_2, LOW); 
  delay(500); 
  digitalWrite(LED_2, HIGH); 
  delay(500);  
  digitalWrite(LED_2, LOW); 
  delay(500); 
  digitalWrite(LED_2, HIGH); 
  delay(500);  
  digitalWrite(LED_2, LOW); 
  delay(500); 
  digitalWrite(LED_2, HIGH); 
  delay(500);  
  digitalWrite(LED_2, LOW); 
  delay(500); 
  digitalWrite(LED_2, HIGH); 
  delay(500);  
  digitalWrite(LED_2, LOW);
  digitalWrite(LED_3, LOW);  
  shouldSendFlag = true;
  delay(10000); 
  } 
}