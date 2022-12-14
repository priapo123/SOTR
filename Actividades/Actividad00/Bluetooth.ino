
/*
Código para la conexión y manejo de un LED de nuestra ESP32 a través del Bluetooth de la misma
*/

#include <BluetoothSerial.h>
#define LED1_GPIO 5

BluetoothSerial SerialBT;
void ConfigByBluetooth(void);

void setup() {
  pinMode(LED1_GPIO, OUTPUT);
  Serial.begin(115200);
  Serial.println("Inicio de codigo");
}

void loop() {
  ConfigByBluetooth();
}

void ConfigByBluetooth(void){
  //Inicializamos bluetooth
  String message = "";
  String message2 = "";
  char incomingChar; 
  char incomingChar2;
  bool BTHControl = true;
  SerialBT.begin("ESP32Priapo");
  while(BTHControl == true){
    Serial.println("Bluetooth disponible");
    if(SerialBT.available()){
      char incomingChar = SerialBT.read();
      if(incomingChar != '\n'){
        message += String(incomingChar);
      }
      else {
        message = "";
      }
      Serial.write(incomingChar);
    }
  if(message =="Reset"){
    BTHControl = false; 
  }
  if(Serial.available()){
    char incomingChar2 = Serial.read();
    if(incomingChar2 != '\n'){
      message2 += String(incomingChar2);
    }
    else {
      message2 ="";
    }
    SerialBT.write(incomingChar2);
  }
 }
  SerialBT.end();
}
