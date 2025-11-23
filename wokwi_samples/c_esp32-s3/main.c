#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "lcd1602a.h"

int esp32_gpio_setup(LCD1602A_PIN_T pin) {
    gpio_reset_pin(pin);
    gpio_set_direction(pin, GPIO_MODE_OUTPUT);
    return 0;
}

int esp32_gpio_set(LCD1602A_PIN_T pin, LCD1602A_VALUE_T value) {
    return gpio_set_level(pin, value);
}

int esp32_gpio_read(LCD1602A_PIN_T pin, LCD1602A_VALUE_T* value) {
    *value = gpio_get_level(pin);
    return 0;
}

void esp32_delay_ms(int ms) {
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

void app_main() {
  struct lcd1602a_pins pinout = {
    14, 13, 12, 11, 10, 9, 46, 3, 8, 18, 17, 16, 15, 7
  };
  struct lcd1602a_lcd lcd = {
    &esp32_gpio_set, &esp32_gpio_read, &esp32_gpio_setup, &esp32_delay_ms, &pinout
  };

  lcd1602a_init(
    &lcd,
    LCD1602A_OP_8_BIT | LCD1602A_OP_2_LINE | LCD1602A_OP_5_8_FONT,
    LCD1602A_CURSOR_OP_ON
  );
  lcd1602a_print(&lcd, "Hello World!");

  while (true) {
  }
}