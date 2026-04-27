cmake_minimum_required(VERSION 4.0)
project(mastersystem_dissassembler)

set(CMAKE_CXX_STANDARD 26)

include_directories(src)

find_package(PNG REQUIRED)

add_executable(mastersystem_dissassembler
        src/main.cpp
        src/RomToPNG.cpp
        src/Cartridges/Rom.cpp
        src/Cartridges/CartridgeFactory.cpp
        src/Cartridges/MasterSystem/Cartridge.cpp
        src/Cartridges/Nes/Cartridge.cpp
        src/Converters/Png/MasterSystem/TileDecoder.cpp
        src/Converters/Png/Nes/TileDecoder.cpp
)

target_link_libraries(mastersystem_dissassembler PRIVATE PNG::PNG)