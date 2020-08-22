cmake_minimum_required(VERSION 3.14)
set(PROJECT_NAME "GREMIN")
project(${PROJECT_NAME})

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "${CMAKE_CURRENT_LIST_DIR}/libs/SFML-2.5.1/lib/cmake/SFML")

file(COPY libs/SFML-2.5.1/bin/openal32.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors)
link_libraries(stdc++fs) #link the filesystem


include_directories(libs)


set(SOURCE_FILES
        src/characters.cpp
        src/event.cpp
        src/map.cpp
        src/camera.cpp)

include_directories(include)

add_executable(GREMIN ${SOURCE_FILES} main.cpp)


find_package(SFML 2.5.1 COMPONENTS system window graphics network audio REQUIRED)
target_link_libraries(${PROJECT_NAME} sfml-audio sfml-graphics sfml-window sfml-system)


set(SOURCES_FILES_TESTS

        )

add_executable(GREMIN__test ${SOURCE_FILES} ${SOURCES_FILES_TESTS} test/main_gtest.cpp)

# TODO: Poniższe ścieżki prawdopodobnie musisz zmienić...
target_include_directories(GREMIN__test PUBLIC
        libs/googletest-master/googlemock/include
        libs/googletest-master/googletest/include
        )

# TODO: Poniższą ścieżkę prawdopodobnie musisz zmienić...
add_subdirectory(libs/googletest-master libs/googletest-master)

target_link_libraries(GREMIN__test gmock)
