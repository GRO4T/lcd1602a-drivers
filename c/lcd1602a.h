#ifndef __LCD1602A_H
#define __LCD1602A_H

#include <stdlib.h>

#define LCD1602A_PIN_T uint8_t
#define LCD1602A_VALUE_T uint8_t

const LCD1602A_VALUE_T LCD1602A_COMMAND_MODE  = 0;
const LCD1602A_VALUE_T LCD1602A_DATA_MODE     = 1;
const LCD1602A_VALUE_T LCD1602A_WRITE_MODE    = 0;
const LCD1602A_VALUE_T LCD1602A_READ_MODE     = 1;

const uint8_t LCD1602A_COMMAND_CLEAR  = 0b00000001;
const uint8_t LCD1602A_FUNCTION_SET   = 0b00111000;

struct lcd1602a_pins {
  LCD1602A_PIN_T contrast;     // Vo
  LCD1602A_PIN_T data_command; // RS (Register Select)
  LCD1602A_PIN_T read_write;   // RW
  LCD1602A_PIN_T clock;        // E
  LCD1602A_PIN_T d0;
  LCD1602A_PIN_T d1;
  LCD1602A_PIN_T d2;
  LCD1602A_PIN_T d3;
  LCD1602A_PIN_T d4;
  LCD1602A_PIN_T d5;
  LCD1602A_PIN_T d6;
  LCD1602A_PIN_T d7;
  LCD1602A_PIN_T led_plus;
  LCD1602A_PIN_T led_minus;
};

struct lcd1602a_lcd {
  int (*gpio_set)(LCD1602A_PIN_T pin, LCD1602A_VALUE_T value);
  int (*gpio_read)(LCD1602A_PIN_T pin, LCD1602A_VALUE_T* value);
  int (*gpio_setup)(LCD1602A_PIN_T pin);
  void (*delay)(uint8_t ms);
  struct lcd1602a_pins* pinout;
};

void lcd1602a_send(struct lcd1602a_lcd* lcd, uint8_t value, LCD1602A_VALUE_T register_select) {
  lcd->gpio_set(lcd->pinout->data_command, register_select);
  lcd->gpio_set(lcd->pinout->read_write, LCD1602A_WRITE_MODE);

  lcd->gpio_set(lcd->pinout->d0, (value & 1)   ? 1 : 0);
  lcd->gpio_set(lcd->pinout->d1, (value & 2)   ? 1 : 0);
  lcd->gpio_set(lcd->pinout->d2, (value & 4)   ? 1 : 0);
  lcd->gpio_set(lcd->pinout->d3, (value & 8)   ? 1 : 0);
  lcd->gpio_set(lcd->pinout->d4, (value & 16)  ? 1 : 0);
  lcd->gpio_set(lcd->pinout->d5, (value & 32)  ? 1 : 0);
  lcd->gpio_set(lcd->pinout->d6, (value & 64)  ? 1 : 0);
  lcd->gpio_set(lcd->pinout->d7, (value & 128) ? 1 : 0);

  lcd->gpio_set(lcd->pinout->clock, 0);
  lcd->delay(1);
  lcd->gpio_set(lcd->pinout->clock, 1);
  lcd->delay(1);
  lcd->gpio_set(lcd->pinout->clock, 0);
}

void lcd1602a_send_command(struct lcd1602a_lcd* lcd, uint8_t command) {
  lcd1602a_send(lcd, command, LCD1602A_COMMAND_MODE);
}

void lcd1602a_init(struct lcd1602a_lcd* lcd) {
  lcd->gpio_setup(lcd->pinout->contrast);
  lcd->gpio_setup(lcd->pinout->data_command);
  lcd->gpio_setup(lcd->pinout->read_write);
  lcd->gpio_setup(lcd->pinout->clock);
  lcd->gpio_setup(lcd->pinout->d0);
  lcd->gpio_setup(lcd->pinout->d1);
  lcd->gpio_setup(lcd->pinout->d2);
  lcd->gpio_setup(lcd->pinout->d3);
  lcd->gpio_setup(lcd->pinout->d4);
  lcd->gpio_setup(lcd->pinout->d5);
  lcd->gpio_setup(lcd->pinout->d6);
  lcd->gpio_setup(lcd->pinout->d7);
  lcd->gpio_setup(lcd->pinout->led_plus);
  lcd->gpio_setup(lcd->pinout->led_minus);

  lcd1602a_send_command(lcd, LCD1602A_FUNCTION_SET);
  lcd1602a_send_command(lcd, LCD1602A_COMMAND_CLEAR);
  lcd1602a_send_command(lcd, 0b00000110); // Increment and shift cursor
  lcd1602a_send_command(lcd, 0b00001110); // Turn on display
  lcd1602a_send_command(lcd, LCD1602A_COMMAND_CLEAR);
}

void lcd1602a_print_char(struct lcd1602a_lcd* lcd, char c) {
  lcd1602a_send(lcd, c, LCD1602A_DATA_MODE);
}

void lcd1602a_print(struct lcd1602a_lcd* lcd, char* const text) {
  for (const char* c = text; *c != '\0'; c++) {
    lcd1602a_print_char(lcd, *c);
  }
}

#endif
