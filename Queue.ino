/* En este código hacemos uso de queue, la es es la lsita de espera, la cual nos sirve para poder en fila de espera
 *  diferentes tareas, manejamos dos parámetros importantes, el tiempo de espera las tareas, y el tiempo en que tarda
 *  en refresacrse, deben estar en equilibrio para que una sola tarea no se repita hasta el refresh, y para que no se
 *  llegue al límite de tareas en lista de espera*/

// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
//static const BaseType_t app_cpu = 1;
#endif

// Settings
static const uint8_t msg_queue_len = 5;

// Globals
static QueueHandle_t msg_queue;

// Tasks

void printMessages(void *parameters) {

  int item;

  // Loop forever
  while (1) {

    
    if (xQueueReceive(msg_queue, (void *)&item, 0) == pdTRUE) {
    }
    Serial.println(item);

    // Wait before trying again
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// Main 

void setup() {

// Configuración serial
  Serial.begin(115200);

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Queue Demo---");

  msg_queue = xQueueCreate(msg_queue_len, sizeof(int));

 // Impresión de tareas
  xTaskCreatePinnedToCore(printMessages,
                          "Imprimir mensajes",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);
}

void loop() {

  static int num  = 0;

//Por el momento la lisat de tareas máximas a almacenar son 10
  if (xQueueSend(msg_queue, (void *)&num, 10) != pdTRUE) {
    Serial.println("Queue full");
  }
  num++;

  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
