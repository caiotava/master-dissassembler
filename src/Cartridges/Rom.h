#ifndef MASTERSYSTEM_DISSASSEMBLER_ROM_H
#define MASTERSYSTEM_DISSASSEMBLER_ROM_H

#include <expected>
#include <filesystem>

class Rom
{
public:
    Rom();

    static std::expected<std::unique_ptr<Rom>, std::string> Load(const std::filesystem::path& path);

    const std::vector<uint8_t>& GetData() const;
    std::filesystem::path GetPath() const;
    uint32_t GetSize() const;

private:
    std::filesystem::path m_path;
    std::vector<uint8_t> m_data;
    uint32_t m_size = {0};
};


#endif //MASTERSYSTEM_DISSASSEMBLER_ROM_H
