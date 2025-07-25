#include <stdio.h>
#include "pico/stdlib.h"

#include "bsp/board.h"
#include "tusb.h"
#include "class/hid/hid_device.h"
#include "class/hid/hid.h"
#include "device/usbd.h"   
#include "tusb_config.h"
//#include "usb_descriptors.c"

#define GREEN_KEY 15
#define RED_KEY 14
#define YELLOW_KEY 13
#define BLUE_KEY 12
#define ORANGE_KEY 11

#define S_UP 17
//#define S_DOWN 18

int keys[] = {GREEN_KEY}; // RED_KEY, YELLOW_KEY, BLUE_KEY, ORANGE_KEY
int strum[] = {S_UP}; // S_DOWN};

bool green_button_pressed_last = false;
bool red_pressed_last = false;

void init_fretboard(void){
  for(int i=0; i<1; i++){
    gpio_init(keys[i]);
    gpio_set_dir(keys[i], GPIO_IN);
    gpio_pull_up(keys[i]);
  }
}

int main() {
  board_init();
  tusb_init();
  init_fretboard();

  while (true) {
    tud_task(); // TinyUSB device task

    // Read button state (active low with pull-up)
    bool green_button_pressed = !gpio_get(keys[0]);
    bool red_pressed = !gpio_get(keys[1]);
    
    // Send 'A' key when button is pressed (rising edge detection)
    if (tud_hid_ready() && green_button_pressed && !green_button_pressed_last) {
      uint8_t keycode[6] = {0};
      keycode[0] = HID_KEY_A;
      tud_hid_keyboard_report(0, 0, keycode);
    } else if(tud_hid_ready() && red_pressed && !red_pressed_last){
      uint8_t keycode[6] = {0};
      keycode[0] = HID_KEY_S;
      tud_hid_keyboard_report(0, 0, keycode);
    }
    
    
    // release key
    if (tud_hid_ready() && !green_button_pressed && green_button_pressed_last) {
      tud_hid_keyboard_report(0, 0, NULL); // Release key
    }
    if (tud_hid_ready() && !red_pressed && green_button_pressed_last) {
      tud_hid_keyboard_report(0, 0, NULL); // release
    }
    
    green_button_pressed_last = green_button_pressed;
    red_pressed_last = red_pressed;
    sleep_ms(1); // Small delay for debouncing
  }
}
