#ifndef __RULESMANAGER_HPP__
#define __RULESMANAGER_HPP__

#include "Utils/Singleton.hpp"

#include "JsonLib/Struct.hpp"

namespace Vox::Game::Generation::Datas::Biomes
{
    class RulesManager : public Vox::Utils::Singleton<RulesManager>
    {
        friend class Vox::Utils::Singleton<RulesManager>;

        public:
            RulesManager();
            ~RulesManager() {};
    
        private:
            JSONLib::Dictionnary _rules;
            /* private */
    
    };
} // namespace Vox::Game::Generation::Datas::Biomes


#endif // __RULESMANAGER_HPP__