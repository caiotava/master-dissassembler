#include "TileDecoder.h"
#include "Cartridge.h"
#include <span>

using namespace mastersystem;

TileDecoder::TileDecoder()
{
}

uint32_t TileDecoder::GetBytesPerTile() const
{
    return 4;
}

std::vector<uint8_t> TileDecoder::Bitplane(const std::span<const uint8_t> tileData, const uint32_t line) const
{
    return {
        tileData[line * GetBytesPerTile() + 0],
        tileData[line * GetBytesPerTile() + 1],
        tileData[line * GetBytesPerTile() + 2],
        tileData[line * GetBytesPerTile() + 3]
    };
}
