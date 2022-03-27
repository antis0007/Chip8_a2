//#pragma once
#ifndef DISPLAY_H
#define DISPLAY_H

//Cross-Platform Includes:
#ifdef __MINGW32__
//do some stuff for Windows
#include "../include/SDL2/SDL.h"
#elif __APPLE__
//do some stuff for Apple
#include<SDL2/SDL.h>
#elif __linux__
//do stuff for Linux
#include<SDL2/SDL.h>
#endif

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
#endif
