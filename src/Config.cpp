#include "Config.h"

#include <ranges>

Config::Config(const int32_t argc, char* argv[]) : m_argc(argc), m_argv(argv)
{
}

const std::string convertPngParam = "--convert-png";
const std::string extractChrParam = "--extract-chr";

std::expected<bool, std::string> Config::Parse()
{
    for (int32_t x = 1; x < m_argc; ++x){
        std::string arg(m_argv[x]);

        if (arg == "-h" || arg == "--help") {
            return std::unexpected("help");
        }

        if (arg.starts_with(convertPngParam)){
            m_convertPng = true;
            continue;
        }

        if (arg.starts_with(extractChrParam)) {
            m_extractChrRom = true;
            continue;
        }

        m_romPath = arg;
    }

    return true;
}

std::expected<bool, std::string> Config::Validate() const
{
    if (m_romPath.empty()){
        return std::unexpected("rom path is empty");
    }

    if (!std::filesystem::exists(m_romPath)){
        return std::unexpected("rom path does not exist");
    }

    return true;
}

const std::filesystem::path& Config::GetRomPath() const
{
    return m_romPath;
}

bool Config::ShouldConvertPng() const
{
    return m_convertPng;
}

bool Config::ShouldExtractChrRom() const
{
    return m_extractChrRom;
}

std::string Config::GetHelpText()
{
    return R"(
     Usage:
         master-dissassembler --extract-chr --convert-png <rom-path>

     Options:
         --extract-chr       - If the cartridge support CHR rom it extracts the data in a file - default: false
         --convert-png       - Convert the rom in a png representation.                        - default: false

     Examples:
     master-dissassembler --extract-chr --convert-png rom.nes
     )";
}

std::string Config::getNextParam(const uint32_t i) const
{
    if (i+1 >= m_argc) {
        return "";
    }

    return m_argv[i+1];
}
