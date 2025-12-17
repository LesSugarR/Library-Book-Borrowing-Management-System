cmake_minimum_required(VERSION 3.20)

project(Library-Book-Borrowing-Management-system)
set(CMAKE_CXX_STANDARD 17)
file(GLOB source_files "src/*.cpp")

add_executable(main ${source_files}
        include/History.h
        include/Library.h
        include/Menu.h
        include/Console.h)