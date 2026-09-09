cmake_minimum_required(VERSION 3.28)

project(The_Great_War VERSION 1.0.0 LANGUAGES CXX)

# The game finds its assets relative to the working directory, so it has to be
# launched from the source tree: `./build/The_Great_War` from here, or `cmake
# --build build --target run`, which sets the directory for you.

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release RelWithDebInfo MinSizeRel)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(TGW_SANITIZE "none" CACHE STRING "Sanitizer to build with: none, address or thread")
set_property(CACHE TGW_SANITIZE PROPERTY STRINGS none address thread)

find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)
find_package(SDL3_ttf REQUIRED CONFIG)

add_executable(The_Great_War
    src/main.cpp
    src/core/Color.cpp
    src/core/Coordinate.cpp
    src/core/MainWindow.cpp
    src/core/Resolutions.cpp
    src/core/SDL_ColorDetection.cpp
    src/core/SDL_ctx.cpp

    src/game/AI.cpp
    src/game/Country.cpp
    src/game/Diplomacy.cpp
    src/game/Factory.cpp
    src/game/Market.cpp
    src/game/PlayerController.cpp
    src/game/State.cpp

    src/screens/CountrySelection.cpp
    src/screens/CreditScreen.cpp
    src/screens/DiplomacyScreen.cpp
    src/screens/EconomyScreen.cpp
    src/screens/GameScreen.cpp
    src/screens/IndustryScreen.cpp
    src/screens/MainMenu.cpp
    src/screens/MenuSettingsScreen.cpp
    src/screens/OpenFactoryScreen.cpp
    src/screens/PauseMenu.cpp
    src/screens/Screen.cpp
    src/screens/StatePreview.cpp
    src/screens/TradeScreen.cpp

    src/ui/Button.cpp
    src/ui/Checkbox.cpp
    src/ui/Drawable.cpp
    src/ui/Image.cpp
    src/ui/Label.cpp
    src/ui/MessageBox.cpp
    src/ui/Slider.cpp
    src/ui/TextEntry.cpp
    src/ui/ToggleButton.cpp
    src/ui/UI.cpp

    include/core/Color.h
    include/core/Coordinate.h
    include/core/FontSize.h
    include/core/MainWindow.h
    include/core/Resolutions.h
    include/core/SDL_ColorDetection.h
    include/core/SDL_ctx.h

    include/game/AI.h
    include/game/Country.h
    include/game/Diplomacy.h
    include/game/Factory.h
    include/game/Market.h
    include/game/PerGood.h
    include/game/PlayerController.h
    include/game/State.h
    include/game/Stockpile.h

    include/screens/CreditScreen.h
    include/screens/Screen.h
    include/screens/ScreenList.h

    include/ui/Button.h
    include/ui/Checkbox.h
    include/ui/Drawable.h
    include/ui/Image.h
    include/ui/Label.h
    include/ui/MessageBox.h
    include/ui/Slider.h
    include/ui/TextEntry.h
    include/ui/ToggleButton.h
    include/ui/UI.h

    include/util/Eater.h
)

# Mirror the two trees in IDEs that show a source tree.
get_target_property(TGW_SOURCES The_Great_War SOURCES)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "Files" FILES ${TGW_SOURCES})

# Headers are included by their group, e.g. #include "ui/Button.h".
target_include_directories(The_Great_War PRIVATE include)

target_link_libraries(The_Great_War PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
)

# GCC and Clang share a warning vocabulary. Any other compiler keeps its own
# defaults rather than being handed flags it has never heard of.
target_compile_options(The_Great_War PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Woverloaded-virtual -pedantic-errors>
)

if(NOT TGW_SANITIZE STREQUAL "none")
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "^(GNU|Clang|AppleClang)$")
        message(FATAL_ERROR "TGW_SANITIZE needs GCC or Clang, not ${CMAKE_CXX_COMPILER_ID}")
    endif()
    if(TGW_SANITIZE STREQUAL "address")
        target_compile_options(The_Great_War PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
        target_link_options(The_Great_War PRIVATE -fsanitize=address,undefined)
    elseif(TGW_SANITIZE STREQUAL "thread")
        target_compile_options(The_Great_War PRIVATE -fsanitize=thread -fno-omit-frame-pointer)
        target_link_options(The_Great_War PRIVATE -fsanitize=thread)
    else()
        message(FATAL_ERROR "TGW_SANITIZE must be none, address or thread, got '${TGW_SANITIZE}'")
    endif()
endif()

# On Windows the SDL DLLs have to sit next to the executable. Elsewhere the
# list is empty, which would leave `cmake -E copy_if_different` with nothing to
# copy, so only ask for it where it means something.
if(WIN32)
    add_custom_command(TARGET The_Great_War POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_RUNTIME_DLLS:The_Great_War> $<TARGET_FILE_DIR:The_Great_War>
        COMMAND_EXPAND_LISTS
    )
endif()

add_custom_target(run
    COMMAND $<TARGET_FILE:The_Great_War>
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    DEPENDS The_Great_War
    USES_TERMINAL
)

find_program(TGW_VALGRIND valgrind)
if(TGW_VALGRIND)
    add_custom_target(valgrind
        COMMAND ${TGW_VALGRIND} --show-error-list=yes --track-origins=yes
                --leak-check=full --show-leak-kinds=all $<TARGET_FILE:The_Great_War>
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        DEPENDS The_Great_War
        USES_TERMINAL
    )
endif()
