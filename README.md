
# 1602A (16x2) LCD — Drivers (non-I2C)

Clean, hardware-agnostic drivers for the popular 1602A 16x2 character LCD (parallel
mode, not I2C). The driver is designed so it can be
adapted easily to different MCUs by providing a few thin GPIO/delay wrappers.

Highlights
- Hardware-agnostic C driver for 1602A (4-bit / 8-bit parallel modes)
- Easy to adapt to any microcontroller by supplying GPIO and delay callbacks
- Example projects and Wokwi samples included for quick testing

Usage
-----

The driver expects a small platform layer to be provided by the user:
- a function to set GPIO output level
- a function to read GPIO input level (optional for busy-wait)
- a small delay function with millisecond granularity
- an optional GPIO setup/init function

These allow the core driver to remain portable — it never depends on a specific
HAL or SDK.

ESP32-S3 example
-----------------

Below is a minimal example showing how to adapt the driver for the ESP32-S3
API. Supply these wrapper functions and then create the `lcd` instance with
your pin mapping.

```c
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

  lcd1602a_init(&lcd);
  lcd1602a_print(&lcd, "Hello World!");
}
```

Wiring / pin mapping
---------------------

The exact pin order depends on how the `struct lcd1602a_pins` is defined in
`lcd1602a.h`.

For a quick reference image showing an example connection in Wokwi see:

![](esp32_s3.png)

Wokwi samples
-------------

The `wokwi_samples` directory contains ready-to-run examples you can copy into
Wokwi (https://wokwi.com) to try the driver in simulation. Copy the files from
`wokwi_samples/c_esp32-s3/` for the ESP32-S3 sample.

Resources
---------

- 1602A LCD Datasheet: https://www.openhacks.com/uploadsproductos/eone-1602a1.pdf

Contributing
------------

Contributions are welcome. If you add ports for other languages or boards,
please open a PR and add a short example to `wokwi_samples` if possible.

License
-------

This repository is provided under the terms in the `LICENSE` file.

