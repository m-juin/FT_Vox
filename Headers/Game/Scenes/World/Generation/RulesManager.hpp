#ifndef __RULESMANAGER_HPP__
#define __RULESMANAGER_HPP__

#include "Utils/Singleton.hpp"

#include "JsonLib/Struct.hpp"
#include "Game/Datas/BiomesData/SurfaceDecoration.hpp"

namespace Vox::Game::Generation::Datas::Biomes
{
    class RulesManager : public Vox::Utils::Singleton<RulesManager>
    {
        friend class Vox::Utils::Singleton<RulesManager>;

        public:
            RulesManager();
            ~RulesManager() {};
            static Game::Datas::Blocks::BlockType GetBlockType(Biomes biome, int depth);
            static float GetHeight(Biomes biome, const float &val);
            static float GetTreeChance(Biomes biome);
            std::unordered_map<const Biomes, const SurfaceDecoration> decorationRule;
        private:
            // JSONLib::Dictionnary _rules;
            /* private */
    
    };
} // namespace Vox::Game::Generation::Datas::Biomes


#endif // __RULESMANAGER_HPP__