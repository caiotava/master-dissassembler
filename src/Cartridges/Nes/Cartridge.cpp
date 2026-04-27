
#include "Cartridge.h"

namespace nes
{
    constexpr uint32_t headerPositionPRGSize = 0x04;
    constexpr uint32_t headerPositionCHRSize = 0x05;
    constexpr uint32_t headerSize = 0x0F; // 16
    constexpr uint32_t prgRomMultiplier = 16 * 1024; // 16kb
    constexpr uint32_t chrRomMultiplier = 8 * 1024; // 8kb

    Cartridge::Cartridge()
    {

    }

    std::expected<bool, std::string> Cartridge::LoadRom(const std::filesystem::path& path)
    {
        auto rom = Rom::Load(path);
        if (!rom){
            return std::unexpected("Failed to load ROM: " + rom.error());
        }

        m_rom = std::move(rom.value());

        return true;
    }

    std::expected<bool, std::string> Cartridge::ParseHeader()
    {
        const std::string header = "NES\x1A";

        const std::string_view headerView(
            reinterpret_cast<char const*>(m_rom->GetData().data()),
            header.size()
        );

        if (header != headerView) {
            return std::unexpected("header signature not found");
        }

        m_prgRomSize = m_rom->GetData()[headerPositionPRGSize] * prgRomMultiplier;
        m_chrRomSize = m_rom->GetData()[headerPositionCHRSize] * chrRomMultiplier;

        return true;
    }

    const std::vector<uint8_t>& Cartridge::GetRomData() const
    {
        return m_rom->GetData();
    }

    uint32_t Cartridge::GetRomSize() const
    {
        return m_rom->GetSize();
    }

    uint32_t Cartridge::GetHeaderLocation() const
    {
        return 0;
    }

    uint32_t Cartridge::GetPrgRomSize() const
    {
        return m_prgRomSize;
    }

    uint32_t Cartridge::GetChrRomSize() const
    {
        return m_chrRomSize;
    }

    std::span<const uint8_t> Cartridge::GetPrgRomData() const
    {
        return {
            m_rom->GetData().data() + headerSize,
            m_prgRomSize
        };
    }

    std::span<const uint8_t> Cartridge::GetChrRomData() const
    {
        return {
            m_rom->GetData().data() + headerSize + m_prgRomSize,
            m_chrRomSize
        };
    }
} // nes