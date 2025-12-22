cmake_minimum_required(VERSION 3.20)

project(Library-Book-Borrowing-Management-system)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")
file(GLOB source_files "cpp/src/*.cpp")

add_executable(main ${source_files}
        cpp/include/History.h
        cpp/include/Library.h
        cpp/include/Menu.h
        cpp/include/Console.h
        cpp/src/Library.cpp)
