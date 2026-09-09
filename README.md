# The Great War
### A WW1 themed strategy game in progress. Built using C++ and SDL 3.

Starting date: January 1st 1910

The world is in turmoil, tensions between the great powers of Europe are rising.
The Balkan Nation-states are in a state of constant war.
This might spark a greater conflict that will change the World forever.

> One day the great European War will come out of some damned foolish thing in the Balkans.
>
> -- _Otto von Bismarck_

In this time of crisis you take control over one of the following countries:
- The German Empire
- The British Empire
- The Austro-Hungarian Empire
- The French Republic
- The Russian Empire
- The Ottoman Empire
- The United States of America
- The Kingdom of Italy

The game is meant to be played in various ways.
A real life economy simulation is currently in progress.
The player will be producing different kinds of goods
in order to sustain his Country's population and it's military.
Excess goods will be sold on the market with prices that will change
based on supply and demand.

The trade tab will allow the player to keep track of the prices of all goods to understand the state of the market and invest in the most profitable goods.

The player will be able to manage his relations with other countries through the diplomacy tab, make alliances declare wars etc.

When all diplomacy fails, the player will have to lead his army to victory, by attacking at the most suitable locations, supplying his army, training officers and many more.

Last but not least, the player will have the ability to create universities. If the universities are adequately funded they will start discovering new technologies over time that will increase the country's indusrial efficiency, improve it's military equipment, improve the healthcare system and many more more.

The game is stable at it's current state and there are 
no bugs that will cause a crash as far as I'm aware of.

---

## Building

The build is CMake-only (3.28 or newer) and compiles with GCC. You also need
the SDL 3 development packages listed below.

```sh
cmake --preset default
cmake --build --preset default
```

`cmake --list-presets` shows the rest.

The presets do not pin a generator, so CMake takes it from the `CMAKE_GENERATOR`
environment variable and falls back to Makefiles when that is unset. Ninja is
worth having; to make it the default for every CMake project:

```sh
export CMAKE_GENERATOR=Ninja
```

The game looks for `Backgrounds/`, `map/`, `Fonts/` and the rest relative to the
working directory, so run it from the root of the repository:

```sh
./build/The_Great_War
```

`cmake --build build --target run` does the same thing and sets the working
directory for you.

### Presets

Each one configures into its own build tree, so they never fight over object
files.

| Preset | Tree | What it is |
| --- | --- | --- |
| `default` | `build/` | Release. |
| `debug` | `build-debug/` | Debug. |
| `asan` | `build-asan/` | Debug with the address and undefined sanitizers. |
| `tsan` | `build-tsan/` | Debug with the thread sanitizer. |

```sh
cmake --preset asan
cmake --build --preset asan
```

The underlying knob is `TGW_SANITIZE` (`none`, `address` or `thread`) if you
would rather configure a tree by hand.

There is also a `valgrind` target, present when `valgrind` is found on `PATH`.

### Layout

Headers live in `include/`, translation units in `src/`, and the two mirror each
other. Headers are included by their group, so `#include "ui/Button.h"` rather
than a bare filename.

| Group | What is in it |
| --- | --- |
| `core` | The window, the SDL wrappers, and the small value types they need. |
| `ui` | Widgets: buttons, labels, images, sliders, text entry. |
| `screens` | One file per screen, plus the base class they share. |
| `game` | The simulation: countries, states, factories, goods, diplomacy. |
| `util` | Odds and ends with no dependency on the rest. |

C++ version: C++26

This project makes use the following libraries:
- SDL 3.4.16, 3.4 or newer required(Link: https://github.com/libsdl-org/SDL)
- SDL Image 3.4.4(Link: https://github.com/libsdl-org/SDL_image)
- SDL ttf 3.2.2(Link: https://github.com/libsdl-org/SDL_ttf)

Sound effects are played through SDL's own audio streams rather than through
SDL_mixer, which has no SDL 3 release packaged yet. SDL decodes WAV and nothing
else on its own, so sound effects have to be WAV files.

The project is a work in progress and currently on a non-playable state.
Many features are missing and/or are not working as intended.

Note that some parts of the code are not documented with comments yet.

Minimum System requirements:

- CPU: Intel Core i5 3450 or AMD Ryzen 3 1200
- Ram: 4 Gb
- GPU: Nvidia GeForce GTX 750 TI or AMD Radeon RX 550
