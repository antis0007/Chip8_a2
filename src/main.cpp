#include<iostream>

#include "chip8.h"
#include "display.h"
//#include <chrono>
int main(int argc, char ** argv){

  display screen;
  //Display Setup and Main Loop:
  char screen_name[] = "Chip8_a2";
  int width = 64;
  int height = 32;
  int pitch = width*sizeof(uint8_t)*4; //"The length of a surface scanline in bytes."
  int pixels_size = pitch*height;

  uint8_t * pixels = new uint8_t[pixels_size] {};

  // std::cout << "NEW PTR 1:" << std::endl;
  // std::cout << &pixels << std::endl;
  screen.set_pixel_all(pixels, pixels_size, 0x00); //Helper function to memset, sets all pixels to black
  screen.display_setup(screen_name, width, height, &pixels, pixels_size, pitch); //Set up our screen

  //uint8_t * temp_pixels = NULL;
  //int temp_pitch;


  bool quit = false;
  //char rom_dir [] = {"IBM Logo.ch8"};
  //char rom_dir [] = {"test_opcode.ch8"};
  //char rom_dir [] = {"game_of_life.ch8"};
  //char rom_dir [] = {"nyancat2.ch8"};
  //char rom_dir [] = {"snake.ch8"};
  //char rom_dir [] = {"invaders.rom"};
  //char rom_dir [] = {"pong.rom"};
  char rom_dir [] = {"tetris.rom"};
  //test_opcode.ch8

  chip8 cpu(&screen);
  cpu.load_rom(rom_dir, cpu.memory);

  SDL_Event event;
  int cycles = 3;
  int cycles_count = 0;
  while (!quit)
  {
    //auto start = std::chrono::high_resolution_clock::now();
    //Main render and processing loop
    SDL_RenderPresent(screen.render);
    SDL_LockTexture(screen.texture, NULL, (void **)&pixels, &pitch); // Lock full texture
    //Apply pixel operations:

    //set_pixels(pixels, pixels_size, 0xFF);
    cpu.cycle();
    cycles_count++;
    if(cycles_count == cycles){
      //SDL_Delay(16);
      cycles_count = 0;
      if (cpu.delay > 0)
        cpu.delay--;
    }
    //screen.set_pixel_all(pixels, pixels_size, 0xFF);

    //End pixel operations:
    SDL_UnlockTexture(screen.texture);

    SDL_RenderClear(screen.render);
    SDL_RenderCopy(screen.render, screen.texture, NULL, NULL);

    SDL_PollEvent(&event);
    //std::cout<< "BEGIN INPUT"<<std::endl;
    switch (event.type)
    {
      case SDL_QUIT:
        quit = true;
        break;
      case SDL_KEYDOWN:
          if (event.key.keysym.sym == SDLK_ESCAPE){
            quit = true;
            break;
          }
          for(int i = 0; i < 16; i++){
              if(event.key.keysym.sym == cpu.keys_map[i]){
                  screen.keys[i] = 1;
                  //std::cout<< " DOWN "<< std::endl;
              }
          }
          break;
      case SDL_KEYUP:
        for (int i = 0; i < 16; i++) {
          if (event.key.keysym.sym == cpu.keys_map[i]){
            screen.keys[i] = 0;
            //std::cout<< " UP "<<std::endl;
          }
        }
        break;
      break;
    }
    //std::cout<< "END INPUT ---"<<std::endl;
    // auto stop = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    //
    // std::cout << "Time taken by function: "
    //      << duration.count() << " microseconds" << std::endl;

  }
  delete[] pixels;
  SDL_DestroyTexture(screen.texture);
  SDL_DestroyRenderer(screen.render);

return(0);
}
