#pragma once
#include "cgp/cgp.hpp"
#include "item.hpp"
#include <vector>

enum BlockType {
    AIR = 0,
    GRASS = 1,
    DIRT = 2,
    SAND = 3,
    STONE = 4,
    BEDROCK = 5
};

class Block : public Item {
protected:
    int dimension = 1;
    BlockType type;
    
    static cgp::mesh shared_cube_mesh;
    static bool mesh_initialized;
    cgp::vec3 position;

    // Static instancing data for each block type - simplified to just positions
    static std::vector<cgp::vec3> grass_instances;
    static std::vector<cgp::vec3> stone_instances;
    static std::vector<cgp::vec3> sand_instances;
    
    // Static mesh drawables for instanced rendering
    static cgp::mesh_drawable grass_instanced;
    static cgp::mesh_drawable stone_instanced;
    static cgp::mesh_drawable sand_instanced;
    static bool instanced_drawables_initialized;

public:
    Block(); 
    virtual ~Block() = default;
    
    virtual void initialize() = 0;
    
    static void initialize_shared_mesh();
    static void initialize_instanced_drawables();
    
    // New methods for instanced rendering
    static void clear_all_instances();
    static void add_grass_instance(const cgp::vec3& position);
    static void add_stone_instance(const cgp::vec3& position);
    static void add_sand_instance(const cgp::vec3& position);
    static void render_all_instances(const cgp::environment_generic_structure& environment);
    
    // Keep original method for compatibility
    virtual void draw_block_at(const cgp::environment_generic_structure& environment);
    
    virtual void setup_texture() = 0;

    BlockType get_type() const;
    BlockType& set_type();
    
    // Getter Setter
    int getDimension() const;
    cgp::vec3 getPosition() const;
    cgp::vec3& setPosition();

private:
    static void render_instanced_blocks(cgp::mesh_drawable& drawable, 
                                      const std::vector<cgp::vec3>& positions,  // Changed to vec3
                                      const cgp::environment_generic_structure& environment);
};