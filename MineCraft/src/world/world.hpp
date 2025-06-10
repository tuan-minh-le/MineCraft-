#pragma once

#include "chunk.hpp"
#include "worldgenerator.hpp"
#include "frustum.hpp"

class World{
    private:
        Chunk dummyChunk;
        std::vector<Chunk*> chunks;
        WorldGenerator worldGenerator;

        int worldSizeX;
        int worldSizeZ;

        Frustum frustum;
        
        int renderDistance;

        
    
    public:
        World();
        ~World();

        void initialize(int pWorldSizeX, int pWorldSizeZ, int pRenderDistance);
        void generateWorld();

        void renderBasic(const cgp::vec3& position, const cgp::environment_generic_structure& environment);

        void renderCached(const cgp::vec3& position, const cgp::environment_generic_structure& environment);
        
        void renderInstanced(const cgp::vec3& position, const cgp::environment_generic_structure& environment);
        //Chunk Acces
        Chunk* getChunkAt(const cgp::vec3& worldPos) const;
        void renderFrustum(const cgp::vec3& position, const cgp::environment_generic_structure& environment, 
                          const cgp::mat4& viewMatrix, const cgp::mat4& projectionMatrix);

        // Block access
        BlockType getBlock(const cgp::vec3& worldPos) const;
        void setBlock(const cgp::vec3& worldPos, BlockType blockType);
        Block* getBlockObject(const cgp::vec3& worldPos) const;

        int getRenderDistance() const;

        void debugMatrixAccess(const cgp::mat4& viewMatrix, const cgp::mat4& projectionMatrix);

        Chunk* getChunkByIndex(size_t index) const;
        size_t getChunkCount() const;
        void debugFirstChunk() const;

        void debugWorldState() const;
        void checkChunkIntegrity() const;
};