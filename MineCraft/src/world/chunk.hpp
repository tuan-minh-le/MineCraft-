#pragma once
#include "cgp/cgp.hpp"
#include "block/grass.hpp"
#include "block/sand.hpp" 
#include "block/stone.hpp"
#include "block/block.hpp"
#include "block/snow.hpp"
#include "block/craft_table.hpp"
#include <queue>

class World;

struct ChunkSize {
    int width;
    int height; 
    int depth;
};

class Chunk {
private:
    ChunkSize chunkSize = {16, 64, 16};
    cgp::vec3 chunkWorldPosition; 
    std::vector<BlockType> blockData;
    std::vector<Block*> blockObjects;
    int nbBlockInChunk;

    // Block instances for rendering
    Grass grass;
    Stone stone;
    Sand sand;
    Snow snow;
    Craft_table craftingTable;

    bool isDataGenerated = false;

    World* world = nullptr;

    bool isValidCoordinate(int x, int y, int z) const;
    int coordinateToIndex(int x, int y, int z) const;

    Block* createBlockObject(BlockType type, const cgp::vec3& position);
    void updateBlockObject(int x, int y, int z, BlockType newType);
    void cleanupBlockObjects(); // For destructor

    bool isBlockExposed(int x, int y, int z) const;

    
public:
    Chunk();
    Chunk(const cgp::vec3& worldPos);
    ~Chunk();

    std::vector<std::tuple<int, int, int>> cachedSurfaceBlocks;
    bool surfaceBlocksCached = false;
    
    void initialize();
    void setWorld(World* worldptr);
    void setWorldPosition(const cgp::vec3& worldPos);
    
    BlockType getBlock(int x, int y, int z) const;
    BlockType getBlock();
    void setBlock(int x, int y, int z, BlockType blockType);
    bool isBlockSolid(int x, int y, int z) const;
    
    cgp::vec3 localToWorld(const cgp::vec3& localPos) const;
    cgp::vec3 worldToLocal(const cgp::vec3& worldPos) const;
    cgp::vec3 getWorldPosition() const { return chunkWorldPosition; }
    ChunkSize getSize() const { return chunkSize; }

    // Get Block object for collision detection
    Block* getBlockObject(int x, int y, int z) const;
    std::vector<Block*> getBlockObjectList()const;
    int getBlockObjectListsize() const;
    int& setBlockObjectListsize();
    

    
    bool isGenerated() const { return isDataGenerated; }
    void markAsGenerated() { isDataGenerated = true; }

    void renderBasic(const cgp::environment_generic_structure& environment);
    
    void renderCached(const cgp::environment_generic_structure& environment);

    void renderInstanced(const cgp::environment_generic_structure& environment);

    
    void findSurfaceBlocksBFS(std::vector<std::tuple<int, int, int>>& surfaceBlocks) const;

    void drawBlockAt(int x, int y, int z, BlockType block, const cgp::environment_generic_structure& environment);

    void populateBlockObjects();

    cgp::vec3 getChunkCenter() const;

    void debugChunkContents() const;
    void testCoordinate(int x, int y, int z) const;
    
};