# The Great War
A ww1 themed strategy game on progress. Built using C++ and SDL2.0.

Created with Visual Studio 2019

C++ version: C++/11

Starting date: January 1st 1910

The world is in turmoil, tensions between the great powers of europe are rising.
The balkan Nation-states are in a state of constant war.
This might spark a greater conflict that will change europe forever.

"One day the great European War will come out of some damned foolish thing in the Balkans."

-Otto von Bismarck

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
The player will be producing different kinds of good
in order to sustain his Country's population and it's military.
Excess goods will be sold on the market with prices that will change
based on supply and demand.

The trade tab will allow the player to keep track of the prices of all goods to understand the state of the market and invest in the most profitable goods.

The player will be able to manage his relations with other countries through the diplomacy tab, make alliances declare wars etc.

When all diplomacy fails, the player will have to lead his army to victory, by attacking at the most suitable locations, supplying his army, training officers and many more.

Last but not least, the player will have the ability to create universities. If the universities are adequately funded they will start discovering new technologies over time that will increase the country's indusrial efficiency, improve it's military equipment, improve the healthcare system and many more more.

The game is stable at it's current state and there are 
no bugs that will cause a crash as far as I'm aware of.

This project requires the following libraries:
- SDL 2.0.14(Link: https://www.libsdl.org/download-2.0.php)
- SDL Mixer 2.0.4(Link: https://www.libsdl.org/projects/SDL_mixer/)
- SDL Image 2.0.5(Link: https://www.libsdl.org/projects/SDL_image/)
- SDL ttf 2.0.15(Link: https://www.libsdl.org/projects/SDL_ttf/)

Download the Visual C++ 32/64-bit Development Library

Edit the settings on Project > Project properties as following

Add "SDL2.lib;SDL2main.lib;SDL2_image.lib;SDL2_ttf.lib;SDL2_mixer.lib;" in Configuration Properties > Linker > Input > Additonal Dependencies

Add the path to all the "include" folders of the aforementioned libraries in Configuration Properties > VC++ Directories > Include Directories

Add the path to all the "lib" folders of the aforementioned libraries in Configuration Properties > VC++ Directories > Library Directories

The project is a work in progress and currently on a non-playable state.
Many features are missing and/or are not working as intended.

Note that some parts of the code are not documented with comments yet.

Minimum System requirements:

CPU: Intel Core i5 3450 or AMD ryzen 3 1200

Ram: 4 Gb

GPU: Nvidia GeForce GTX 750 TI or AMD Radeon RX 550
