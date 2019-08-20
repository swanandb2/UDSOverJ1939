cmake_minimum_required(VERSION 3.2.2)
project(UDSOverJ1939)

include_directories(src src/include)

file(GLOB SOURCES_APP "src/*.cpp")
if(CMAKE_COMPILER_IS_GNUCXX)	
       set(CMAKE_CXX_FLAGS "-fpermissive -std=c++11")
endif()

add_executable(UDSOverJ1939App ${SOURCES_APP})

