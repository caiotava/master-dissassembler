#include "System.h"

#include <Systems/MasterSystem/Cartridge.h>
#include <Systems/Nes/Cartridge.h>

std::expected<std::unique_ptr<System>, std::string> System::CreateSystemFromRom(const std::filesystem::path& path)
{
    auto system = std::make_unique<System>();
    auto cartridge = createCartridge(path);
    if (!cartridge){
        return std::unexpected("creating cartridge: " + cartridge.error());
    }

    system->m_cartridge = std::move(cartridge.value());

    if (auto loadResult = system->m_cartridge->LoadRom(path); !loadResult){
        return std::unexpected("loading rom: " + loadResult.error());
    }

    if (auto parseResult = system->m_cartridge->ParseHeader(); !parseResult){
        return std::unexpected("parsing header: " + parseResult.error());
    }

    return system;
}

const ICartridge& System::GetCartridge() const
{
    return *m_cartridge;
}

std::expected<std::unique_ptr<ICartridge>, std::string> System::createCartridge(const std::filesystem::path& path)
{
    if (path.extension() == ".sms"){
        return std::make_unique<mastersystem::Cartridge>();
    }

    if (path.extension() == ".nes"){
        return std::make_unique<nes::Cartridge>();
    }

    return std::unexpected("unsupported file type: " + path.string());
}
