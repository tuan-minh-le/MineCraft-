#include "chunk.hpp"
#include "world.hpp"

Chunk::Chunk():chunkWorldPosition({0, 0, 0}){}

Chunk::Chunk(const cgp::vec3& worldPos):chunkWorldPosition(worldPos){}

Chunk::~Chunk(){
    cleanupBlockObjects();
}

void Chunk::initialize(){
    //std::cout << "Initializing chunk at position: (" << chunkWorldPosition.x << ", " << chunkWorldPosition.y << ", " << chunkWorldPosition.z << ")" << std::endl;
    // std::cout << "Chunk size: " << chunkSize.width << "x" << chunkSize.height << "x" << chunkSize.depth << std::endl;
    
    int totalBlocks = chunkSize.width * chunkSize.height * chunkSize.depth;
    // std::cout << "Total block slots: " << totalBlocks << std::endl;
    
    blockData.resize(totalBlocks, AIR);
    blockObjects.resize(totalBlocks, nullptr);

    grass.initialize();
    stone.initialize();
    sand.initialize();

    surfaceBlocksCached = false;
    
    // std::cout << "Chunk initialized with " << blockData.size() << " block data slots" << std::endl;
}

void Chunk::setWorld(World* worldptr){
    world = worldptr;
}

bool Chunk::isBlockExposed(int x, int y, int z) const{
    BlockType currentBlock = getBlock(x, y, z);

    if(currentBlock == AIR){
        return false;
    }

    static const int directions [6][3] = {
        {1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, 
        {0, -1, 0}, {0, 0, 1}, {0, 0, -1}
    };

    for(int i = 0; i < 6; i++){
        int nx = x + directions[i][0];
        int ny = y + directions[i][1];
        int nz = z + directions[i][2];

        BlockType neighborBlock;

        // If neighbor is within this chunk bounds
        if(isValidCoordinate(nx, ny, nz)){
            neighborBlock = getBlock(nx, ny, nz);
        }
        // If neighbor is outside chunk, check world (cross-chunk)
        else{
            if(world != nullptr){
                cgp::vec3 currentWorldPos = localToWorld({
                    static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)
                });

                cgp::vec3 neighborWorldPos = currentWorldPos + cgp::vec3(
                    static_cast<float>(directions[i][0]),
                    static_cast<float>(directions[i][1]),
                    static_cast<float>(directions[i][2])
                );

                neighborBlock = world->getBlock(neighborWorldPos);
            }
            else{
                neighborBlock = AIR;
            }
        }
        
        // If any neighbor is air, this block is exposed
        if(neighborBlock == AIR){
            return true;
        }
    }
    return false;
}

Block* Chunk::createBlockObject(BlockType type, const cgp::vec3& position) {
    Block* block = nullptr;
    
    switch(type) {
        case GRASS:
            block = new Grass();
            break;
        case DIRT:
            block = new Grass(); 
            break;
        case SAND:
            block = new Sand();
            break;
        case STONE:
            block = new Stone();
            break;
        case SNOW:
            block = new Snow();
            break;
        case BEDROCK:
            block = new Stone(); 
            break;
        case AIR:
        default:
            return nullptr; 
    }
    
    if(block) {
        block->initialize();
        block->setPosition() = position;
    }
    
    return block;
}

void Chunk::cleanupBlockObjects() {
    for(Block* block : blockObjects) {
        delete block;
    }
    blockObjects.clear();
}

void Chunk::populateBlockObjects() {
    // std::cout << "Populating Block objects..." << std::endl;
    int cpt = 0;
    int objectsCreated = 0;
    for(int x = 0; x < chunkSize.width; x++) {
        for(int y = 0; y < chunkSize.height; y++) {
            for(int z = 0; z < chunkSize.depth; z++) {
                int index = coordinateToIndex(x, y, z);
                BlockType blockType = blockData[index];
                
                if(blockType != AIR) {
                    cgp::vec3 worldPos = localToWorld({
                        static_cast<float>(x), 
                        static_cast<float>(y), 
                        static_cast<float>(z)
                    });
                    //std::cout << "Creating block at: " << worldPos << std::endl;
                    // Create Block object
                    blockObjects[index] = createBlockObject(blockType, worldPos);
                    cpt+=1;
                    if(blockObjects[index] != nullptr) {
                        objectsCreated++;
                    }
                }
            }
        }
    }
    setBlockObjectListsize() = cpt;
    // std::cout << "Created " << objectsCreated << " Block objects" << std::endl;
}

void Chunk::updateBlockObject(int x, int y, int z, BlockType newType) {
    int index = coordinateToIndex(x, y, z);
    cgp::vec3 worldPos = localToWorld({static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)});
    
    // delete blockObjects[index];
    
    blockObjects[index] = createBlockObject(newType, worldPos);

}

Block* Chunk::getBlockObject(int x, int y, int z) const {
    if(!isValidCoordinate(x, y, z)) {
        return nullptr;
    }
    
    int index = coordinateToIndex(x, y, z);
    return blockObjects[index];
}

std::vector<Block*> Chunk::getBlockObjectList() const{
    return blockObjects;
}

int Chunk::getBlockObjectListsize() const{
   return nbBlockInChunk;
}

int& Chunk::setBlockObjectListsize(){
   return nbBlockInChunk;
}



// std::vector<std::tuple<int, int, int>> Chunk::findSurfaceBlocksBFS() const{
//     std::vector<std::tuple<int, int, int>> ret;
//     int totalBlocks = chunkSize.width * chunkSize.height * chunkSize.depth;
//     std::vector<bool> visited(totalBlocks, false);
//     std::queue<std::tuple<int, int, int>> queue;
    
//    // 26-directional movement (6 faces + 12 edges + 8 corners)
//     static const int directions[26][3] = {
//         // 6 face neighbors
//         {-1, 0, 0}, {1, 0, 0},   // X axis (left, right)
//         {0, -1, 0}, {0, 1, 0},   // Y axis (down, up)
//         {0, 0, -1}, {0, 0, 1},   // Z axis (back, front)
        
//         // 12 edge neighbors
//         {-1, -1, 0}, {-1, 1, 0}, {1, -1, 0}, {1, 1, 0},     // XY edges
//         {-1, 0, -1}, {-1, 0, 1}, {1, 0, -1}, {1, 0, 1},     // XZ edges  
//         {0, -1, -1}, {0, -1, 1}, {0, 1, -1}, {0, 1, 1},     // YZ edges
        
//         // 8 corner neighbors
//         {-1, -1, -1}, {-1, -1, 1}, {-1, 1, -1}, {-1, 1, 1},
//         {1, -1, -1}, {1, -1, 1}, {1, 1, -1}, {1, 1, 1}
//     };
    
//     // Fixed: Use only 6 face directions for exposure check (standard Minecraft rule)
//     auto isBlockExposed = [&](int x, int y, int z) -> bool {
//         if(getBlock(x, y, z) == AIR){
//             return false; 
//         }

//         for(int face = 0; face < 6; face++) {  // Only check 6 faces for exposure
//             int nx = x + directions[face][0];
//             int ny = y + directions[face][1];
//             int nz = z + directions[face][2];
            
//             if(!isValidCoordinate(nx, ny, nz) || getBlock(nx, ny, nz) == AIR) {
//                 return true;
//             }
//         }
//         return false; 
//     };
    
//     bool foundStartBlock = false;
//     for(int x = 0; x < chunkSize.width && !foundStartBlock; x++) {
//         for(int y = 0; y < chunkSize.height && !foundStartBlock; y++) {
//             for(int z = 0; z < chunkSize.depth && !foundStartBlock; z++) {
//                 if(isBlockExposed(x, y, z)) {
//                     queue.push({x, y, z});
//                     int index = coordinateToIndex(x, y, z);
//                     visited[index] = true;
//                     ret.push_back({x, y, z});
//                     foundStartBlock = true;
//                 }
//             }
//         }
//     }
    
//     if(!foundStartBlock) {
//         std::cout << "No exposed blocks found - chunk is completely buried or empty" << std::endl;
//     }

//     while(!queue.empty()) {
//         auto [x, y, z] = queue.front();
//         queue.pop();
        
//         for(int dir = 0; dir < 26; dir++) {  
//             int nx = x + directions[dir][0];
//             int ny = y + directions[dir][1];
//             int nz = z + directions[dir][2];
            
//             if(isValidCoordinate(nx, ny, nz)) {
//                 int neighborIndex = coordinateToIndex(nx, ny, nz);
//                 if(!visited[neighborIndex]) {
//                     if(isBlockExposed(nx, ny, nz)) {
//                         visited[neighborIndex] = true;
//                         queue.push({nx, ny, nz});
//                         ret.push_back({nx, ny, nz});
//                     }
//                 }
//             }
//         }
//     }
//     return ret;
// }

void Chunk::findSurfaceBlocksBFS(std::vector<std::tuple<int, int, int>>& surfaceBlocks) const {
    surfaceBlocks.clear();
    
    // Check if this chunk is on the border of the world
    bool isBorderChunk = false;
    if(world != nullptr) {
        cgp::vec3 chunkPos = getWorldPosition();
        ChunkSize size = getSize();
        
        // Check if there are adjacent chunks in all 4 horizontal directions
        cgp::vec3 neighbors[4] = {
            {chunkPos.x + size.width, chunkPos.y, chunkPos.z},  // Right
            {chunkPos.x - size.width, chunkPos.y, chunkPos.z},  // Left  
            {chunkPos.x, chunkPos.y, chunkPos.z + size.depth},  // Forward
            {chunkPos.x, chunkPos.y, chunkPos.z - size.depth}   // Backward
        };
        
        for(int i = 0; i < 4; i++) {
            if(world->getChunkAt(neighbors[i]) == nullptr) {
                isBorderChunk = true;
                break;
            }
        }
    } else {
        isBorderChunk = true;
    }
    
    // Use BFS for both border and interior chunks, but with different starting strategies
    int totalBlocks = chunkSize.width * chunkSize.height * chunkSize.depth;
    std::vector<bool> visited(totalBlocks, false);
    std::queue<std::tuple<int, int, int>> queue;
    
    static const int directions[26][3] = {
        {-1, 0, 0}, {1, 0, 0}, {0, -1, 0}, {0, 1, 0}, {0, 0, -1}, {0, 0, 1},
        {-1, -1, 0}, {-1, 1, 0}, {1, -1, 0}, {1, 1, 0},
        {-1, 0, -1}, {-1, 0, 1}, {1, 0, -1}, {1, 0, 1},
        {0, -1, -1}, {0, -1, 1}, {0, 1, -1}, {0, 1, 1},
        {-1, -1, -1}, {-1, -1, 1}, {-1, 1, -1}, {-1, 1, 1},
        {1, -1, -1}, {1, -1, 1}, {1, 1, -1}, {1, 1, 1}
    };

    if(isBorderChunk) {
        // For border chunks, find any exposed block as starting point
        bool foundStartBlock = false;
        for(int x = 0; x < chunkSize.width && !foundStartBlock; x++) {
            for(int y = 0; y < chunkSize.height && !foundStartBlock; y++) {
                for(int z = 0; z < chunkSize.depth && !foundStartBlock; z++) {
                    if(isBlockExposed(x, y, z)) {
                        queue.push({x, y, z});
                        int index = coordinateToIndex(x, y, z);
                        visited[index] = true;
                        surfaceBlocks.push_back({x, y, z});
                        foundStartBlock = true;
                    }
                }
            }
        }
    } else {
        // For interior chunks, start BFS from the top surface only
        // This finds the top surface and any connected internal surfaces (caves, holes)
        for(int x = 0; x < chunkSize.width; x++) {
            for(int z = 0; z < chunkSize.depth; z++) {
                // Find topmost solid block in this column as starting point
                for(int y = chunkSize.height - 1; y >= 0; y--) {
                    if(getBlock(x, y, z) != AIR) {
                        // Check if this block is exposed (top surface or has air above)
                        if(y == chunkSize.height - 1 || getBlock(x, y + 1, z) == AIR) {
                            int index = coordinateToIndex(x, y, z);
                            if(!visited[index] && isBlockExposed(x, y, z)) {
                                queue.push({x, y, z});
                                visited[index] = true;
                                surfaceBlocks.push_back({x, y, z});
                            }
                        }
                        break; // Found the topmost block in this column
                    }
                }
            }
        }
    }

    // BFS to find all connected exposed blocks
    while(!queue.empty()) {
        auto [x, y, z] = queue.front();
        queue.pop();
        
        for(int dir = 0; dir < 26; dir++) {
            int nx = x + directions[dir][0];
            int ny = y + directions[dir][1];
            int nz = z + directions[dir][2];
            
            if(isValidCoordinate(nx, ny, nz)) {
                int neighborIndex = coordinateToIndex(nx, ny, nz);
                if(!visited[neighborIndex]) {
                    if(isBlockExposed(nx, ny, nz)) {
                        visited[neighborIndex] = true;
                        queue.push({nx, ny, nz});
                        surfaceBlocks.push_back({nx, ny, nz});
                    }
                }
            }
        }
    }
}

void Chunk::setWorldPosition(const cgp::vec3& position){
    chunkWorldPosition = position;
    surfaceBlocksCached = false;
}

void Chunk::setBlock(int x, int y, int z, BlockType blockType){
    if (!isValidCoordinate(x, y, z)) {
        // std::cerr << "Invalid coordinates for setBlock: " << x << ", " << y << ", " << z << std::endl;
        // std::cerr << "Valid range: [0-" << (chunkSize.width-1) << ", 0-" << (chunkSize.height-1) << ", 0-" << (chunkSize.depth-1) << "]" << std::endl;
        return;
    }
    
    int index = coordinateToIndex(x, y, z);
    if(index < 0 || index >= blockData.size()) {
        // std::cerr << "Invalid index " << index << " for blockData size " << blockData.size() << std::endl;
        return;
    }
    blockData[index] = blockType;
    surfaceBlocksCached = false;

    
    // Debug output for first few blocks set
    static int blocksSet = 0;
    if(blocksSet < 10 && blockType != AIR) {
        // std::cout << "Set block " << blocksSet << ": type " << static_cast<int>(blockType) 
        //         << " at local(" << x << "," << y << "," << z << ") index " << index << std::endl;
        blocksSet++;
    }
    updateBlockObject(x, y, z, blockType);
}

BlockType Chunk::getBlock(int x, int y, int z) const{
    if(!isValidCoordinate(x, y, z)){
        // std::cout << "getBlock: Invalid coordinates (" << x << ", " << y << ", " << z << ")" << std::endl;
        // std::cout << "Valid range: [0-" << (chunkSize.width-1) << ", 0-" << (chunkSize.height-1) << ", 0-" << (chunkSize.depth-1) << "]" << std::endl;
        return AIR;
    }

    int index = coordinateToIndex(x, y, z);
    if(index < 0 || index >= blockData.size()) {
        // std::cout << "getBlock: Invalid index " << index << " for size " << blockData.size() << std::endl;
        return AIR;
    }
    
    BlockType result = blockData[index];
    
    // Debug output for first few successful gets
    static int getsCount = 0;
    if(getsCount < 5) {
        // std::cout << "getBlock " << getsCount << ": coords(" << x << "," << y << "," << z 
        //          << ") index(" << index << ") = " << static_cast<int>(result) << std::endl;
        getsCount++;
    }
    
    return result;
}

bool Chunk::isBlockSolid(int x, int y, int z) const{
    return getBlock(x, y, z) != AIR;
}

void Chunk::renderCached(const cgp::environment_generic_structure& environment){
    if(!surfaceBlocksCached) {
        cachedSurfaceBlocks.clear();
        findSurfaceBlocksBFS(cachedSurfaceBlocks);
        surfaceBlocksCached = true;
    }
    
    for(const auto& [x, y, z] : cachedSurfaceBlocks) {
        Block* blockObj = getBlockObject(x, y, z);
        if(blockObj) {
            blockObj->draw_block_at(environment);
        }
    }
}

void Chunk::drawBlockAt(int x, int y, int z, BlockType block, const cgp::environment_generic_structure& environment){
    Block* blockObj = getBlockObject(x, y, z);
    if(blockObj) {
        blockObj->draw_block_at(environment);
    }
}

cgp::vec3 Chunk::localToWorld(const cgp::vec3& localPos) const {
    return chunkWorldPosition + localPos;
}

cgp::vec3 Chunk::worldToLocal(const cgp::vec3& worldPos) const {
    cgp::vec3 localPos = worldPos - chunkWorldPosition;
    
    // Debug output
    static int conversionCount = 0;
    if(conversionCount < 5) {
        // std::cout << "worldToLocal " << conversionCount << ": world(" << worldPos.x << "," << worldPos.y << "," << worldPos.z 
        //           << ") - chunk(" << chunkWorldPosition.x << "," << chunkWorldPosition.y << "," << chunkWorldPosition.z
        //           << ") = local(" << localPos.x << "," << localPos.y << "," << localPos.z << ")" << std::endl;
        conversionCount++;
    }
    
    return localPos;
}

bool Chunk::isValidCoordinate(int x, int y, int z) const{
    return x >= 0 && x < chunkSize.width &&
        y >= 0 && y < chunkSize.height &&
        z >= 0 && z < chunkSize.depth;
}

int Chunk::coordinateToIndex(int x, int y, int z) const {
    return z * (chunkSize.width * chunkSize.height) + y * chunkSize.width + x;
}

cgp::vec3 Chunk::getChunkCenter() const{
    return chunkWorldPosition + cgp::vec3({chunkSize.width / 2.0f, 
                                        chunkSize.height / 2.0f, 
                                        chunkSize.depth / 2.0f});
}

// Debug methods
void Chunk::debugChunkContents() const {
    // std::cout << "\n=== CHUNK CONTENTS DEBUG ===" << std::endl;
    // std::cout << "Chunk at world position: (" << chunkWorldPosition.x << ", " << chunkWorldPosition.y << ", " << chunkWorldPosition.z << ")" << std::endl;
    
    int airCount = 0;
    int solidCount = 0;
    std::map<BlockType, int> blockCounts;
    
    for(int x = 0; x < chunkSize.width; x++) {
        for(int y = 0; y < chunkSize.height; y++) {
            for(int z = 0; z < chunkSize.depth; z++) {
                BlockType block = getBlock(x, y, z);
                blockCounts[block]++;
                
                if(block == AIR) {
                    airCount++;
                } else {
                    solidCount++;
                    // Print first few solid blocks found
                    if(solidCount <= 10) {
                        // std::cout << "Solid block " << solidCount << ": type " << static_cast<int>(block) 
                        //           << " at (" << x << "," << y << "," << z << ")" << std::endl;
                    }
                }
            }
        }
    }
    
    // std::cout << "Block type counts:" << std::endl;
    // for(const auto& [type, count] : blockCounts) {
    //     std::cout << "  Type " << static_cast<int>(type) << ": " << count << " blocks" << std::endl;
    // }
    
    // std::cout << "Total: " << airCount << " air, " << solidCount << " solid blocks" << std::endl;
    // std::cout << "=========================" << std::endl;
}

void Chunk::renderInstanced(const cgp::environment_generic_structure& environment) {
    if (!surfaceBlocksCached) {
        cachedSurfaceBlocks.clear();
        findSurfaceBlocksBFS(cachedSurfaceBlocks);
        surfaceBlocksCached = true;
    }
    
    // Clear previous frame's instances
    Block::clear_all_instances();
    
    // Collect all visible blocks by type
    for (const auto& [x, y, z] : cachedSurfaceBlocks) {
        BlockType blockType = getBlock(x, y, z);
        cgp::vec3 worldPos = localToWorld({
            static_cast<float>(x), 
            static_cast<float>(y), 
            static_cast<float>(z)
        });
        
        // Add to appropriate instance list based on block type
        switch (blockType) {
            case GRASS:
                Block::add_grass_instance(worldPos);
                break;
            case STONE:
                Block::add_stone_instance(worldPos);
                break;
            case SAND:
                Block::add_sand_instance(worldPos);
                break;
            default:
                // For other block types, you can add more instance lists
                // or fall back to individual rendering
                break;
        }
    }
    
    // Render all instances at once
    Block::render_all_instances(environment);
}