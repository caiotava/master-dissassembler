#ifndef MASTERSYSTEM_DISSASSEMBLER_CONFIG_H
#define MASTERSYSTEM_DISSASSEMBLER_CONFIG_H
#include <expected>
#include <filesystem>

class Config
{
public:
    Config(int32_t argc, char* argv[]);

    std::expected<bool, std::string> Parse();
    std::expected<bool, std::string> Validate() const;

    const std::filesystem::path& GetRomPath() const;
    bool ShouldConvertPng() const;
    bool ShouldExtractChrRom() const;

    static std::string GetHelpText();

private:
    int32_t m_argc;
    char** m_argv;

    std::filesystem::path m_romPath;
    bool m_convertPng = false;
    bool m_extractChrRom = false;

    std::string getNextParam(const uint32_t i) const;
};


#endif //MASTERSYSTEM_DISSASSEMBLER_CONFIG_H
