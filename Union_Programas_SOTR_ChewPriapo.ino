#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>  

const char WIFI_SSTD[] = "Static-X";
const char WIFI_PASSWORD[] = "H3RM3SCH3W";

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 3600;

 //ultrasonic
 #define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701 
const int trigPin = 14;
const int echoPin = 2;

long duration;
float distanceOn;
float distanceInch;

String fecha = "0";
void GetDate(void);
void Ultrasonix(void);
//variable de control

int control = 0;
void GetDate(void);
void Ultrasonix(void);
void FireBase(void);
////////////////////////////
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include <addons/TokenHelper.h>

//Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */

//For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyBiwCOplNh9yuApUsJN-tYDuV-QfujHMhc"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://esp32upiiz-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "USER_EMAIL"
#define USER_PASSWORD "USER_PASSWORD"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;
void setup() {
  Serial.begin(9600);
  WiFi.begin(WIFI_SSTD, WIFI_PASSWORD);
  Serial.println("Connecting");
  while(WiFi.status()!=WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }

  control = 1;
  Serial.println(" Conection successful");
  Serial.print("Conectando con la siguiente dirección IP: ");
  Serial.println(WiFi.localIP());
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin,LOW);

   Serial.begin(9600);

    WiFi.begin(WIFI_SSTD, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    /* Assign the api key (required) */
    config.api_key = API_KEY;

    /* Assign the user sign in credentials */
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    /* Assign the RTDB URL (required) */
    config.database_url = DATABASE_URL;

    /* Assign the callback function for the long running token generation task */
    config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

    //Or use legacy authenticate method
    //config.database_url = DATABASE_URL;
    //config.signer.tokens.legacy_token = "<database secret>";

    Firebase.begin(&config, &auth);

    //Comment or pass false value when WiFi reconnection will control by your code or third party library
    Firebase.reconnectWiFi(true);
}


void loop() {
  while(1){
    GetDate();
    Ultrasonix();
    FireBase();
    delay(5000);
  }

}

void GetDate (void){
  struct tm timeinfo;
  //Int and get the date
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  
  char timeYear[5];
  strftime(timeYear,5,"%Y", &timeinfo);
  char timeMonth[15];
  strftime(timeMonth,15,"%B", &timeinfo);
  char timeDay[10];
  strftime(timeDay,10,"%d", &timeinfo);
  char timeHour[3];
  strftime(timeHour,3,"%H", &timeinfo);
  char timeMin[3];
  strftime(timeMin,3,"%M", &timeinfo);
  fecha = String(timeYear)+String(timeMonth)+String(timeDay)+String("  ")+String(timeHour)+String(":")+String(timeMin);
  Serial.println(fecha);
}

void Ultrasonix(void){
  //limpiar el trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  //poner el trigger en alto 10 microsegundos
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  //leer el pin echo
  duration = pulseIn(echoPin, HIGH);

  //calcular la distancias
  distanceOn = duration * SOUND_SPEED/2;

  //IMprimimos la distancia
  Serial.print("Disance (cm): ");
  Serial.println(distanceOn);
}
void FireBase(void){
  //Flash string (PROGMEM and  (FPSTR), String C/C++ string, const char, char array, string literal are supported
    //in all Firebase and FirebaseJson functions, unless F() macro is not supported.

    if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
    {
        sendDataPrevMillis = millis();

        FirebaseJson dist;
        dist.setDoubleDigits(4);
        dist.add("value", distanceOn);

        Serial.printf("Set dist... %s\n", Firebase.RTDB.setJSON(&fbdo, "/test/distancia", &dist) ? "ok" : fbdo.errorReason().c_str());

        FirebaseJson date;
        date.setDoubleDigits(12);
        date.add("value", fecha);

        FirebaseJson json;
        json.setDoubleDigits(3);
        json.add("value",count);
        
        Serial.printf("Get json... %s\n", Firebase.RTDB.getJSON(&fbdo, "/test/json") ? fbdo.to<FirebaseJson>().raw() : fbdo.errorReason().c_str());

        FirebaseJson jVal;
        Serial.printf("Get json ref... %s\n", Firebase.RTDB.getJSON(&fbdo, "/test/json", &jVal) ? jVal.raw() : fbdo.errorReason().c_str());

        FirebaseJsonArray arr;
        arr.setFloatDigits(2);
        arr.setDoubleDigits(4);
        arr.add("a", "b", "c", true, 45, (float)6.1432, 123.45692789);

        Serial.printf("Set array... %s\n", Firebase.RTDB.setArray(&fbdo, "/test/array", &arr) ? "ok" : fbdo.errorReason().c_str());

        Serial.printf("Get array... %s\n", Firebase.RTDB.getArray(&fbdo, "/test/array") ? fbdo.to<FirebaseJsonArray>().raw() : fbdo.errorReason().c_str());

        //Serial.printf("Push json... %s\n", Firebase.RTDB.pushJSON(&fbdo, "/test/push", &json) ? "ok" : fbdo.errorReason().c_str());

        json.set("value", count + 0.29745);
        Serial.printf("Update json... %s\n\n", Firebase.RTDB.updateNode(&fbdo, "/test/push/" + fbdo.pushName(), &json) ? "ok" : fbdo.errorReason().c_str());

        count++;
    }
  }
