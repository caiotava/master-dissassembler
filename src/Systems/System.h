#ifndef MASTERSYSTEM_DISSASSEMBLER_SYSTEM_H
#define MASTERSYSTEM_DISSASSEMBLER_SYSTEM_H
#include <filesystem>

#include "Cartridges/ICartridge.h"

class System
{
public:
    static std::expected<std::unique_ptr<System>, std::string> CreateSystemFromRom(const std::filesystem::path& path);

    const ICartridge& GetCartridge() const;

private:
    std::filesystem::path m_path;
    std::unique_ptr<ICartridge> m_cartridge;

    static std::expected<std::unique_ptr<ICartridge>, std::string> createCartridge(const std::filesystem::path &path);
};


#endif //MASTERSYSTEM_DISSASSEMBLER_SYSTEM_H
