cmake_minimum_required(VERSION 4.0)
project(mastersystem_dissassembler)

set(CMAKE_CXX_STANDARD 26)

include_directories(src)

find_package(PNG REQUIRED)

add_executable(mastersystem_dissassembler
        src/main.cpp
        src/Config.cpp
        src/Converters/RomToPNG.cpp
        src/Cartridges/Rom.cpp
        src/Systems/System.cpp
        src/Systems/MasterSystem/Cartridge.cpp
        src/Systems/Nes/Cartridge.cpp
        src/Systems/MasterSystem/TileDecoder.cpp
        src/Systems/Nes/TileDecoder.cpp
)

target_link_libraries(mastersystem_dissassembler PRIVATE PNG::PNG)