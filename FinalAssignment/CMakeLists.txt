cmake_minimum_required(VERSION 3.10)
project(FinalAssignment)

set(CMAKE_CXX_STANDARD 14)

# Add all source files
set(SOURCE_FILES
    Ammo.cpp
    Ammo.h
    Asteroids.cpp
    Asteroids.h
    CreateSpace.cpp
    player.cpp
    player.h
    Ship1.cpp
    Ship1.h
    Space.cpp
    Space.h
    SpawnAmmo.cpp
    SpawnShip1.cpp
    TextureHolder.cpp
    TextureHolder.h
)

# Add all resources
set(RESOURCE_FILES
    fonts/spaceFont.otf
    fonts/zombiecontrol.ttf
    graphics/ammo_icon.png
    graphics/ammo_pickup.png
    graphics/asteroid.png
    graphics/asteroidfire.jpg
    graphics/background\ 1280-720.png
    graphics/background.png
    graphics/background_sheet.png
    graphics/bloater.png
    graphics/blood.png
    graphics/chaser.png
    graphics/crawler.png
    graphics/crosshair.png
    graphics/crosshair2.jpg
    graphics/health_pickup.png
    graphics/player.png
    graphics/sample.png
    graphics/Ship1.png
    graphics/space.png
)

# Add the executable
add_executable(FinalAssignment ${SOURCE_FILES})

# Find SFML
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.6.0 COMPONENTS graphics audio REQUIRED)

# Link SFML
target_link_libraries(FinalAssignment PRIVATE sfml-graphics sfml-audio)

# Copy resources to the build directory
foreach(resource ${RESOURCE_FILES})
    configure_file(${CMAKE_SOURCE_DIR}/${resource} ${CMAKE_BINARY_DIR}/${resource} COPYONLY)
endforeach()