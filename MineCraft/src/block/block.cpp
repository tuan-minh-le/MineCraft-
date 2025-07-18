#include "block.hpp"

// Static member definitions
cgp::mesh Block::shared_cube_mesh;
bool Block::mesh_initialized = false;
bool Block::instanced_drawables_initialized = false;

std::vector<cgp::vec3> Block::grass_instances;
std::vector<cgp::vec3> Block::stone_instances;
std::vector<cgp::vec3> Block::sand_instances;
std::vector<cgp::vec3> Block::snow_instances;
std::vector<cgp::vec3> Block::ct_instances;
std::vector<cgp::vec3> Block::cactus_instances;
std::vector<cgp::vec3> Block::leaf_instances;
std::vector<cgp::vec3> Block::wood_instances;

cgp::mesh_drawable Block::grass_instanced;
cgp::mesh_drawable Block::stone_instanced;
cgp::mesh_drawable Block::sand_instanced;
cgp::mesh_drawable Block::snow_instanced;
cgp::mesh_drawable Block::ct_instanced;
cgp::mesh_drawable Block::cactus_instanced;
cgp::mesh_drawable Block::leaf_instanced;
cgp::mesh_drawable Block::wood_instanced;


Block::Block() {}

void Block::initialize_shared_mesh() {
    if (!mesh_initialized) {
        shared_cube_mesh = cgp::mesh_primitive_cube({0,0,0}, 1.0f);
        mesh_initialized = true;
    }
}

void Block::initialize_instanced_drawables() {
    if (!instanced_drawables_initialized) {
        initialize_shared_mesh();
        
        // Initialize grass drawable
        grass_instanced.initialize_data_on_gpu(shared_cube_mesh);
        grass_instanced.material.color = cgp::vec3{0.2f, 0.8f, 0.2f}; // Green
        
        // Initialize stone drawable
        stone_instanced.initialize_data_on_gpu(shared_cube_mesh);
        stone_instanced.material.color = cgp::vec3{0.1f, 0.1f, 0.1f}; // Dark gray
        
        // Initialize sand drawable
        sand_instanced.initialize_data_on_gpu(shared_cube_mesh);
        sand_instanced.material.color = cgp::vec3{0.9f, 0.9f, 0.1f}; // Yellow

        snow_instanced.initialize_data_on_gpu(shared_cube_mesh);
        snow_instanced.material.color = cgp::vec3(1.f, 1.f, 1.f);

        ct_instanced.initialize_data_on_gpu(shared_cube_mesh);
        ct_instanced.material.color = cgp::vec3(0.5f, 0.3f, 0.1f);

        cactus_instanced.initialize_data_on_gpu(shared_cube_mesh);
        cactus_instanced.material.color = cgp::vec3{0.2f, 0.6f, 0.2f}; 
        
        // Initialize leaf drawable
        leaf_instanced.initialize_data_on_gpu(shared_cube_mesh);
        leaf_instanced.material.color = cgp::vec3{0.1f, 0.7f, 0.1f}; 
        
        // Initialize wood drawable
        wood_instanced.initialize_data_on_gpu(shared_cube_mesh);
        wood_instanced.material.color = cgp::vec3{0.5f, 0.3f, 0.1f};
        
        instanced_drawables_initialized = true;
    }
}

void Block::clear_all_instances() {
    grass_instances.clear();
    stone_instances.clear();
    sand_instances.clear();
    snow_instances.clear();
    ct_instances.clear();
    cactus_instances.clear();
    leaf_instances.clear();
    wood_instances.clear();

}

BlockType Block::get_type() const{
    return type;
}

BlockType& Block::set_type(){
    return type;
}

void Block::add_grass_instance(const cgp::vec3& position) {
    grass_instances.push_back(position); // Store just the position
}

void Block::add_stone_instance(const cgp::vec3& position) {
    stone_instances.push_back(position); // Store just the position
}

void Block::add_sand_instance(const cgp::vec3& position) {
    sand_instances.push_back(position); // Store just the position
}

void Block::add_snow_instance(const cgp::vec3& position) {
    snow_instances.push_back(position);
}

void Block::add_ct_instance(const cgp::vec3& position){
    ct_instances.push_back(position);
}

void Block::add_cactus_instance(const cgp::vec3& position) {
    cactus_instances.push_back(position);
}

void Block::add_leaf_instance(const cgp::vec3& position) {
    leaf_instances.push_back(position);
}

void Block::add_wood_instance(const cgp::vec3& position) {
    wood_instances.push_back(position);
}

void Block::render_instanced_blocks(cgp::mesh_drawable& drawable, 
                                  const std::vector<cgp::vec3>& positions,  // Changed to vec3
                                  const cgp::environment_generic_structure& environment) {
    if (positions.empty()) return;
    
    // Render each instance by setting translation
    for (const auto& position : positions) {
        drawable.model.translation = position;  // Use translation directly
        cgp::draw(drawable, environment);
    }
}

void Block::render_all_instances(const cgp::environment_generic_structure& environment) {
    if (!instanced_drawables_initialized) {
        initialize_instanced_drawables();
    }
    
    // Debug: Print instance counts
    static bool debug_printed = false;
    if (!debug_printed) {
        std::cout << "Rendering instances - Grass: " << grass_instances.size() 
                  << ", Stone: " << stone_instances.size() 
                  << ", Sand: " << sand_instances.size() << std::endl;
        debug_printed = true;
    }
    
    // Render all grass blocks
    render_instanced_blocks(grass_instanced, grass_instances, environment);
    
    // Render all stone blocks
    render_instanced_blocks(stone_instanced, stone_instances, environment);
    
    // Render all sand blocks
    render_instanced_blocks(sand_instanced, sand_instances, environment);

    render_instanced_blocks(snow_instanced, snow_instances, environment);

    render_instanced_blocks(ct_instanced, ct_instances, environment);

    render_instanced_blocks(cactus_instanced, cactus_instances, environment);

    render_instanced_blocks(leaf_instanced, leaf_instances, environment);
    
    render_instanced_blocks(wood_instanced, wood_instances, environment);
}

void Block::draw_block_at(const cgp::environment_generic_structure& environment) {
    initialize_shared_mesh();
    
    static cgp::mesh_drawable cube_drawable;
    static bool drawable_initialized = false;
    
    if (!drawable_initialized) {
        cube_drawable.initialize_data_on_gpu(shared_cube_mesh);
        drawable_initialized = true;
    }
    
    cube_drawable.model.translation = position;
    cgp::draw(cube_drawable, environment);
}

int Block::getDimension() const {
    return dimension;
}

cgp::vec3 Block::getPosition() const {
    return position;
}

cgp::vec3& Block::setPosition() {
    return position;
}