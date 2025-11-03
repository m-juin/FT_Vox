#include "JsonLib/Json.hpp"

int main()
{
	auto dict = JSONLib::LoadJSON("./Assets/TexturesPacks/1/Generation/BiomesRules.json");
    
	for (auto &subDict : dict.As<JSONLib::JSONObject>())
    {
        dict[subDict.first]["TreeDensity"] = 0.0;
    }
    JSONLib::ExportToJSON(dict, "./Assets/TexturesPacks/1/Generation/BiomesRules2.json");
}