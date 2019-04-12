CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)

project(h6nsdk C)

# Fix dumb bug in cmake...
if(CMAKE_C_STANDARD_DEFAULT EQUAL 98)
    set(CMAKE_C_STANDARD_DEFAULT 99 CACHE STRING "" FORCE)
endif()

# C90 and below is too old for us to feasibly support
set(CMAKE_C_STANDARD 99)

add_library(h6n STATIC
        src/createinterface.c)
    
target_include_directories(h6n PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/)

