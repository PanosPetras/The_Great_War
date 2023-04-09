SRC = $(wildcard *.cpp)
HEADERS = $(wildcard *.h)
OBJ = $(SRC:%.cpp=build/%.o)
AOBJ = $(SRC:%.cpp=build/%.asan.o)
TOBJ = $(SRC:%.cpp=build/%.tsan.o)
SDLLIBS = $(shell pkg-config --cflags --libs SDL2_image SDL2_ttf SDL2_mixer sdl2)

CMD = $(CXX) -c -std=c++20 $(SDLLIBS) -g $< -o $@ -Wall -Wextra -pedantic-errors
LCMD = $(CXX) -std=c++20 -o $@ $^ $(SDLLIBS) -g

The_Great_War: $(OBJ)
	$(LCMD)

asan: $(AOBJ)
	$(LCMD) -fsanitize=address,undefined,leak

tsan: $(TOBJ)
	$(LCMD) -fsanitize=thread

valgrind: The_Great_War
	valgrind --show-error-list=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./$<

build/%.o : %.cpp Makefile $(HEADERS) | build
	$(CMD)

build/%.asan.o : %.cpp Makefile $(HEADERS) | build
	$(CMD) -fsanitize=address,undefined,leak

build/%.tsan.o : %.cpp Makefile $(HEADERS) | build
	$(CMD) -fsanitize=thread

build:
	mkdir -p build

clean:
	rm -rf build The_Great_War asan tsan
