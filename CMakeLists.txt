cmake_minimum_required(VERSION 3.13)
project(PCV)


set(CMAKE_CXX_STANDARD 14)
add_library(myLib SHARED main.cpp )
set(CMAKE_EXE_LINKER_FLAGS " -static")
target_link_libraries(myLib -static-libgcc -static-libstdc++)


add_executable(PCV main.cpp Arquivos.h Arquivos.cpp Construcao.h Construcao.cpp Descida.h Descida.cpp Menus.h Menus.cpp MS.h MS.cpp Util.h Util.cpp )