
/*
Código para la conexión a WiFi de nuestra ESP32 en la que escribios, esta fue la primera versión del código que 
después pasaría a ser el código del Firebase
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>

const char WIFI_SSID[] = "TP-Link_953A";
const char WIFI_PASSWORD[] = "52232473";

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 3600;

//------Ultrasonix------//
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701 
const int trigPin = 14;
const int echoPin = 2;

long duration;
float distanceOn;
float distanceInch;
// ---------------------//

String fecha = "0";

void GetDate(void);
void Ultrasonix(void);

//Variable de control
int control = 0;

void setup() {
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
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
}

void loop() {
  while(1){
    GetDate();
    Ultrasonix();
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
