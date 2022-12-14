/* Código de manejo de tareas de un sistema operativo en tiempo real para aprecial la diferencia
de como se le da prioridad a una tarea con una frecuencia mayor y a la otra se la menos, esto será visualizado
a través de LEDs, por lo generál una tarea tendrá el doble de aparción (Tarea 1 de mayor freceuncia), y de vez en cuando 
aparecerá la segunda tarea (Tarea 2 de menor freceuncia)
*/ 

//Primero se configuran los núcleos
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else 
static const BaseType_t app_cpu =1;
#endif

//Asignación de pines de la ESP32 para LEDs y sus toggleos
int led_pin1 = 2; 
static const int led_pin2 = 4;

static const int rate_1 = 600;
static const int rate_2 = 300;

//Toggleo del LED 1 a 600ms
void toggleLED_1(void *parameter) {
  while(1) {
    digitalWrite(led_pin2, HIGH);
    vTaskDelay(rate_1 / portTICK_PERIOD_MS);
    digitalWrite(led_pin2, LOW);
    vTaskDelay(rate_1 / portTICK_PERIOD_MS);
  }
}

//Toggleo del LED 2 a 300ms
void toggleLED_2(void *parameter) {
  while(1) {
    digitalWrite(led_pin1, HIGH);
    vTaskDelay(rate_2 / portTICK_PERIOD_MS);
    digitalWrite(led_pin1, LOW);
    vTaskDelay(rate_2 / portTICK_PERIOD_MS);
  }
}


void setup() {  
  pinMode(led_pin1, OUTPUT);
  pinMode(led_pin2, OUTPUT);

//Creación de tareas para cada toggleo de los LED, pri ero LED 1 y luego LED 2
xTaskCreatePinnedToCore(  
              toggleLED_1,  
              "Toggle 1",   
              1024,         
              NULL,         
              1,            
              NULL,         
              app_cpu);
                   
xTaskCreatePinnedToCore(  
              toggleLED_2,  
              "Toggle 2",   
              1024,         
              NULL,         
              2,            
              NULL,         
              app_cpu);     
}

void loop() {
