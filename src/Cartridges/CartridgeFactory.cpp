#include "CartridgeFactory.h"

#include "Cartridges/MasterSystem/Cartridge.h"
#include "Cartridges/Nes/Cartridge.h"

std::unique_ptr<ICartridge> CartridgeFactory::CreateCartridge(const std::filesystem::path& path)
{
    if (path.extension() == ".sms"){
        return std::make_unique<mastersystem::Cartridge>();
    } else if (path.extension() == ".nes"){
        return std::make_unique<nes::Cartridge>();
    }

    return nullptr;
}
