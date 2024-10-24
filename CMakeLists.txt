cmake_minimum_required(VERSION 3.5)
project ("Envelope_Generator")

# set C++ standard
set(CMAKE_CXX_STANDARD 17)

# sfml directories
set(SFML_DIR "C:/Users/Mason/Documents/Visual Studio Code/Libraries/SFML-2.6.1-windows-gcc-13.1.0-mingw-64-bit/SFML-2.6.1/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

# add the executable
add_executable(envelope src/main.cpp
                        src/app_manager.cpp
                        src/envelope_generator.cpp
                        src/envelope_visualizer.cpp
                        src/slider.cpp
                        src/knob.cpp
                        src/button.cpp
                        src/theme.cpp)

# link sfml libraries
target_link_libraries(envelope sfml-graphics)