MODE_COMMAND = 0
MODE_DATA = 1
MODE_WRITE = 0
MODE_READ = 1

COMMAND_CLEAR = 0b00000001
COMMAND_RETURN_HOME = 0b00000010
COMMAND_NEWLINE = 0b11000000

OP_8_BIT = 0b00110000
OP_4_BIT = 0b00100000
OP_2_LINE = 0b00101000
OP_1_LINE = 0b00100000
OP_5_11_FONT = 0b00100100
OP_5_8_FONT = 0b00100000

CURSOR_OP_ON = 0b00001010
CURSOR_OP_POSITION_ON = 0b00001001


class Pins:
    def __init__(
        self,
        contrast: int,
        register_select: int,
        read_write: int,
        enable: int,
        data_pins: list[int],
        led_plus: int,
        led_minus: int,
    ):
        self.contrast = contrast
        self.register_select = register_select
        self.read_write = read_write
        self.enable = enable
        self.data_pins = data_pins
        self.led_plus = led_plus
        self.led_minus = led_minus


class Lcd:
    def __init__(self, pins: Pins, options: int, cursor_opts: int):
        self.pins = pins

        self._gpio_setup(self.pins.contrast)
        self._gpio_setup(self.pins.register_select)
        self._gpio_setup(self.pins.read_write)
        self._gpio_setup(self.pins.enable)
        for data_pin in self.pins.data_pins:
            self._gpio_setup(data_pin)
        self._gpio_setup(self.pins.led_plus)
        self._gpio_setup(self.pins.led_minus)

        self._send_command(options)
        self._send_command(0b00000110)
        self._send_command(0b00001100 | cursor_opts)
        self._send_command(COMMAND_CLEAR)

    def print_text(self, text: str):
        for char in text:
            if char == "\n":
                self._send_command(COMMAND_NEWLINE)
            else:
                self.print_char(char)

    def print_char(self, char: str):
        self._send(ord(char), MODE_DATA)

    def return_home(self):
        self._send_command(COMMAND_RETURN_HOME)

    def _gpio_set(self, pin: int, value: int):
        pass  # Placeholder for GPIO set implementation

    def _gpio_read(self, pin: int) -> int:
        pass  # Placeholder for GPIO read implementation

    def _gpio_setup(self, pin: int):
        pass  # Placeholder for GPIO setup implementation

    def _delay_ms(self, milliseconds: int):
        pass  # Placeholder for delay implementation

    def _send_command(self, command: int):
        self._send(command, MODE_COMMAND)

    def _send(self, value: int, register_select: int):
        self._gpio_set(self.pins.register_select, register_select)
        self._gpio_set(self.pins.read_write, MODE_WRITE)

        for index, data_pin in enumerate(self.pins.data_pins):
            bit_value = (value >> index) & 0x01
            self._gpio_set(data_pin, bit_value)

        self._gpio_set(self.pins.enable, 0)
        self._delay_ms(1)
        self._gpio_set(self.pins.enable, 1)
        self._delay_ms(1)
        self._gpio_set(self.pins.enable, 0)
