#include"chip8.h"

#include<iostream>
#include<fstream>
#include<cstring>
#include<vector>
#include<sstream>
//Note: Potentially heavy header
#include<stdlib.h>


//Constructor
chip8::chip8(display* screen){
  screen_ptr = screen;
  pc = 0x200; //Start of ROM and work RAM

  //From multigesture.net guide:
  //0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
  //0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
  //0x200-0xFFF - Program ROM and work RAM

  unsigned char font[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
  };
  // Load font:
  for(uint8_t i = 0; i < 80; i++){
    //i is our offset
    memory[fontoffset+i]=font[i];
  }
}
//Destructor
chip8::~chip8(){

}
void chip8::load_rom(char rom_dir[], unsigned char * memory_ptr){
  //​ Load game (Based on Kolby Load function)
  std::ifstream input(rom_dir, std::ios::binary);
  std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(input), {});
  for(unsigned int i = 0; i < buffer.size(); i++){
    //i is our offset
    memory_ptr[0x200+i]=buffer[i];
  }
  //Finished Load game
}
void chip8::cycle(){
  //std::cout << "NEW PTR 3: CHIP8" << std::endl;
  //std::cout << &((*screen_ptr).pixels) << std::endl;
  //(*screen_ptr).set_pixel_all(*(*screen_ptr).pixels, ((*screen_ptr).pixels_size), 0xFF);
  //std::cout << "FIRST CYCLE:" << std::endl;
  //Fetch:
  opcode = (memory[pc] << 8 | memory[pc + 1]);

  if(debug){
    std::cout << "CYCLE:" << std::endl;

    std::string result;
    std::stringstream sstream;

    std::cout << pc << std::endl;

    sstream << std::hex << pc;
    result = sstream.str();
    std::cout << result << std::endl;

    std::stringstream sstream2;
    result = "";
    sstream2 << std::hex << opcode;
    result = sstream2.str();
    std::cout << result << std::endl;
    std::cout << int((opcode & 0xF000)>>12) << std::endl;
    std::cout << lookup[int((opcode & 0xF000)>>12)] << std::endl;
  }
  //Decode:
  switch(opcode & 0xF000){

    case 0x0000:{
      //0NNN
      //No need to implement
      switch(opcode & 0x00FF){
        case 0x00E0:{
          //Clear display:
          (*screen_ptr).set_pixel_all(*(*screen_ptr).pixels, ((*screen_ptr).pixels_size), 0x00);
          break;
        }

        case 0x00EE:{
          //Return from subroutine
          uint16_t temp = (stack).top();
          if(temp){
            pc = temp;
            (stack).pop();
          }
          break;
        }
        break;
      }
      pc = pc+2;
      break;
    }

    case 0x1000:{
      //1NNN
      //Jumps to address NNN.
      pc = (opcode & 0x0FFF);
      break;
    }
    case 0x2000:{
      //2NNN
      //Calls subroutine at NNN.
      (stack).push(pc);
      pc = (opcode & 0x0FFF);
      break;
    }


    case 0x3000:{
      if((v[((opcode & 0x0F00)>>8)]) == (opcode & 0x00FF)){
        pc = pc+2;
      }
      pc = pc+2;
      break;
    }

    case 0x4000:{
      if((v[((opcode & 0x0F00)>>8)]) != (opcode & 0x00FF)){
        pc = pc+2;
      }
      pc = pc+2;
      break;
    }

    case 0x5000:{
      if((v[((opcode & 0x0F00)>>8)]) == (v[((opcode & 0x0F0)>>4)])){
        pc = pc+2;
      }
      pc = pc+2;
      break;
    }

    case 0x6000:{
      (v[((opcode & 0x0F00)>>8)]) = (opcode & 0x00FF);
      pc = pc+2;
      break;
    }

    case 0x7000:{
      (v[((opcode & 0x0F00)>>8)]) += (opcode & 0x00FF);
      pc = pc+2;
      break;
    }

    case 0x8000:{
      switch(opcode & 0x000F){

        case 0x0000:
          (v[((opcode & 0x0F00)>>8)]) = (v[((opcode & 0x00F0)>>4)]);

          break;

        case 0x0001:
          (v[((opcode & 0x0F00)>>8)]) |= (v[((opcode & 0x00F0)>>4)]);

          break;

        case 0x0002:
          (v[((opcode & 0x0F00)>>8)]) &= (v[((opcode & 0x00F0)>>4)]);

          break;

        case 0x0003:
          (v[((opcode & 0x0F00)>>8)]) ^= (v[((opcode & 0x00F0)>>4)]);

          break;

        case 0x0004:
          if((v[((opcode & 0x0F00)>>8)]) < (v[((opcode & 0x00F0)>>4)])){
            (v)[0xF] = 1; //carry
          }
          else{
            (v)[0xF] = 0;
            (v[((opcode & 0x0F00)>>8)]) += (v[((opcode & 0x00F0)>>4)]);
          }

          break;

        case 0x0005:
          if((v[((opcode & 0x0F00)>>8)]) > (v[((opcode & 0x00F0)>>4)])){
            (v)[0xF] = 1; //carry
          }
          else{
            (v)[0xF] = 0;
            (v[((opcode & 0x0F00)>>8)]) -= (v[((opcode & 0x00F0)>>4)]);
          }

          break;

        case 0x0006:
          (v)[0xF] = (v)[((opcode & 0x0F00)>>8)]&1; //Save LSB to v[0xF]
          (v)[((opcode & 0x0F00)>>8)] >>= 1;

          break;

        case 0x0007:
          if((v[((opcode & 0x0F00)>>8)]) < (v[((opcode & 0x00F0)>>4)])){
            (v)[0xF] = 1; //carry
          }
          else{
            (v)[0xF] = 0;
            (v[((opcode & 0x0F00)>>8)]) = (v[((opcode & 0x00F0)>>4)]) - (v[((opcode & 0x0F00)>>8)]);
          }

          break;
        case 0x000E:
          uint8_t msb = 1 << ((sizeof(uint8_t) * 8) - 1);
          (v)[0xF] = (v)[((opcode & 0x0F00)>>8)] & msb; //Save MSB to v[0xF]
          (v)[((opcode & 0x0F00)>>8)] <<= 1;

          break;
        break;
      }
      pc = pc+2;
      break;
    }


    case 0x9000:{
      if((v[((opcode & 0x0F00)>>8)]) != (v[((opcode & 0x0F0)>>4)])){
        pc = pc+2;
      }
      pc = pc+2;
      break;
    }

    case 0xA000:{
      address = opcode & 0x0FFF;
      pc = pc+2;
      break;
    }

    case 0xB000:{
      pc = (v)[0x0] + (opcode & 0x0FFF);
      pc=pc+2;
      break;
    }

    case 0xC000:{
      v[((opcode & 0x0F00)>>8)] = (rand() % 255) & (opcode & 0x00FF);
      pc = pc+2;
      break;
    }

    case 0xD000:{
      //Draw command
      //std::cout << "WAIT 1 SECOND" << std::endl;
      //SDL_Delay(1000);

      uint8_t x = v[((opcode & 0x0F00)>>8)] & uint8_t((screen_ptr)->width-1);
      uint8_t y = v[((opcode & 0x00F0)>>4)] & uint8_t((screen_ptr)->height-1);
      uint8_t n = (opcode & 0x000F);
      // std::cout << "X y n :" << std::endl;
      // std::cout << int(x) << std::endl;
      // std::cout << int(y) << std::endl;
      // std::cout << int(n) << std::endl;

      (v)[0xF] = 0;
      for (int yline = 0; yline < n; yline++){
        uint8_t row = memory[address + yline];
        uint8_t rev_row = (row * 0x0202020202ULL & 0x010884422010ULL) % 1023; //Bit hack

        //std::cout << "BITS" << std::endl;
        //std::cout << int(row) << std::endl;
        //std::cout << int(rev_row) << std::endl;

        for(int xline = 0; xline < 8; xline++)
        {
          if(x+xline > ((screen_ptr)->width - 1)){
            break;
          }
          if(((rev_row>>xline)&(0x0001)) == (0x0001)){
            bool test = ((screen_ptr)->flip_pixel)( *(screen_ptr)->pixels, (y+yline)*((screen_ptr)->width)+(x)+xline);
            if(test == true){
              (v)[0xF] = 1;
              //std::cout << "FLIPPED" << std::endl;
            }
          }

        }
      }
      pc = pc+2;
      break;
    }

    case 0xE000:{
      //std::cout<<"WAITING"<<std::endl;
      switch(opcode & 0x00FF){
        case 0x009E:{
          if(screen_ptr->keys[v[((opcode & 0x0F00)>>8)]] == 1){
            pc = pc+2;
          }
          break;
        }
        case 0x00A1:{
          if(screen_ptr->keys[v[((opcode & 0x0F00)>>8)]] == 0){
            pc = pc+2;
          }
          break;
        }
      }
      pc = pc+2;
      break;
    }

    case 0xF000:{
      switch(opcode & 0x00FF){
        case 0x0007:{
          v[((opcode & 0x0F00)>>8)] = delay;
          break;
        }

        case 0x000A:{
          break;
        }

        case 0x0015:{
          delay = v[((opcode & 0x0F00)>>8)] ;
          break;
        }

        case 0x0018:{
          sound = v[((opcode & 0x0F00)>>8)] ;
          break;
        }

        case 0x001E:{
          address += v[((opcode & 0x0F00)>>8)] ;
          break;
        }

        case 0x0029:{
          address = (fontoffset+5*v[((opcode & 0x0F00)>>8)]);
          break;
        }

        case 0x0033:{
          memory[address]   = (v[((opcode & 0x0F00)>>8)] % 1000) / 100;
          memory[address+1] = (v[((opcode & 0x0F00)>>8)] % 100) / 10;
          memory[address+2] = (v[((opcode & 0x0F00)>>8)] % 10);
          break;
        }

        case 0x0055:{
          for(uint8_t i = 0; i<=((opcode & 0x0F00)>>8); i++){
            memory[address+i] = v[i];
          }
          break;
        }

        case 0x0065:{
          for(uint8_t i = 0; i<=((opcode & 0x0F00)>>8); i++){
            v[i] = memory[address+i];
          }
          break;
        }

        break;
      }
      pc=pc+2;
      break;
    }
  }
}
