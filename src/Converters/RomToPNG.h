#ifndef MASTERSYSTEM_DISSASSEMBLER_ROMTOPNG_H
#define MASTERSYSTEM_DISSASSEMBLER_ROMTOPNG_H

#include <array>
#include <expected>
#include <filesystem>
#include <string>
#include <span>

#include <png.h>

#include "../Cartridges/ICartridge.h"
#include "ITileDecoder.h"

class RomToPNG
{
public:
    RomToPNG();

    std::expected<bool, std::string> Convert(const ICartridge& cartridge, FILE* file);

    void SetPalette(const std::array<uint8_t, 16>& palette);

private:
    std::array<uint8_t, 16> m_palette{};
    std::vector<png_byte> m_pngData{};
    uint32_t m_tileWidth = {8};
    uint32_t m_tileHeight = {8};
    uint32_t m_tilesPerRow = {32};
    uint32_t m_bitsPerPixel = {2};

    std::span<const uint8_t> getGraphicData(const ICartridge& cartridge);
    static std::unique_ptr<ITileDecoder> getTileDecoder(const ICartridge& cartridge);
    std::array<uint8_t, 3> smsColorIdxToPNGColor(uint8_t color) const;
    static std::array<uint8_t, 3> smsColorToPNGColor(uint8_t color);
};


#endif //MASTERSYSTEM_DISSASSEMBLER_ROMTOPNG_H
