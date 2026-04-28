#include "Cartridge.h"
#include <expected>

using namespace mastersystem;

Cartridge::Cartridge()
{
}

std::expected<bool, std::string> Cartridge::LoadRom(const std::filesystem::path& path)
{
    auto rom = Rom::Load(path);
    if (!rom){
         return std::unexpected(rom.error());
    }

    m_rom = std::move(rom.value());

    return true;
}

std::expected<bool, std::string> Cartridge::ParseHeader()
{
    if (auto err = identifyHeaderLocation(); !err) {
        return err;
    }

    if (auto err = identifyZone(); !err) {
        return err;
    }

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
    return m_headerLocation;
}

std::expected<bool, std::string> Cartridge::identifyHeaderLocation()
{
    static const std::vector<uint16_t> headerLocations = {0x7FF0, 0x1FF0, 0x3FF0};

    std::string error{};

    for (auto const location : headerLocations) {
        if (auto err = validateHeader(location); !err) {
            error = err.error();
            continue;
        }

        m_headerLocation = location;
        break;
    }

    if (!error.empty()) {
        return std::unexpected(error);
    }

    return true;
}

std::expected<bool, std::string> Cartridge::validateHeader(const uint16_t location) const
{
    static const std::string smsHeader = "TMR SEGA";

    if (location + smsHeader.size() > m_rom->GetSize()) {
        return std::unexpected("header is out of bounds");
    }

    const std::string_view headerView(
        reinterpret_cast<char const*>(m_rom->GetData().data() + location),
        smsHeader.size()
        );

    if (smsHeader != headerView) {
        return std::unexpected("header location not found");
    }

    return true;
}

std::expected<bool, std::string> Cartridge::identifyZone()
{
    const uint32_t zoneLocation = m_headerLocation + 0x0F;

    if (zoneLocation > m_rom->GetSize()) {
        return std::unexpected("zone location is out of bounds");
    }

    const auto zone = m_rom->GetData()[zoneLocation] >> 4 & 0x0F;

    switch (zone) {
    case 3:
        m_romZone = Zone::JapanSMS;
        break;
    case 4:
        m_romZone = Zone::InternationalSMS;
        break;
    default:
        m_romZone = Zone::Unknown;
        break;
    }

    return true;
}