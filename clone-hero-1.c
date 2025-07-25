#include <stdio.h>
#include "pico/stdlib.h"

#include "bsp/board.h"
#include "tusb.h"
#include "class/hid/hid_device.h"
#include "class/hid/hid.h"
#include "device/usbd.h"   
#include "tusb_config.h"

#define GREEN_KEY 13
#define RED_KEY 14
#define YELLOW_KEY 15
#define BLUE_KEY 12
#define ORANGE_KEY 11

#define S_UP 17
#define S_DOWN 18

#define STARPOWER
#define SELECT
#define OPTIONS

int keys[] = {GREEN_KEY, RED_KEY, YELLOW_KEY, BLUE_KEY, ORANGE_KEY};
int strum[] = {S_UP, S_DOWN};

void init_fretboard(void){
  for(int i=0; i<5; i++){
    gpio_init(keys[i]);
    gpio_set_dir(keys[i], GPIO_IN);
    gpio_pull_up(keys[i]);
  }
}

bool green_pressed_last = false;
bool red_pressed_last = false;
bool yellow_pressed_last = false;
bool blue_pressed_last = false;
bool orange_pressed_last = false;


int main() {
  board_init();
  tusb_init();
  init_fretboard();

  while (true) {
    tud_task(); // TinyUSB device task

    // Read button state (active low with pull-up)
    bool green_pressed = !gpio_get(keys[0]);
    bool red_pressed = !gpio_get(keys[1]);
    bool yellow_pressed = !gpio_get(keys[2]);
    bool blue_pressed = !gpio_get(keys[3]);
    bool orange_pressed = !gpio_get(keys[4]);

    if(tud_hid_ready()){ //make sure mc is ready 
      //handles when the button is pressed
      if (green_pressed && !green_pressed_last) {
        uint8_t keycode[6] = {0}; //clear array
        keycode[0] = HID_KEY_A; //map key to array
        tud_hid_keyboard_report(0, 0, keycode); //send keypress
      }

      if(red_pressed && !red_pressed_last){ //red key
        uint8_t keycode[6] = {0};
        keycode[0] = HID_KEY_S;
        tud_hid_keyboard_report(0, 0, keycode);
      }

      if(yellow_pressed && !yellow_pressed_last){ //yellow key
        uint8_t keycode[6] = {0};
        keycode[0] = HID_KEY_D;
        tud_hid_keyboard_report(0, 0, keycode);
      }

      // releasing the keys
      if (!green_pressed && green_pressed_last) {
        //release iff red is also not pressed
        if (!red_pressed) {
          tud_hid_keyboard_report(0, 0, NULL);
        } else {
          uint8_t keycode[6] = {0};
          keycode[0] = HID_KEY_S;
          tud_hid_keyboard_report(0, 0, keycode);
        }
      }

      if (!red_pressed && red_pressed_last) {
        // Only release if green is also not pressed  
        if (!green_pressed) {
          tud_hid_keyboard_report(0, 0, NULL);
        } else {
          // Keep green key pressed
          uint8_t keycode[6] = {0};
          keycode[0] = HID_KEY_A;
          tud_hid_keyboard_report(0, 0, keycode);
        }
      }
    }
  
    green_pressed_last = green_pressed;
    red_pressed_last = red_pressed;
    yellow_pressed_last = yellow_pressed;
    blue_pressed_last = blue_pressed;
    orange_pressed_last = orange_pressed;

    sleep_ms(1); // debouncing
  }
}