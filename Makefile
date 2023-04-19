SRC := $(wildcard *.cpp)
HEADERS := $(wildcard *.h)
SDLINCS := $(shell pkg-config --cflags SDL2_image SDL2_ttf SDL2_mixer sdl2 | sed 's/-I/-isystem/g')
SDLLIBS := $(shell pkg-config --libs SDL2_image SDL2_ttf SDL2_mixer sdl2)

X64DLLS := $(wildcard SDL2/*/lib/x64/*.dll)
X64DLLT := $(wildcard SDL2/*/lib/x64/*/*.dll)
X64DLLSO = $(notdir $(X64DLLS))
X64DLLTO = $(notdir $(X64DLLT))
X64ALL = $(X64DLLSO) $(X64DLLTO)

OBJ = $(SRC:%.cpp=build/%.o)
AOBJ = $(SRC:%.cpp=build/%.asan.o)
TOBJ = $(SRC:%.cpp=build/%.tsan.o)
CCMD = $(CXX) -std=c++20 -g $(SDLINCS) -c -o $@ $< -Wall -Wextra -Woverloaded-virtual -pedantic-errors $(OPTS)
LCMD = $(CXX) -o $@ -std=c++20 -g $^ $(SDLLIBS) $(OPTS)

CLANGOPTS = -Weverything -Wweak-vtables -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Wno-documentation -Wno-poison-system-directories -Wno-weak-vtables

The_Great_War: $(OBJ)
	$(LCMD)

clang:
	CXX=clang++ OPTS='$(CLANGOPTS)' make -j

asan: $(AOBJ)
	$(LCMD) -fsanitize=address,undefined,leak

tsan: $(TOBJ)
	$(LCMD) -fsanitize=thread

valgrind: The_Great_War
	valgrind --show-error-list=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./$<

build/%.o : %.cpp Makefile $(HEADERS) | build
	$(CCMD) -O3

build/%.asan.o : %.cpp Makefile $(HEADERS) | build
	$(CCMD) -Og -fsanitize=address,undefined,leak

build/%.tsan.o : %.cpp Makefile $(HEADERS) | build
	$(CCMD) -Og -fsanitize=thread

build:
	mkdir -p build

links: $(X64ALL)
	ln -s ./x64/Release/The_Great_War.exe .

%.dll : SDL2/*/lib/x64/%.dll
	ln -s $< $@

%.dll : SDL2/*/lib/x64/*/%.dll
	ln -s $< $@

clean:
	rm -rf build The_Great_War asan tsan $(X64ALL) The_Great_War.exe
