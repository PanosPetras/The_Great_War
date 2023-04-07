SRC = $(wildcard *.cpp)
SDLLIBS = $(shell pkg-config --cflags --libs SDL2_image SDL2_ttf SDL2_mixer sdl2)

CMD = $(CXX) -std=c++20 -o $@ $(SRC) $(SDLLIBS)

The_Great_War: $(SRC)
	$(CMD)

asan: $(SRC)
	$(CMD) -g -fsanitize=address,undefined,leak

tsan: $(SRC)
	$(CMD) -g -fsanitize=thread
