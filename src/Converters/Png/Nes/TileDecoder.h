#ifndef MASTERSYSTEM_DISSASSEMBLER_TILEDECODER_NES_H
#define MASTERSYSTEM_DISSASSEMBLER_TILEDECODER_NES_H

#include <Converters/Png/ITileDecoder.h>

namespace nes
{
    class TileDecoder : public ITileDecoder
    {
    public:
        TileDecoder();
        ~TileDecoder() override = default;

        uint32_t GetBytesPerTile() const override;
        std::vector<uint8_t> Bitplane(std::span<const uint8_t> tileData, uint32_t line) const override;

    private:
        uint32_t m_tileWidth = {8};
    };
} // nes

#endif //MASTERSYSTEM_DISSASSEMBLER_TILEDECODER_NES_H
