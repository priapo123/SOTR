// #include <Arduino.h>
/*
Este código crea un bot de telegran capaz de automarizar mensajesz y de recibir comandos lo cuales, por medio de Telegram,
pueden envira comando sdirectos a una ESP32, pudiendon manejar esta última a través de mensajes de Telegram, aquí específicamente
manejamos el LED de nuestra ESP32 a través de mensajes
*/

#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   
#include <ArduinoJson.h>

//Datos de la red para comunicación
const char* ssid = "TP-Link_312A";
const char* password = "Ramon123.";

//Datos de nuestro Bot creado
#define BOTtoken "5605250615:AAEw6lrjwKORUCWcM0rslajBW6hFsHiJRgQ"  

#define CHAT_ID "5467164458"

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);


int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

const int ledPin = 4;
bool ledState = LOW;


void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

//Mensajes por defecto para Telegram, que nos indican los comandos posibles
    if (text == "/start") {
      String welcome = "Bienvenido, " + from_name + ".\n";
      welcome += "Utilice los siguientes comando para interactuar con el ESP32.\n\n";
      welcome += "/led_on para encender el LED \n";
      welcome += "/led_off para apagar el LED \n";
      welcome += "/state para obtener el estado del LED \n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/led_on") {
      bot.sendMessage(chat_id, "LED ON", "");
      ledState = HIGH;
      digitalWrite(ledPin, ledState);
    }
    
    if (text == "/led_off") {
      bot.sendMessage(chat_id, "LED OFF", "");
      ledState = LOW;
      digitalWrite(ledPin, ledState);
    }
    
    if (text == "/state") {
      if (digitalRead(ledPin)){
        bot.sendMessage(chat_id, "LED esta apagado", "");
      }
      else{
        bot.sendMessage(chat_id, "LED esta encendido", "");
      }
    }
  }
}

void setup() {
  Serial.begin(115200);

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      
    client.setTrustAnchors(&cert); 
  #endif

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); 
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  
  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
