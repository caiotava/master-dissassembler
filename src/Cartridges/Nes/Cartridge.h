#ifndef MASTERSYSTEM_DISSASSEMBLER_NES_CARTRIDGE_H
#define MASTERSYSTEM_DISSASSEMBLER_NES_CARTRIDGE_H

#include <span>
#include <Cartridges/ICartridge.h>
#include <Cartridges/Rom.h>

namespace nes
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
            return ICartridge::Type::Nes;
        };

        uint32_t GetPrgRomSize() const;
        uint32_t GetChrRomSize() const;

        std::span<const uint8_t> GetPrgRomData() const;
        std::span<const uint8_t> GetChrRomData() const;
    private:
        std::unique_ptr<Rom> m_rom;
        uint32_t m_prgRomSize{};
        uint32_t m_chrRomSize{};
    };
} // nes

#endif //MASTERSYSTEM_DISSASSEMBLER_NES_CARTRIDGE_H
