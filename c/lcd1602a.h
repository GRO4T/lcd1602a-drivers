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

void lcd1602a_send_command(struct lcd1602a_lcd* lcd, uint8_t command);
void lcd1602a_init(struct lcd1602a_lcd* lcd);
void lcd1602a_send(struct lcd1602a_lcd* lcd, uint8_t value, LCD1602A_VALUE_T register_select);
void lcd1602a_print_char(struct lcd1602a_lcd* lcd, char c);
void lcd1602a_print(struct lcd1602a_lcd* lcd, char* const text);

#endif
