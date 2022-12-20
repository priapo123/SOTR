/*
Código de so de timers, ase progarmaron dos tareas, las cuales fueron dos timers
que muestran un mensaje por el monitor serial, uno lo hace otra vez, y otro indefinido tiempo,
yal mandar un mensaje específico por el monitor serie se reincia el timer
*/

//#include <timers.h>
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// Variables globales
static TimerHandle_t one_shot_timer = NULL;
static TimerHandle_t auto_reload_timer = NULL;

//*****************************************************************************
// Callbacks


void myTimerCallback(TimerHandle_t xTimer) {

  // Mensajes que se imprimiran en cad timer:
  
  if ((uint32_t)pvTimerGetTimerID(xTimer) == 0) {
    Serial.println("One-shot timer expired");
  }
  if ((uint32_t)pvTimerGetTimerID(xTimer) == 1) {
    Serial.println("Auto-reload timer expired");
  }
}

//*****************************************************************************
// Main (runs as its own task with priority 1 on core 1)

void setup() {

  // Configure Serial
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Timers---");

  // Primer timer, de uso único
  one_shot_timer = xTimerCreate(
                      "One-shot timer",           // Name of timer
                      2000 / portTICK_PERIOD_MS,  // Period of timer (in ticks)
                      pdFALSE,                    // Auto-reload
                      (void *)0,                  // Timer ID
                      myTimerCallback);           // Callback function

  // Segundo timer, repetible
  auto_reload_timer = xTimerCreate(
                      "Auto-reload timer",        // Name of timer
                      1000 / portTICK_PERIOD_MS,  // Period of timer (in ticks)
                      pdTRUE,                     // Auto-reload
                      (void *)1,                  // Timer ID
                      myTimerCallback);           // Callback function

  if (one_shot_timer == NULL || auto_reload_timer == NULL) {
    Serial.println("Could not create one of the timers");
  } else {

    vTaskDelay(1000 / portTICK_PERIOD_MS);
    Serial.println("Starting timers...");

    // Start timers
    xTimerStart(one_shot_timer, portMAX_DELAY);
    xTimerStart(auto_reload_timer, portMAX_DELAY);
  }

  // Delete self task to show that timers will work with no user tasks
  vTaskDelete(NULL);
}

void loop() {
  // Execution should never get here
  if (Serial.available()>0){
  char incoming = Serial.read();
  if (incoming != '\n'){
        Serial.println("reinicio de timer");
        xTimerStart(one_shot_timer, portMAX_DELAY);
  }
  }
}
