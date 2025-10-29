#include "JsonLib/Json.hpp"

// void AddKey(JSONLib::Dictionnary rootDict, std::vector<std::string> keyToEmplacement, const std::string &newElemKey,
// 			JSONLib::Dictionnary toAdd)
// {
// 	JSONLib::Dictionnary &current = rootDict;
// }

int main()
{
	auto dict = JSONLib::LoadJSON("./Assets/TexturesPacks/1/Generation/BiomesRules.json");
    
	for (auto &subDict : dict.As<JSONLib::JSONObject>())
    {
        dict[subDict.first]["TreeDensity"] = 0.0;
    }
    JSONLib::ExportToJSON(dict, "./Assets/TexturesPacks/1/Generation/BiomesRules2.json");
}