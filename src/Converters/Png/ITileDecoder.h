#ifndef MASTERSYSTEM_DISSASSEMBLER_ITILEDECODER_H
#define MASTERSYSTEM_DISSASSEMBLER_ITILEDECODER_H

#include <cstdint>
#include <span>
#include <vector>

class ITileDecoder
{
public:
    virtual ~ITileDecoder() = default;

    virtual uint32_t GetBytesPerTile() const = 0;
    virtual std::vector<uint8_t> Bitplane(std::span<const uint8_t> tileDat, uint32_t line) const = 0;
};

#endif //MASTERSYSTEM_DISSASSEMBLER_ITILEDECODER_H
