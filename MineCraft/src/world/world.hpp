#pragma once

#include "chunk.hpp"
#include "worldgenerator.hpp"

class World{
    private:
        Chunk dummyChunk;
        std::vector<Chunk*> chunks;
        WorldGenerator worldGenerator;

        int worldSizeX;
        int worldSizeZ;
        
        int renderDistance;

        
    
    public:
        World();
        ~World();

        void initialize(int pWorldSizeX, int pWorldSizeZ, int pRenderDistance);
        void generateWorld();

        void render(const cgp::vec3& position, const cgp::environment_generic_structure& environment);
        
        //Chunk Acces
        Chunk* getChunkAt(const cgp::vec3& worldPos) const;
        // Block access
        BlockType getBlock(const cgp::vec3& worldPos) const;
        void setBlock(const cgp::vec3& worldPos, BlockType blockType);
        Block* getBlockObject(const cgp::vec3& worldPos) const;

        int getRenderDistance() const;
};