import time

from lcd1602a import CURSOR_OP_ON, OP_2_LINE, OP_5_8_FONT, OP_8_BIT, Lcd, Pins
from machine import Pin


class Esp32Lcd1602a(Lcd):
    def __init__(self, pins: Pins, options: int, cursor_opts: int):
        self._pin_objs = {}
        super().__init__(pins, options, cursor_opts)

    def _gpio_set(self, pin: int, value: int):
        self._pin_objs[pin].value(value)

    def _gpio_read(self, pin: int) -> int:
        return self._pin_objs[pin].value()

    def _gpio_setup(self, pin: int):
        pin_obj = Pin(pin, Pin.OUT)
        self._pin_objs[pin] = pin_obj

    def _delay_ms(self, milliseconds: int):
        time.sleep_ms(milliseconds)


pins = Pins(14, 13, 12, 11, [10, 9, 46, 3, 8, 18, 17, 16], 15, 7)
lcd = Esp32Lcd1602a(pins, OP_8_BIT | OP_2_LINE | OP_5_8_FONT, CURSOR_OP_ON)
lcd.print_text("Hello, World!\nESP32 LCD1602A")
