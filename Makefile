#$(info $(OS))
ifeq ($(OS), Windows_NT)
LIB_LINKS := -lmingw32 -lSDL2main -lSDL2
else
LIB_LINKS := -lSDL2main -lSDL2
endif

CXX := g++
CXXFLAGS := -Wall -g
CPPFLAGS := -I include -L lib

#LIB_LINKS := -lmingw32 -lSDL2main -lSDL2 -static-libgcc -static-libstdc++
#LIB_LINKS := `sdl2-config --libs --cflags`

NAME := Chip8_a2
SRC := src
OBJ := obj
BIN := bin
LIB := lib

EXT := cpp
EXTH := h

SRC_FILES := $(wildcard $(SRC)/**.$(EXT))
#SRC_FILES += $(wildcard $(SRC)/*.$(EXTH))
OBJ_FILES := $(patsubst %.$(EXT), %.o, $(SRC_FILES))
OBJ_FILES := $(patsubst $(SRC)/%, $(OBJ)/%, $(OBJ_FILES))
#EXECUTABLE_FILES = $(NAME:%=$(BIN)/%)
EXECUTABLE_FILES = $(BIN)/$(NAME)

build: $(EXECUTABLE_FILES)

clean:
	rm -r -f $(EXECUTABLE_FILES)
	rm -r -f $(OBJ_FILES)

.PHONY: build clean

$(OBJ_FILES): $(OBJ)/%.o : $(SRC)/%.$(EXT)
	$(CXX) $(CPPFLAGS) $(LIB_LINKS) $(CXXFLAGS) -c $< -o $@

$(EXECUTABLE_FILES): $(OBJ_FILES)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $(EXECUTABLE_FILES) $(OBJ_FILES) $(LIB_LINKS)
