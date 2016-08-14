#include <Arduino_FreeRTOS.h>
#include <task.h>

#define PINLED 13

TaskHandle_t hTaskPrint = NULL;

void task_blink(void *params);
void task_print(void *params);

void setup() {
  Serial.begin(9600);
  pinMode(PINLED, OUTPUT);
  BaseType_t rc = xTaskCreate(task_blink, "blink", 128, NULL, 3, NULL);
  if (rc != pdPASS) {
    Serial.println("fail");
    delay(1000);
    exit(0);
  }
  rc = xTaskCreate(task_print, "print", 128, NULL, 2, &hTaskPrint);
  if (rc != pdPASS) {
    Serial.println("fail2");
    delay(1000);
    exit(0);
  }
}

void loop() {
  // empty
  //delay(60000);
  //vTaskDelete(hTaskPrint);
  //delay(60000);
}

void task_blink(void *params) {
  (void) params;
  TickType_t period = 800 / portTICK_PERIOD_MS;
  TickType_t xLastWakeTime = xTaskGetTickCount();
  for (;;) {
    digitalWrite(PINLED, HIGH);
    for (volatile uint32_t i = 0; i < 5530; i++) {}
    //vTaskDelay(3000 / portTICK_PERIOD_MS);
    digitalWrite(PINLED, LOW);
    for (volatile uint32_t i = 0; i < 5530; i++) {}
    //vTaskDelay(500 / portTICK_PERIOD_MS);
    vTaskDelayUntil(&xLastWakeTime, period);
  }
}

void task_print(void *params) {
  (void) params;
  TickType_t period = 500 / portTICK_PERIOD_MS;
  TickType_t xLastWakeTime = xTaskGetTickCount();
  for (;;) {
    Serial.print(xTaskGetTickCount());
    Serial.print('\n');
    //delay(400);
    vTaskDelayUntil(&xLastWakeTime, period);
    //vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

