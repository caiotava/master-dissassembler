#include "TileDecoder.h"

namespace nes
{
    TileDecoder::TileDecoder()
    {
    }

    uint32_t TileDecoder::GetBytesPerTile() const
    {
        return 2;
    }

    std::vector<uint8_t> TileDecoder::Bitplane(std::span<const uint8_t> tileData, const uint32_t line) const
    {
        return {
            tileData[line],
            tileData[line + m_tileWidth]
        };
    }
} // nes
