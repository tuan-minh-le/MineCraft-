#include "chunk.hpp"

Chunk::Chunk():chunkWorldPosition({0, 0, 0}){}

Chunk::Chunk(const cgp::vec3& worldPos):chunkWorldPosition(worldPos){}

Chunk::~Chunk(){}

void Chunk::initialize(){
    blockData.resize(chunkSize.width * chunkSize.height * chunkSize.depth, AIR);

    grass.initialize();
    stone.initialize();
    sand.initialize();
}

void Chunk::findSurfaceBlocksBFS(std::vector<std::tuple<int, int, int>>& surfaceBlocks) const {
    surfaceBlocks.clear();
    
    int totalBlocks = chunkSize.width * chunkSize.height * chunkSize.depth;
    std::vector<bool> visited(totalBlocks, false);
    std::queue<std::tuple<int, int, int>> queue;
    
   // 26-directional movement (6 faces + 12 edges + 8 corners)
    static const int directions[26][3] = {
        // 6 face neighbors
        {-1, 0, 0}, {1, 0, 0},   // X axis (left, right)
        {0, -1, 0}, {0, 1, 0},   // Y axis (down, up)
        {0, 0, -1}, {0, 0, 1},   // Z axis (back, front)
        
        // 12 edge neighbors
        {-1, -1, 0}, {-1, 1, 0}, {1, -1, 0}, {1, 1, 0},     // XY edges
        {-1, 0, -1}, {-1, 0, 1}, {1, 0, -1}, {1, 0, 1},     // XZ edges  
        {0, -1, -1}, {0, -1, 1}, {0, 1, -1}, {0, 1, 1},     // YZ edges
        
        // 8 corner neighbors
        {-1, -1, -1}, {-1, -1, 1}, {-1, 1, -1}, {-1, 1, 1},
        {1, -1, -1}, {1, -1, 1}, {1, 1, -1}, {1, 1, 1}
    };
    
    
    auto isBlockExposed = [&](int x, int y, int z) -> bool {
        if(getBlock(x, y, z) == AIR){
            return false; 
        }

        for(int face = 0; face < 26; face++) {
            int nx = x + directions[face][0];
            int ny = y + directions[face][1];
            int nz = z + directions[face][2];
            
            if(!isValidCoordinate(nx, ny, nz) || getBlock(nx, ny, nz) == AIR) {
                return true;
            }
        }
        return false; 
    };
    
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
    
    if(!foundStartBlock) {
        std::cout << "No exposed blocks found - chunk is completely buried or empty" << std::endl;
        return;
    }

    while(!queue.empty()) {
        auto [x, y, z] = queue.front();
        queue.pop();
        
        for(int dir = 0; dir < 26; dir++) {
            int nx = x + directions[dir][0];
            int ny = y + directions[dir][1];
            int nz = z + directions[dir][2];
            
            if(isValidCoordinate(nx, ny, nz)) {
                int neighborIndex = coordinateToIndex(nx, ny, nz);
                
                // Only process if not visited yet
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
    
    std::cout << "Found " << surfaceBlocks.size() << " crust blocks using surface-walking BFS" << std::endl;
}


void Chunk::setWorldPosition(const cgp::vec3& position){
    chunkWorldPosition = position;
}

void Chunk::setBlock(int x, int y , int z, BlockType blockType){
    if (!isValidCoordinate(x, y, z)) {
        std::cerr << "Invalid coordinates: " << x << ", " << y << ", " << z << std::endl;
        return;
    }
    int index = coordinateToIndex(x, y, z);
    blockData[index] = blockType;
}

BlockType Chunk::getBlock(int x, int y, int z) const{
    if(!isValidCoordinate(x, y, z)){
        return AIR;
    }

    int index = coordinateToIndex(x, y, z);
    return blockData[index];
}

bool Chunk::isBlockSolid(int x, int y, int z) const{
    return getBlock(x, y, z) != AIR;
}


void Chunk::render(const cgp::environment_generic_structure& environment){
    if(!surfaceBlocksCached) {
        cachedSurfaceBlocks.clear();
        findSurfaceBlocksBFS(cachedSurfaceBlocks);
        surfaceBlocksCached = true;
    }
    
    for(const auto& [x, y, z] : cachedSurfaceBlocks) {
        BlockType block = getBlock(x, y, z);
        if(block != AIR) {
            drawBlockAt(x, y, z, block, environment);
        }
    }
}

void Chunk::drawBlockAt(int x, int y, int z, BlockType block, const cgp::environment_generic_structure& environment){
        cgp::vec3 worldPos = localToWorld({
        static_cast<float>(x), 
        static_cast<float>(y), 
        static_cast<float>(z)
    });
    
    switch(block) {
        case GRASS:
            grass.setPosition() = worldPos;
            grass.draw_block_at(environment);
            // std::cout << "Drew grass at: " << worldPos << std::endl;
            break;
        case DIRT:
            grass.setPosition() = worldPos;
            grass.draw_block_at(environment);
            // std::cout << "Drew dirt at: " << worldPos << std::endl;
            break;
        case SAND:
            sand.setPosition() = worldPos;
            sand.draw_block_at(environment);
            // std::cout << "Drew sand at: " << worldPos << std::endl;
            break;
        case STONE:
            stone.setPosition() = worldPos;
            stone.draw_block_at(environment);
            // std::cout << "Drew stone at: " << worldPos << std::endl;
            break;
        case BEDROCK:
            stone.setPosition() = worldPos;
            stone.draw_block_at(environment);
            // std::cout << "Drew bedrock at: " << worldPos << std::endl;
            break;
        case AIR:
            break;
    }
}

cgp::vec3 Chunk::localToWorld(const cgp::vec3& localPos) const {
    return chunkWorldPosition + localPos;
}

cgp::vec3 Chunk::worldToLocal(const cgp::vec3& worldPos) const {
    return worldPos - chunkWorldPosition;
}

bool Chunk::isValidCoordinate(int x, int y, int z) const{
    return x >= 0 && x < chunkSize.width &&
        y >= 0 && y < chunkSize.height &&
        z >= 0 && z < chunkSize.depth;
}

int Chunk::coordinateToIndex(int x, int y, int z) const {
    return z * (chunkSize.width * chunkSize.height) + y * chunkSize.width + x;
}