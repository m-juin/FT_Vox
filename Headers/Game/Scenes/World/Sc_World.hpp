#ifndef __SC_WORLD_HPP__
#define __SC_WORLD_HPP__

#include "Front/Scenes/AScene.hpp"
#include "Front/Scenes/ScenesManager.hpp"

namespace Vox::Game::Scenes::World
{
	class Sc_World : public Front::Scenes::AScene
	{
		public:
			static constexpr const char *getName()
			{
				return "World";
			}

			Sc_World() : AScene() {}
			~Sc_World() override = default;

			void Load() override;
			void Unload() override;
			void Render() override;
			void InitSceneData() override;

			const std::string &GetName() const override
			{
				static const std::string name = getName();
				return name;
			}

		private:
			/* private */
	};
} // namespace Vox::Game::Scenes::World

template <> void Vox::Front::Scenes::RegisterSceneFactory<Vox::Game::Scenes::World::Sc_World>();

#endif // __SC_WORLD_HPP__