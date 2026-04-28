#include <iostream>
#include <filesystem>
#include <fstream>

#include "Config.h"
#include "Converters/RomToPNG.h"
#include "Systems/System.h"
#include "Systems/Nes/Cartridge.h"

std::filesystem::path g_path;
std::unique_ptr<System> g_system;

void displayRomInfo()
{
    std::cout << "Rom Loaded            : " << g_path.string() << std::endl;
    std::cout << "Rom Size              : " << g_system->GetCartridge().GetRomSize() << std::endl;
    std::cout << "Rom header Location   : " << g_system->GetCartridge().GetHeaderLocation() << std::endl;
}

std::expected<bool, std::string> convertPng()
{
    const auto pngFileName = g_path.stem().string() + ".png";
    const auto pngFilePath = g_path.parent_path() / pngFileName;

    FILE* file = fopen(pngFilePath.string().c_str(), "wb");
    if (!file){
        return std::unexpected("failed to open png file: " + pngFilePath.string());
    }

    RomToPNG pngConverter{};
    if (auto err = pngConverter.Convert(g_system->GetCartridge(), file); !err){
        return std::unexpected("error converting rom to png: " + err.error());
    }

    std::cout << "Rom converted to png  : " << pngFilePath.string() << std::endl;

    fclose(file);

    return true;
}

std::expected<bool, std::string> extractChrRom()
{
    if (g_system->GetCartridge().GetType() != ICartridge::Type::Nes){
        return std::unexpected("cartridge doesn't support CHR-ROM");
    }

    auto& nesCartridge = dynamic_cast<const nes::Cartridge&>(g_system->GetCartridge());

    const auto chrFileName = g_path.stem().string() + ".chr";
    const auto chrFilePath = g_path.parent_path() / chrFileName;

    std::ofstream file(chrFilePath, std::ios::binary);
    if (!file.is_open()) {
        return std::unexpected("failed to open chr-rom file: " + chrFilePath.string());
    }

    auto chrData = nesCartridge.GetChrRomData();
    file.write(reinterpret_cast<const char*>(chrData.data()), chrData.size());

    if (!file){
        return std::unexpected("failed to write chr-rom file: " + chrFilePath.string());
    }

    file.close();

    std::cout << "extract CHR-ROM to    : " << chrFilePath.string() << std::endl;

    return true;
}

int main(int argc, char* argv[])
{
    if (argc <= 1){
        std::cerr << Config::GetHelpText() << std::endl;
        return 1;
    }

    Config config(argc, argv);
    if (auto err = config.Parse(); !err){
        std::cerr << "error parsing params: " << err.error() << std::endl;
        std::cerr << Config::GetHelpText() << std::endl;
        return 2;
    }

    if (auto err = config.Validate(); !err){
        std::cerr << "error validating params: " << err.error() << std::endl;
        std::cerr << Config::GetHelpText() << std::endl;
        return 3;
    }

    g_path = config.GetRomPath();
    auto systemResult = System::CreateSystemFromRom(g_path);
    if (!systemResult){
        std::cerr << "error creating system: " << systemResult.error() << std::endl;
        std::cerr << Config::GetHelpText() << std::endl;
        return 4;
    }

    g_system = std::move(systemResult.value());

    displayRomInfo();

    if (config.ShouldConvertPng()){
        if (auto err = convertPng(); !err){
            std::cerr << "error converting rom to png: " << err.error() << std::endl;
            return 5;
        }
    }

    if (config.ShouldExtractChrRom()){
        if (auto err = extractChrRom(); !err){
            std::cerr << "error extracting chr rom: " << err.error() << std::endl;
            return 6;
        }
    }

    return 0;
}
