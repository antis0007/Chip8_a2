#pragma once
#include "../include/SDL2/SDL.h"

class display{
private:

public:
  //Methods:
  void set_pixel(uint8_t * pixels, int pixel);
  void set_pixel_all(uint8_t * pixels, int pixels_size, uint8_t value);
  void clear_pixel(uint8_t * pixels, int pixel);
  bool flip_pixel(uint8_t * pixels, int pixel);
  void display_setup(char screen_name[], int width, int height, uint8_t ** pixels_ptr, int pixels_size, int pitch);
  void set_keys();
  //Declarations:
  SDL_Window * window;
  SDL_Renderer * render;
  SDL_Texture * texture;

  uint8_t ** pixels; //A pointer to the pixels pointer
  int pixels_size; // copy of val
  int pitch; // copy of val
  int width;
  int height;

  uint8_t keys[16];


  //Misc:

  //Constructor and destructor
  //display();
  //~display();

};
