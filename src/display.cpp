#include"display.h"
#include<stdio.h>
#include<cstring>
#include<iostream>

//display::display(){

//}
void display::set_pixel(uint8_t * pixels, int pixel){
  pixels[pixel*4] = 0xFF;
  pixels[pixel*4+1] = 0xFF;
  pixels[pixel*4+2] = 0xFF;
  pixels[pixel*4+3] = 0xFF;
}

void display::set_pixel_all(uint8_t * pixels, int pixels_size, uint8_t value){
  //std::cout<<"TESTING"<<std::endl;
  memset(pixels, value, pixels_size*sizeof(uint8_t));
  //for(int i = 0; i++; i<pixels_size){
    //set_pixel(pixels, i);
  //}
  //std::cout<<"TESTING 2"<<std::endl;
}

void display::clear_pixel(uint8_t * pixels, int pixel){
  pixels[pixel*4] = 0x00;
  pixels[pixel*4+1] = 0x00;
  pixels[pixel*4+2] = 0x00;
  pixels[pixel*4+3] = 0x00;
}

bool display::flip_pixel(uint8_t * pixels, int pixel){
  uint32_t val = ((pixels[pixel*4] << 24) | (pixels[pixel*4+1] << 16) | (pixels[pixel*4+2] << 8) | (pixels[pixel*4+3]));
  pixels[pixel*4] = ~pixels[pixel*4];
  pixels[pixel*4+1] = ~pixels[pixel*4+1];
  pixels[pixel*4+2] = ~pixels[pixel*4+2];
  pixels[pixel*4+3] = ~pixels[pixel*4+3];
  if((val & 0xFFFFFFFF) == 0xFFFFFFFF){
    return(true);
  }
  else{
    return(false);
  }
}

void display::display_setup(char screen_name[], int width, int height, uint8_t ** pixels_ptr, int pixels_size, int pitch){
  SDL_Init(SDL_INIT_VIDEO);
  pixels = pixels_ptr; //Set our pixels pointer
  this->pixels_size = pixels_size;
  this->pitch = pitch;
  this->width = width;
  this->height = height;
  int width_scale = 32;
  int height_scale = 32;
  window = SDL_CreateWindow(screen_name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width*width_scale, height*height_scale, 0);
  render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  texture = SDL_CreateTexture(render,
    SDL_PIXELFORMAT_ARGB8888,
    SDL_TEXTUREACCESS_STREAMING,
    width, height);
}
void display::set_keys(){
}
