SRC = $(wildcard *.cpp)
HEADERS = $(wildcard *.h)
OBJ = $(SRC:%.cpp=build/%.o)
AOBJ = $(SRC:%.cpp=build/%.asan.o)
TOBJ = $(SRC:%.cpp=build/%.tsan.o)
SDLINCS = $(shell pkg-config --cflags SDL2_image SDL2_ttf SDL2_mixer sdl2)
SDLLIBS = $(shell pkg-config --libs SDL2_image SDL2_ttf SDL2_mixer sdl2)

CCMD = $(CXX) -std=c++20 -g $(SDLINCS) -c -o $@ $< -Wall -Wextra -pedantic-errors
LCMD = $(CXX) -o $@ -std=c++20 -g $^ $(SDLLIBS)

The_Great_War: $(OBJ)
	$(LCMD)

asan: $(AOBJ)
	$(LCMD) -fsanitize=address,undefined,leak

tsan: $(TOBJ)
	$(LCMD) -fsanitize=thread

valgrind: The_Great_War
	valgrind --show-error-list=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./$<

build/%.o : %.cpp Makefile $(HEADERS) | build
	$(CCMD)

build/%.asan.o : %.cpp Makefile $(HEADERS) | build
	$(CCMD) -fsanitize=address,undefined,leak

build/%.tsan.o : %.cpp Makefile $(HEADERS) | build
	$(CCMD) -fsanitize=thread

build:
	mkdir -p build

clean:
	rm -rf build The_Great_War asan tsan
