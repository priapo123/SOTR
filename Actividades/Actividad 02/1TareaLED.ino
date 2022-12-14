/*
Código para el toggleo de un LED a través de una única tarea en nuestra ESP32
 */
//#include <Arduino.h> 

// Configuracion de los núcelos a usar
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Se escoge el pin GPIO donde se manejará el LED
static const int led_pin = 2;

// Comandos de toggleo del LED así como del retraso de la tarea para que pueda ser apreciada
void toggleLED(void *parameter) {
  while(1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {

// Se pone el pin como salida
  pinMode(led_pin, OUTPUT);

// Se crea la tarea del toggleo de LED, la cual siempre se ejecutará
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
              toggleLED,    // Function to be called
              "Toggle LED", // Name of task
              1024,         // Stack size (bytes in ESP32, words in FreeRTOS)
              NULL,         // Parameter to pass to function
              1,            // Task priority (0 to configMAX_PRIORITIES - 1)
              NULL,         // Task handle
              app_cpu);     // Run on one core for demo purposes (ESP32 only)

}
void loop() {
}
