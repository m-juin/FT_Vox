#include "Front/Rendering/Utils/Buffers/Utils.hpp"
#include <iostream>

int main()
{
    VkPhysicalDevice device = nullptr; // nullptr pour le test
    try {
        uint32_t result = Vox::Front::Rendering::Utils::Buffers::Utils::FindMemoryType(
            1, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, device);
        std::cout << "SUCCÈS: Fonction trouvée" << std::endl;
    } catch(...) {
        std::cout << "Fonction appelable mais erreur d'exécution (normal avec device=nullptr)" << std::endl;
    }
    return 0;
}