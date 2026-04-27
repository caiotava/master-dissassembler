#include "RomToPNG.h"

#include "Cartridges/MasterSystem/Cartridge.h"
#include "Cartridges/Nes/Cartridge.h"
#include "Converters/Png/MasterSystem/TileDecoder.h"
#include "Converters/Png/Nes/TileDecoder.h"

#define DEFAULT_PALETTE 0x1A, 0x00, 0x3F, 0x18, 0x1B, 0x2F, 0x07, 0x06, 0x15, 0x1A, 0x2A, 0x28, 0x06, 0x3E, 0x15, 0x2A

constexpr uint32_t PNG_COLOR_FORMAT_BYTES = 3; // RGB one byte per color

RomToPNG::RomToPNG()
{
    m_palette = std::array<uint8_t, 16>{DEFAULT_PALETTE};
}

std::expected<bool, std::string> RomToPNG::Convert(const ICartridge& cartridge, FILE* file)
{
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

    if (!png_ptr){
        return std::unexpected("failed to create png struct");
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);

    if (!info_ptr){
        return std::unexpected("failed to create png info struct");
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }

    if (setjmp(png_jmpbuf(png_ptr))){
        return std::unexpected("error during png creation");
    }

    png_init_io(png_ptr, file);

    auto graphicData = getGraphicData(cartridge);

    const uint32_t tileSize = m_tileWidth * m_tileHeight;
    const uint8_t tileSizeBytes = tileSize * m_bitsPerPixel / m_tileWidth;
    const uint32_t imageWidth = m_tileWidth * m_tilesPerRow;
    const uint32_t tilesCount = graphicData.size() / tileSizeBytes + m_tilesPerRow - 1;
    const uint32_t imageHeight = tilesCount / m_tilesPerRow * m_tileHeight;

    png_set_IHDR(
        png_ptr,
        info_ptr,
        imageWidth,
        imageHeight,
        8,
        PNG_COLOR_TYPE_RGB,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );

    m_pngData.resize(imageWidth * imageHeight * PNG_COLOR_FORMAT_BYTES, 0);

    auto tileDecoder = getTileDecoder(cartridge);

    uint32_t spriteIdx = 0;
    for (uint32_t x = 0; x < graphicData.size(); x += tileSizeBytes, ++spriteIdx){
        const auto begin = graphicData.begin() + x;
        const auto end = begin + tileSizeBytes;
        auto tileData = std::vector(begin, end);

        const uint32_t baseLine = spriteIdx / m_tilesPerRow;
        const uint32_t baseColumn = spriteIdx % m_tilesPerRow;
        for (uint8_t line = 0; line < m_tileHeight; ++line){
            const auto bitplanes = tileDecoder->Bitplane(tileData, line);

            for (int8_t px = m_tileWidth-1; px >= 0; --px){
                uint8_t color = {0};
                for (auto idxBpp = 0; idxBpp < bitplanes.size(); ++idxBpp){
                    color = color | ((bitplanes[idxBpp] >> px) & 1) << idxBpp;
                }

                auto [r, g, b] = smsColorIdxToPNGColor(color);

                const uint32_t xPixel = baseColumn * m_tileWidth + (m_tileWidth - px - 1);
                const uint32_t yPixel = baseLine * m_tileHeight + line;

                const uint32_t pos = (yPixel * imageWidth + xPixel) * PNG_COLOR_FORMAT_BYTES;

                m_pngData[pos + 0] = r;
                m_pngData[pos + 1] = g;
                m_pngData[pos + 2] = b;
            }
        }
    }

    std::vector<png_bytep> rowsPtr;
    rowsPtr.reserve(imageHeight);
    for (uint32_t y = 0; y < imageHeight; ++y){
        rowsPtr.push_back(&m_pngData[y * imageWidth * PNG_COLOR_FORMAT_BYTES]);
    }

    png_write_info(png_ptr, info_ptr);
    png_write_rows(png_ptr, rowsPtr.data(), rowsPtr.size());

    png_write_end(png_ptr, info_ptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);

    return true;
}

void RomToPNG::SetPalette(const std::array<uint8_t, 16>& palette)
{
    m_palette = palette;
}

std::span<const uint8_t> RomToPNG::getGraphicData(const ICartridge& cartridge)
{
    if (cartridge.GetType() == ICartridge::Type::Nes){
        return dynamic_cast<const nes::Cartridge&>(cartridge).GetChrRomData();
    }

    m_bitsPerPixel = 4;
    return {
        cartridge.GetRomData().data(),
        cartridge.GetRomSize()
    };
}

std::array<uint8_t, 3> RomToPNG::smsColorIdxToPNGColor(const uint8_t color) const
{
    if (color >= m_palette.size()){
        return {};
    }

    return smsColorToPNGColor(m_palette[color]);
}

std::array<uint8_t, 3> RomToPNG::smsColorToPNGColor(const uint8_t color)
{
    uint8_t r = color & 0b11;
    uint8_t g = (color >> 2) & 0b11;
    uint8_t b = (color >> 4) & 0b11;

    r = (r * 255) / 3;
    g = (g * 255) / 3;
    b = (b * 255) / 3;

    return {r, g, b};
}

std::unique_ptr<ITileDecoder> RomToPNG::getTileDecoder(const ICartridge& cartridge)
{
    if (cartridge.GetType() == ICartridge::Type::Nes){
        return std::make_unique<nes::TileDecoder>();
    }

    return std::make_unique<mastersystem::TileDecoder>();
}
