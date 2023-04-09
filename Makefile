SRC = $(wildcard *.cpp)
HEADERS = $(wildcard *.h)
OBJ = $(SRC:%.cpp=build/%.o)
AOBJ = $(SRC:%.cpp=build/%.asan.o)
TOBJ = $(SRC:%.cpp=build/%.tsan.o)
SDLLIBS = $(shell pkg-config --cflags --libs SDL2_image SDL2_ttf SDL2_mixer sdl2)

CMD = $(CXX) -std=c++20 -o $@ $^ $(SDLLIBS) -g

The_Great_War: $(OBJ)
	$(CMD)

asan: $(AOBJ)
	$(CMD) -fsanitize=address,undefined,leak

tsan: $(TOBJ)
	$(CMD) -fsanitize=thread

valgrind: The_Great_War
	valgrind --show-error-list=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./$<

build/%.o : %.cpp Makefile $(HEADERS) | build
	$(CXX) -c -std=c++20 $(SDLLIBS) -g $< -o $@

build/%.asan.o : %.cpp Makefile $(HEADERS) | build
	$(CXX) -c -std=c++20 $(SDLLIBS) -g -fsanitize=address,undefined,leak $< -o $@

build/%.tsan.o : %.cpp Makefile $(HEADERS) | build
	$(CXX) -c -std=c++20 $(SDLLIBS) -g -fsanitize=thread $< -o $@

build:
	mkdir -p build

clean:
	rm -rf build The_Great_War asan tsan
