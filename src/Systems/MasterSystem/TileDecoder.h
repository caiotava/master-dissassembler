
#ifndef MASTERSYSTEM_DISSASSEMBLER_TILEDECODER_SMS_H
#define MASTERSYSTEM_DISSASSEMBLER_TILEDECODER_SMS_H

#include <Converters/ITileDecoder.h>

namespace mastersystem
{
    class TileDecoder : public ITileDecoder
    {
    public:
        TileDecoder();
        ~TileDecoder() override = default;

        uint32_t GetBytesPerTile() const override;
        std::vector<uint8_t> Bitplane(std::span<const uint8_t> tileData, uint32_t line) const override;
    };
} // mastersystem

#endif //MASTERSYSTEM_DISSASSEMBLER_TILEDECODER_SMS_H
