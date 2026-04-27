#include "Rom.h"
#include <fstream>

Rom::Rom()
{
}

std::expected<std::unique_ptr<Rom>, std::string> Rom::Load(const std::filesystem::path& path)
{
    if (!std::filesystem::exists(path)){
        return std::unexpected("File not found: " + path.string());
    }

    auto rom = std::make_unique<Rom>();

    rom->m_path = path;
    std::ifstream file(rom->m_path, std::ios::binary);

    if (!file.is_open()){
        return std::unexpected("failed to open file: " + rom->m_path.string());
    }

    rom->m_size = file.seekg(0, std::ios::end).tellg();
    file.seekg(0, std::ios::beg);
    rom->m_data.resize(rom->m_size, 0);

    file.read(reinterpret_cast<char*>(rom->m_data.data()), rom->m_size);

    return rom;
}

const std::vector<uint8_t>& Rom::GetData() const
{
    return m_data;
}


std::filesystem::path Rom::GetPath() const
{
    return m_path;
}

uint32_t Rom::GetSize() const
{
    return m_size;
}
