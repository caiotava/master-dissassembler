#ifndef MASTERSYSTEM_DISSASSEMBLER_CARTRIDGE_H
#define MASTERSYSTEM_DISSASSEMBLER_CARTRIDGE_H

#include <expected>
#include <filesystem>

#include "Zone.h"

#include <Cartridges/ICartridge.h>
#include "Cartridges/Rom.h"

namespace mastersystem
{
    class Cartridge : public ICartridge
    {
    public:
        Cartridge();

        std::expected<bool, std::string> LoadRom(const std::filesystem::path& path) override;
        std::expected<bool, std::string> ParseHeader() override;

        const std::vector<uint8_t>& GetRomData() const override;
        uint32_t GetRomSize() const override;
        uint32_t GetHeaderLocation() const override;
        ICartridge::Type GetType() const override
        {
            return ICartridge::Type::MasterSystem;
        };

    private:
        std::unique_ptr<Rom> m_rom;
        uint16_t m_headerLocation{};
        Zone m_romZone;

        std::expected<bool, std::string> identifyHeaderLocation();
        std::expected<bool, std::string> validateHeader(uint16_t location) const;
        std::expected<bool, std::string> identifyZone();
    };
}


#endif //MASTERSYSTEM_DISSASSEMBLER_CARTRIDGE_H
