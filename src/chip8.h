#pragma once
#define memory_size 4096
#define v_size 16

#include "display.h"

#include<stack>
#include<map>

class chip8{
private:

public:
  //Methods:
  void load_rom(char rom_dir[], unsigned char * memory);
  void cycle();

  //Various declarations
  uint8_t memory[memory_size];
  uint8_t v[v_size];
  uint16_t address;
  std::stack<uint16_t> stack;

  uint16_t opcode;
  uint16_t pc;

  //Timer registers:
  int delay=0;
  int sound=0;

  display * screen_ptr;

  //Misc:
  const int fontoffset = 0x050;
  bool debug = false;

  std::map<uint8_t, std::string> lookup = {
    {0x0, "DISPLAY/FLOW"},
    {0x1, "FLOW"},
    {0x2, "FLOW"},
    {0x3, "COND"},
    {0x4, "COND"},
    {0x5, "COND"},
    {0x6, "CONST"},
    {0x7, "CONST"},
    {0x8, "ASSIG/BITOP/MATH",},
    {0x9, "COND"},
    {0xA, "MEM"},
    {0xB, "FLOW"},
    {0xC, "RAND"},
    {0xD, "DISPLAY"},
    {0xE, "KEYOP"},
    {0xF, "MISC"},
  };
  uint8_t keys_map[16] = {
      SDLK_1,
      SDLK_2,
      SDLK_3,
      SDLK_4,
      SDLK_q,
      SDLK_w,
      SDLK_e,
      SDLK_r,
      SDLK_a,
      SDLK_s,
      SDLK_d,
      SDLK_f,
      SDLK_z,
      SDLK_x,
      SDLK_c,
      SDLK_v,
  };

  //Constructor and destructor
  chip8(display* screen);
  ~chip8();

};
