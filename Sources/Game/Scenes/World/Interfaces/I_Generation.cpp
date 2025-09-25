
#include "Game/Scenes/World/Interfaces/I_Generation.hpp"

#include "Front/Interfaces/Elements/Buttons/ColoredButton.hpp"
#include "Front/Interfaces/Elements/DynamicImage.hpp"
#include "Front/Interfaces/Elements/Image.hpp"
#include "Front/Interfaces/Elements/InputField.hpp"
#include "Front/Interfaces/Elements/Slider.hpp"
#include "Front/Interfaces/Elements/Text.hpp"

#include "MathGraphicalLib/Utils.hpp"

#include "Game/Scenes/World/WorldManager.hpp"

#include <cstring>
#include <sstream>

#include "Game/GameManager.hpp"

#include "GLFW/glfw3.h"

#include "Game/Scenes/World/Generation/BiomesPerlin.hpp"

namespace Vox::Game::Scenes::World::Interfaces
{
	using namespace Front::Interfaces::Elements;
	I_Generation::I_Generation(Vox::Front::Interfaces::Elements::Vector2 pos,
							   Vox::Front::Interfaces::Elements::Vector2 size)
		: AInterface(pos, size), AUpdatable(10)
	{
		{ // IF_Seed
			auto seed = Game::World::WorldManager::GetInstance().GetGenerationManager().GetSeed();
			std::stringstream ss;
			ss << seed;
			InputField::Constructor pm{};
			pm.BGAtlas = "";
			pm.BGAtlasKey = "";
			pm.inputMode = InputField::E_InputMode::Numeric;
			pm.inputScale = 0.35f;
			pm.pos = {pos[0] + 50, pos[1] + 50};
			pm.size = {300, 35};
			pm.defaultValue = ss.str();

			this->AddElement("IF_Seed", std::make_unique<InputField>(pm));
		}

		{ // Splines Sliders
			auto sManager = Game::GameManager::GetInstance().GetSplineManager();
			auto spl = sManager.GetSplinesCopy();

			Slider::Constructor pm{};
			pm.pos = {pos[0] + size[0] - 300, pos[1] + 50};
			pm.size = {200, 25};

			Text::Vox_Text_Constructor Tpm{};
			Tpm.color = {0.5, 0.5, 0.5, 1.0};
			Tpm.scale = 0.4;
			Tpm.pos = {pos[0] + size[0] - 500, pm.pos[1] + 16};

			for (auto pair : spl)
			{
				Tpm.content = pair.first;
				pm.defaultValue = pair.second.second;
				auto SL = std::make_unique<Slider>(pm);
				std::string key = "SL_" + pair.first;

				SL->onValueChange.AddCallBack(
					[this, key](const float &newVal, const float &prevVal)
					{
						(void)prevVal;
						auto sManager = Game::GameManager::GetInstance().GetSplineManager();

						auto names = sManager.GetSplinesNames();
						int n = (int)names.size();
						if (n < 2)
						{
							return;
						}

						std::vector<Slider *> sliders;
						std::vector<double> vals(n);
						for (int i = 0; i < n; i++)
						{
							sliders.push_back(this->GetElement<Slider>("SL_" + names[i]));
							vals[i] = sliders.back()->GetValue();
						}

						std::string changedName = key.substr(3);
						int idxChanged = 0;
						for (int i = 0; i < n; i++)
							if (names[i] == changedName)
								idxChanged = i;

						double requested = std::clamp<double>(newVal, 0.0f, 1.0f);
						double delta = requested - prevVal;

						if (std::abs(delta) > 1e-9)
						{
							if (delta > 0.0)
							{
								double totalCap = 0.0;
								std::vector<double> cap(n);
								for (int i = 0; i < n; i++)
									if (i != idxChanged)
									{
										cap[i] = vals[i] - 0.0f;
										totalCap += cap[i];
									}
								delta = std::min(delta, totalCap);
								for (int i = 0; i < n; i++)
									if (i != idxChanged && cap[i] > 0)
									{
										double take = (cap[i] / totalCap) * delta;
										vals[i] -= take;
									}
								vals[idxChanged] = prevVal + delta;
							}
							else
							{
								double need = -delta;
								double totalCap = 0.0;
								std::vector<double> cap(n);
								for (int i = 0; i < n; i++)
									if (i != idxChanged)
									{
										cap[i] = 1.0f - vals[i];
										totalCap += cap[i];
									}
								need = std::min(need, totalCap);
								for (int i = 0; i < n; i++)
									if (i != idxChanged && cap[i] > 0)
									{
										double give = (cap[i] / totalCap) * need;
										vals[i] += give;
									}
								vals[idxChanged] = prevVal - need;
							}
						}

						std::vector<int> cents(n);
						int sum = 0;
						for (int i = 0; i < n; i++)
						{
							cents[i] = (int)std::round(vals[i] * 100);
							sum += cents[i];
						}
						int diff = 100 - sum;
						if (diff != 0)
						{
							cents[idxChanged] = std::clamp(cents[idxChanged] + diff, 0, 100);
						}

						for (int i = 0; i < n; i++)
							sliders[i]->SetValue(cents[i] / 100.0f);
					});

				this->AddElement(key, std::move(SL));
				this->AddElement("TXT_" + pair.first, std::make_unique<Text>(Tpm));
				pm.pos[1] += 50;
				Tpm.pos[1] += 50;
			}
		}

		{ // BTN_Update
			Buttons::ColoredButton::Vox_ColorButton_Constructor pm{};
			pm.bgColor = {1.0, 1.0, 1.0, 0.5};
			pm.content = "U";
			pm.disabledBGColor = {0.5, 0.5, 0.5, 0.1};
			pm.disabledTXTColor = {0.5, 0.5, 0.5, 1.0};
			pm.hoverBGColor = {1.0, 1.0, 1.0, 0.7};
			pm.hoverTXTColor = {0.0, 0.0, 0.0, 1.0};
			pm.pos = {pos[0] + 50, pos[1] + size[1] - 100};
			pm.size = {50, 50};
			pm.textColor = {0.2, 0.2, 0.2, 1.0};
			pm.textScale = 0.8f;

			auto btn = std::make_unique<Buttons::ColoredButton>(pm);

			btn->onClickCallbacks.AddCallBack(
				[this](const int &button, const int &action)
				{
					if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_RELEASE)
						return;

					std::unordered_map<std::string, float> newVals;

					auto &sManager = Game::GameManager::GetInstance().GetOwnedSplineManager();
					auto spl = sManager.GetSplinesCopy();
					uint64_t seed = std::stoull(this->GetElement<InputField>("IF_Seed")->GetValue());
					for (auto name : spl)
					{
						float val = this->GetElement<Slider>("SL_" + name.first)->GetValue();
						newVals[name.first] = val;
					}

					sManager.SetNewWeight(newVals);
					Game::World::WorldManager::GetInstance().UpdateSeed(seed);
				});

			this->AddElement("BTN_Update", std::move(btn), 1);
		}

		{ // IMG_Biomes && BTN_DisplayIMG

			DynamicImage::Constructor imgPM{};
			imgPM.pos = {this->_pos[0] + size[0] / 2 - 200, this->_pos[1] + size[1] / 2 - 200};
			imgPM.size = {400, 400};
			imgPM.defaultData.resize(400 * 400 * 4);
			for (size_t i = 0; i < 400 * 400; ++i)
			{
				imgPM.defaultData[i * 4 + 0] = 255; // R
				imgPM.defaultData[i * 4 + 1] = 255; // G
				imgPM.defaultData[i * 4 + 2] = 255; // B
				imgPM.defaultData[i * 4 + 3] = 255; // A
			}
			auto img = std::make_unique<DynamicImage>(imgPM);
			this->AddElement("IMG_Biome", std::move(img), 2, false);

			this->UpdateMap({0, 0});

			Buttons::ColoredButton::Vox_ColorButton_Constructor pm{};
			pm.bgColor = {1.0, 1.0, 1.0, 0.5};
			pm.content = "D";
			pm.disabledBGColor = {0.5, 0.5, 0.5, 0.1};
			pm.disabledTXTColor = {0.5, 0.5, 0.5, 1.0};
			pm.hoverBGColor = {1.0, 1.0, 1.0, 0.7};
			pm.hoverTXTColor = {0.0, 0.0, 0.0, 1.0};
			pm.pos = {pos[0] + size[0] - 100, pos[1] + size[1] - 100};
			pm.size = {50, 50};
			pm.textColor = {0.2, 0.2, 0.2, 1.0};
			pm.textScale = 0.8f;

			auto btn = std::make_unique<Buttons::ColoredButton>(pm);

			btn->onClickCallbacks.AddCallBack(
				[this](const int &button, const int &action)
				{
					if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_RELEASE)
						return;
					auto &imgContainer = this->GetContainerElement("IMG_Biome");
					imgContainer.isVisible = !imgContainer.isVisible;
				});

			this->AddElement("BTN_Display_IMG_Biomes", std::move(btn), 1);
		}

		this->_enabled = false;
	}

	I_Generation::~I_Generation() {}
	
	void I_Generation::UpdateMap(MGL::Vectors::Vector2<int> playerPos)
	{
		auto img = this->GetElement<DynamicImage>("IMG_Biome");
		auto newData = Game::Generation::Perlins::GenerateBiomeImage(playerPos, Game::World::WorldManager::GetInstance().GetGenerationManager().GetSeed(), 400, 4);
		img->SetData(newData);
	}

	void I_Generation::SetPos(const Vox::Front::Interfaces::Elements::Vector2 newPos)
	{
		if (newPos == this->_pos)
			return;
		this->FindElement("TXT_Seed")->elem->SetPos({this->_pos[0] + 50, this->_pos[1] + 50});
	}

	void I_Generation::SetSize(const Vox::Front::Interfaces::Elements::Vector2 newSize)
	{
		if (newSize == this->_size)
			return;
		_size[0] = MGL::Utils::findNextMultiple(newSize[0], 16.f);
		_size[1] = MGL::Utils::findNextMultiple(newSize[1], 16.f);
	}

	void I_Generation::Render()
	{
		for (auto &elem : this->_content)
		{
			if (elem.elem && elem.isVisible)
			{
				elem.elem->Draw();
			}
		}
	}
} // namespace Vox::Game::Scenes::World::Interfaces