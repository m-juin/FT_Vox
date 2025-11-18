#include "JsonLib/Json.hpp"

int main()
{
	auto dict = JSONLib::LoadJSON("./Assets/TexturesPacks/1/Generation/BiomesRules.json");
    
	for (auto &subDict : dict.As<JSONLib::JSONObject>())
    {
        dict[subDict.first]["TreesData"] = JSONLib::JSONObject();
        dict[subDict.first]["TreesData"]["TreeChance"] = dict[subDict.first]["TreeChance"];
        dict[subDict.first]["TreesData"]["TreeTypes"] = JSONLib::JSONVector();
        dict[subDict.first].As<JSONLib::JSONObject>().erase("TreeChance");
    }
    JSONLib::ExportToJSON(dict, "./Assets/TexturesPacks/1/Generation/BiomesRules2.json");
}