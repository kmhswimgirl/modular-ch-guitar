#include <stdio.h>
#include <string.h> 

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
#define BLUE_KEY 11
#define ORANGE_KEY 12

#define S_UP 10
#define S_DOWN 18

#define STARPOWER 16
#define SELECT
#define OPTIONS

int keys[] = {GREEN_KEY, RED_KEY, YELLOW_KEY, BLUE_KEY, ORANGE_KEY};
int strum[] = {S_UP};
int function[] = {STARPOWER};

void init_fretboard(void){
  for(int i=0; i<5; i++){
    gpio_init(keys[i]);
    gpio_set_dir(keys[i], GPIO_IN);
    gpio_pull_up(keys[i]);
  }
}

void init_strumbar(void){
  for(int i=0; i<1; i++){
    gpio_init(strum[i]);
    gpio_set_dir(strum[i], GPIO_IN);
    gpio_pull_up(strum[i]);
  }
}

void init_function_keys(void){
  for(int i=0; i<1; i++){
    gpio_init(function[i]);
    gpio_set_dir(function[i], GPIO_IN);
    gpio_pull_up(function[i]);
  }
}

// fret booleans
bool green_pressed_last = false;
bool red_pressed_last = false;
bool yellow_pressed_last = false;
bool blue_pressed_last = false;
bool orange_pressed_last = false;

//strumbar booleans
bool up_pressed_last = false;

//special key booleans
bool starpower_pressed_last = false;

int main() {
  board_init();
  tusb_init();
  init_fretboard();
  init_strumbar();
  init_function_keys;

  while (true) {
    tud_task(); // TinyUSB device task

    // get state
    bool green_pressed = !gpio_get(keys[0]);
    bool red_pressed = !gpio_get(keys[1]);
    bool yellow_pressed = !gpio_get(keys[2]);
    bool blue_pressed = !gpio_get(keys[3]);
    bool orange_pressed = !gpio_get(keys[4]);

    bool up_pressed = !gpio_get(strum[0]);

    bool starpower_pressed = !gpio_get(function[0]);

    if(tud_hid_ready()){ //make sure mc is ready 
  
      uint8_t keycode[6] = {0}; //wipe array
      uint8_t key_count = 0;
      
      if (green_pressed && key_count < 6) {
        keycode[key_count++] = HID_KEY_A;
      }
      if (red_pressed && key_count < 6) {
        keycode[key_count++] = HID_KEY_S;
      }
      if (yellow_pressed && key_count < 6) {
        keycode[key_count++] = HID_KEY_D;
      }
      if (blue_pressed && key_count < 6) {
        keycode[key_count++] = HID_KEY_F;  
      }
      if (orange_pressed && key_count < 6) {
        keycode[key_count++] = HID_KEY_G;  
      }
      if (up_pressed && key_count < 6) {
        keycode[key_count++] = HID_KEY_ARROW_UP;  
      }
      if (starpower_pressed && key_count < 6) {
        keycode[key_count++] = HID_KEY_H;  
      }
      tud_hid_keyboard_report(0, 0, keycode);
    }

    green_pressed_last = green_pressed;
    red_pressed_last = red_pressed;
    yellow_pressed_last = yellow_pressed;
    blue_pressed_last = blue_pressed;
    orange_pressed_last = orange_pressed;
    up_pressed_last = up_pressed;
    starpower_pressed_last = starpower_pressed;

    sleep_ms(1); // debouncing
  }
}