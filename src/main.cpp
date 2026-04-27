#include <iostream>
#include <filesystem>

#include "Cartridges/CartridgeFactory.h"
#include "RomToPNG.h"

int main(int argc, char * argv[]) {
    if (argc <= 1) {
        std::cout << "No arguments provided" << std::endl;
    }

    const std::filesystem::path path(argv[1]);

    auto cartridge = CartridgeFactory::CreateCartridge(path);
    if (auto err = cartridge->LoadRom(path); !err) {
        std::cerr << "error loading rom: " << err.error() << std::endl;
        return 1;
    }

    if (auto err = cartridge->ParseHeader(); !err) {
        std::cerr << "error validating rom: " << err.error() << std::endl;
        return 2;
    }

    std::cout << "Rom Loaded            : " << path.string() << std::endl;
    std::cout << "Rom Size              : " << cartridge->GetRomSize() << std::endl;
    std::cout << "Rom header Location   : " << cartridge->GetHeaderLocation() << std::endl;

    auto pngFileName = path.stem().string() + ".png";
    auto pngFilePath = path.parent_path() / pngFileName;

    FILE* file = fopen(pngFilePath.string().c_str(), "wb");
    if (!file) {
        std::cerr << "error creating png file: " << pngFilePath.string() << std::endl;
        return 3;
    }

    RomToPNG pngConverter{};
    if (auto err = pngConverter.Convert(*cartridge, file); !err) {
        std::cerr << "error converting rom to png: " << err.error() << std::endl;
        return 4;
    }

    std::cout << "Rom converted to png  : " << pngFilePath.string() << std::endl;

    fclose(file);

    return 0;
}
