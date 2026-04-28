#include <iostream>
#include <filesystem>
#include "Converters/RomToPNG.h"
#include "Systems/System.h"

int main(int argc, char * argv[]) {
    if (argc <= 1) {
        std::cout << "No arguments provided" << std::endl;
    }

    const std::filesystem::path path(argv[1]);

    auto systemResult = System::CreateSystemFromRom(path);
    if (!systemResult){
        std::cerr << "error creating system: " << systemResult.error() << std::endl;
        return 1;
    }

    const auto system = std::move(systemResult.value());

    std::cout << "Rom Loaded            : " << path.string() << std::endl;
    std::cout << "Rom Size              : " << system->GetCartridge().GetRomSize() << std::endl;
    std::cout << "Rom header Location   : " << system->GetCartridge().GetHeaderLocation() << std::endl;

    auto pngFileName = path.stem().string() + ".png";
    auto pngFilePath = path.parent_path() / pngFileName;

    FILE* file = fopen(pngFilePath.string().c_str(), "wb");
    if (!file) {
        std::cerr << "error creating png file: " << pngFilePath.string() << std::endl;
        return 3;
    }

    RomToPNG pngConverter{};
    if (auto err = pngConverter.Convert(system->GetCartridge(), file); !err) {
        std::cerr << "error converting rom to png: " << err.error() << std::endl;
        return 4;
    }

    std::cout << "Rom converted to png  : " << pngFilePath.string() << std::endl;

    fclose(file);

    return 0;
}
