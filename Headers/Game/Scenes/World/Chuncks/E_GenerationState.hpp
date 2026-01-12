#ifndef __E_BUILDSTATE_HPP__
#define __E_BUILDSTATE_HPP__

namespace Vox::Game::Generation
{
    enum class E_GenerationState
    {
        WaitingThread = 0,
        Cache = 1,
        Terrain = 2,
        Mesh = 3,
        Decoration = 4,
        WaitingBuffer = 5,
        End = 6,

        Cancelled = 10
    };

    /***
     Official generation Steps.
     
     1. empty: The chunk is not yet loaded or generated.

     2. structures_starts: This step calculates the starting points for structure pieces. For structures that start in this chunk, the position of all pieces are generated and stored.

     3. structures_references: A reference to nearby chunks that have a structures' starting point are stored.

     4. biomes: Biomes are determined and stored. No terrain is generated at this stage.

     5. noise: The base terrain shape and liquid bodies are placed.

     6. surface: The surface of the terrain is replaced with biome-dependent blocks.

     7. carvers: Carvers carve certain parts of the terrain and replace solid blocks with air.

     8. features: Features and structure pieces are placed and heightmaps are generated.

     9. initialize_light: The lighting engine is initialized and light sources are identified.

     10. light: The lighting engine calculates the light level for blocks.

     11. spawn: Mobs are spawned.

     12. full: Generation is done and a chunk can now be loaded. The proto-chunk is now converted to a level chunk and all block updates deferred in the above steps are executed.
     
     * ***/

} // namespace Vox::Game::Generation


#endif // __E_BUILDSTATE_HPP__