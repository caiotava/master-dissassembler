
#ifndef MASTERSYSTEM_DISSASSEMBLER_CARTRIDGEFACTORY_H
#define MASTERSYSTEM_DISSASSEMBLER_CARTRIDGEFACTORY_H

#include "Cartridges/ICartridge.h"

class CartridgeFactory
{
public:
    static std::unique_ptr<ICartridge> CreateCartridge(const std::filesystem::path &path);

    static std::expected<std::vector<uint8_t>, std::string> LoadRom(const std::filesystem::path &path);
};


#endif //MASTERSYSTEM_DISSASSEMBLER_CARTRIDGEFACTORY_H
