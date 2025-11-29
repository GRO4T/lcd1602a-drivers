import unittest

from .lcd1602a import Lcd, Pins


class MockLcd(Lcd):
    def __init__(self, pinout: Pins):
        self._pins = {}
        super().__init__(pinout)

    def _gpio_set(self, pin: int, value: int):
        self._pins[pin] = value

    def _gpio_read(self, pin: int) -> int:
        return self._pins[pin]

    def _gpio_setup(self, pin: int):
        pass

    def _delay_ms(self, milliseconds: int):
        pass


class TestLcd(unittest.TestCase):
    def test_send(self):
        # given
        pinout = Pins(14, 13, 12, 11, [10, 9, 46, 3, 8, 18, 17, 16], 15, 7)
        lcd = MockLcd(pinout)

        # when
        lcd._send(0b00010101, 1)

        # then
        self.assertEqual(lcd._pins[pinout.data_pins[0]], 1)
        self.assertEqual(lcd._pins[pinout.data_pins[1]], 0)
        self.assertEqual(lcd._pins[pinout.data_pins[2]], 1)
        self.assertEqual(lcd._pins[pinout.data_pins[3]], 0)
        self.assertEqual(lcd._pins[pinout.data_pins[4]], 1)
        self.assertEqual(lcd._pins[pinout.data_pins[5]], 0)
        self.assertEqual(lcd._pins[pinout.data_pins[6]], 0)
        self.assertEqual(lcd._pins[pinout.data_pins[7]], 0)
