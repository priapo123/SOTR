/*
En este código enlazamos dos tareas, la tarea 2 depedenderá de la tara 1, lo que se realizará en la primer tarea será
una lectura del monitor serie, en el que podemos escribir un mensaje, el cual será reproducido por la tarea 2 hasta que
se le detenga el programa con un reset
*/

// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

//Se crea el espacio para recibir el mensaje y se inicializan las tareas
String message = "";
static char incomingChar;
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

// Tarea 1: Lectura del monitor serie y entrada de texto
void startTask1(void *parameter) {

  while (1) {
    if (Serial.available()){
      char incomingChar = Serial.read();
      if (incomingChar != '\n'){
        message += String(incomingChar);
      }
      else{
        message = "";
      }
      Serial.write(incomingChar);
  }
}
}

// Tarea 2: Impresión serial del mensake obtenido en la tarea 1, hasta que el emnsaje sea "Reset", el cual reiniciará todo.
void startTask2(void *parameter) {
  while (1) {
        Serial.println(incomingChar);
         if (message == "Reset"){
            incomingChar='\n';
         }
    }
  }
 
void setup() {

  // Se inicia el serial de medio segundo de velocidad (puede bajar) y se deja un delay a las tareas
  Serial.begin(500);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

 // Tarea 1
  xTaskCreatePinnedToCore(startTask1,
                          "Tarea 1: Lectura del serial",
                          1024,
                          NULL,
                          1,
                          &task_1,
                          app_cpu);

  // Tarea 2
  xTaskCreatePinnedToCore(startTask2,
                          "Tarea 2: Repetición del mensaje",
                          1024,
                          NULL,
                          1,
                          &task_2,
                          app_cpu);
}

void loop() {
  
}
