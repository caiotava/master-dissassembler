#ifndef MASTER_DISSASSEMBLER_ICARTRIDGE_H
#define MASTER_DISSASSEMBLER_ICARTRIDGE_H

#include <expected>
#include <filesystem>

class ICartridge
{
public:
    enum class Type
    {
        MasterSystem,
        Nes,
    };

    virtual ~ICartridge() = default;
    virtual std::expected<bool, std::string> LoadRom(const std::filesystem::path& path) = 0;
    virtual std::expected<bool, std::string> ParseHeader() = 0;

    virtual const std::vector<uint8_t>& GetRomData() const = 0;
    virtual uint32_t GetRomSize() const = 0;
    virtual uint32_t GetHeaderLocation() const = 0;
    virtual Type GetType() const = 0;
};

#endif //MASTER_DISSASSEMBLER_ICARTRIDGE_H
