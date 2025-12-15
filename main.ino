#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "DHT.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define MOTOR 13
#define DHTPIN 23
#define DHTTYPE DHT22
#define OLED_SCL 22
#define OLED_SDA 21
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C

float h = 0.0;
float t = 0.0;
float v = 0.0;

DHT dht(DHTPIN, DHTTYPE);

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

TaskHandle_t task1Handle = NULL;
TaskHandle_t task2Handle = NULL;
TaskHandle_t task3Handle = NULL;

void vTask1(void *pvParameters);
void vTask2(void *pvParameters);
void vTask3(void *pvParameters);

void setup() {
  Serial.begin(9600);

  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();

  dht.begin();

  xTaskCreate(vTask1, "TASK1", 2048, NULL, 1, &task1Handle);
  xTaskCreate(vTask2, "TASK2", 2048, NULL, 2, &task2Handle);
  xTaskCreate(vTask3, "TASK3", 4096, NULL, 3, &task3Handle);
}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(3000));
}

void vTask1(void *pvParameters) {
  pinMode(MOTOR, INPUT);
  while (1) {
    v = analogRead(MOTOR)/4096; //A depender do medidor, deve-se realizar a conversão correta
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

void vTask2(void *pvParameters) {
  while (1) {
    h = dht.readHumidity();
    t = dht.readTemperature();
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

void vTask3(void *pvParameters) {
  while (1) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);

    display.println("Velocidade do vento:");
    display.println(v);

    display.println("Umidade (%):");
    display.println(h);

    display.println("Temperatura (C):");
    display.println(t);

    display.display();
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}