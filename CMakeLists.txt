cmake_minimum_required(VERSION 3.29)
project(battlship)

set(CMAKE_CXX_STANDARD 14)

add_executable(battlship
    main.cpp
        Field.cpp
        Field.h
        ParseFile.cpp
        ParseFile.h
        Field.cpp
        Commands.cpp
        Commands.h
        Commands.cpp
)
