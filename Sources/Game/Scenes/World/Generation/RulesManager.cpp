#include "Game/Scenes/World/Generation/RulesManager.hpp"

#include "JsonLib/Json.hpp"
#include "Game/GameManager.hpp"

namespace Vox::Game::Generation::Datas::Biomes
{
    RulesManager::RulesManager()
    {
        std::string path = Game::GameManager::GetInstance().GetTexturePackPath() + "Generation/BiomesRules.json";
        this->_rules = JSONLib::LoadJSON(path);
    }
}