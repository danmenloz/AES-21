#include <stdio.h>
#include <gpiod.h>

int main() {
  struct gpiod_chip *chip;
  struct gpiod_line *in_line, *out_line;
  int req, value;

  chip = gpiod_chip_open("/dev/gpiochip0");
  if (!chip)
    return -1;
  in_line = gpiod_chip_get_line(chip, 27);
  if (!in_line) {
    gpiod_chip_close(chip);
    return -1;
  }
  req = gpiod_line_request_input(in_line, "gpio_state");
  if (req) {
    gpiod_chip_close(chip);
    return -1;
  }
  out_line = gpiod_chip_get_line(chip, 17);
  if (!out_line) {
    gpiod_chip_close(chip);
    return -1;
  }
  req = gpiod_line_request_output(out_line, "gpio_state", 0);
  if (req) {
    gpiod_chip_close(chip);
    return -1;
  }

  for (int i=0; i<1000000; i++) {
    value = gpiod_line_get_value(in_line);
    gpiod_line_set_value(out_line, value);
  }
  gpiod_chip_close(chip);
}
