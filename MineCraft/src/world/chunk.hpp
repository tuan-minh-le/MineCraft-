#pragma once
#include "cgp/cgp.hpp"
#include "block/grass.hpp"
#include "block/sand.hpp" 
#include "block/stone.hpp"
#include <unordered_set>

enum BlockType {
    AIR = 0,
    GRASS = 1,
    DIRT = 2,
    SAND = 3,
    STONE = 4,
    BEDROCK = 5
};

struct ChunkSize {
    int width;
    int height; 
    int depth;
};

class Chunk {
private:
    ChunkSize chunkSize = {16, 16, 16};
    cgp::vec3 chunkWorldPosition; 
    std::vector<BlockType> blockData;
    std::vector<Block*> blockObjects;


    // Block instances for rendering
    Grass grass;
    Stone stone;
    Sand sand;
    
    bool isDataGenerated = false;

    std::vector<std::tuple<int, int, int>> cachedSurfaceBlocks;
    bool surfaceBlocksCached = false;

    bool isValidCoordinate(int x, int y, int z) const;
    int coordinateToIndex(int x, int y, int z) const;

    Block* createBlockObject(BlockType type, const cgp::vec3& position);
    void updateBlockObject(int x, int y, int z, BlockType newType);
    void cleanupBlockObjects(); // For destructor


    
public:
    Chunk();
    Chunk(const cgp::vec3& worldPos);
    ~Chunk();
    
    void initialize();
    void setWorldPosition(const cgp::vec3& worldPos);
    
    BlockType getBlock(int x, int y, int z) const;
    void setBlock(int x, int y, int z, BlockType blockType);
    bool isBlockSolid(int x, int y, int z) const;
    
    cgp::vec3 localToWorld(const cgp::vec3& localPos) const;
    cgp::vec3 worldToLocal(const cgp::vec3& worldPos) const;
    cgp::vec3 getWorldPosition() const { return chunkWorldPosition; }
    ChunkSize getSize() const { return chunkSize; }

    // Get Block object for collision detection
    Block* getBlockObject(int x, int y, int z) const;
    
    bool isGenerated() const { return isDataGenerated; }
    void markAsGenerated() { isDataGenerated = true; }
    
    void render(const cgp::environment_generic_structure& environment);
    
    void findSurfaceBlocksBFS(std::vector<std::tuple<int, int, int>>& surfaceBlocks) const;

    void drawBlockAt(int x, int y, int z, BlockType block, const cgp::environment_generic_structure& environment);

    void populateBlockObjects();

    cgp::vec3 getChunkCenter() const;
    
};