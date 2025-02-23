cmake_minimum_required(VERSION 3.16)
project(qsnake)

# Set the output directory for built objects
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# This makes sure that the dynamic library goes into the build directory automatically
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL)

# Create the game executable target
add_executable(${PROJECT_NAME})

# List all the sources to compile
target_sources(${PROJECT_NAME}
PRIVATE
    src/main.c
    src/input.c
    src/update.c
)

# Link to the actual SDL3 library
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)