
/**
 * Created by K. Suwatchai (Mobizt)
 * 
 * Email: k_suwatchai@hotmail.com
 * 
 * Github: https://github.com/mobizt
 * 
 * Copyright (c) 2021 mobizt
 *
 *Este código sirve para enlazar nuestra ESP32 a nuestra base de datos creada en Firebase
 *en la que se guardan datos en tiempo real de un sesnor ultra sónico, adicionalmente se puede
 *obtener la fecha y hora del servidor al que se encuentra enlazado nuestra base de datos
 *
*/

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
// Importante poner nuestra red y contraseña
#define WIFI_SSID "TP-Link_953A"
#define WIFI_PASSWORD "52232473"

//For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
// API Key de nuestra base datos
#define API_KEY "AIzaSyBiwCOplNh9yuApUsJN-tYDuV-QfujHMhc"

/* 3. Define the RTDB URL */

// URL de nuestra base de datos
#define DATABASE_URL "https://esp32upiiz-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
// correo y conttraseña usados para nuestra base de datos
#define USER_EMAIL "priapo.alejandro@gmail.com"
#define USER_PASSWORD "PriapoRules1"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;


// Código principal
void setup()
{

    Serial.begin(115200);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
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
    //Uso de datos pregiamente definidos
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

void loop()
{
    //Flash string (PROGMEM and  (FPSTR), String C/C++ string, const char, char array, string literal are supported
    //in all Firebase and FirebaseJson functions, unless F() macro is not supported.

    if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
    {
        sendDataPrevMillis = millis();

        FirebaseJson json;
        json.setDoubleDigits(3);
        json.add("value", count);

        Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, "/test/json", &json) ? "ok" : fbdo.errorReason().c_str());

        Serial.printf("Get json... %s\n", Firebase.RTDB.getJSON(&fbdo, "/test/json") ? fbdo.to<FirebaseJson>().raw() : fbdo.errorReason().c_str());

        FirebaseJson jVal;
        Serial.printf("Get json ref... %s\n", Firebase.RTDB.getJSON(&fbdo, "/test/json", &jVal) ? jVal.raw() : fbdo.errorReason().c_str());

        FirebaseJsonArray arr;
        arr.setFloatDigits(2);
        arr.setDoubleDigits(4);
        arr.add("a", "b", "c", true, 45, (float)6.1432, 123.45692789);

        Serial.printf("Set array... %s\n", Firebase.RTDB.setArray(&fbdo, "/test/array", &arr) ? "ok" : fbdo.errorReason().c_str());

        Serial.printf("Get array... %s\n", Firebase.RTDB.getArray(&fbdo, "/test/array") ? fbdo.to<FirebaseJsonArray>().raw() : fbdo.errorReason().c_str());

        Serial.printf("Push json... %s\n", Firebase.RTDB.pushJSON(&fbdo, "/test/push", &json) ? "ok" : fbdo.errorReason().c_str());

        json.set("value", count + 0.29745);
        Serial.printf("Update json... %s\n\n", Firebase.RTDB.updateNode(&fbdo, "/test/push/" + fbdo.pushName(), &json) ? "ok" : fbdo.errorReason().c_str());

        count++;
    }
}
void GetDate(void){
  struct tm timeinfo;
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  char timeYear[5];
  strftime(timeYear,5, "%Y", &timeinfo);
  char timeMonth[15];
  strftime(timeMonth,15, "%B", &timeinfo);
  char timeDay[10];
  strftime(timeDay,10, "%d", &timeinfo);
  char timeHour[3];
  strftime(timeHour,3, "%H", &timeinfo);
  char timeMin[3];
  strftime(timeMin,3, "%M", &timeinfo);
  fecha = String(timeYear)+String(timeMonth)+String(timeDay)+String(timeHour)+String(timeMin);
  Serial.println(fecha);
}
void ReadUltrasonico(void){
  //Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  //Sets the trigPin on HIGH state for 10 micro
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  //Reads the echoPin, returns the sound wave
  duration = pulseIn(echoPin, HIGH);

  //Calculate the distance
  distanceCm= duration * SOUND_SPEED/2;

  //Prints the distance in the Serial Monitor
  Serial.print("Distance (cm):");
  Serial.println(distanceCm);
}
