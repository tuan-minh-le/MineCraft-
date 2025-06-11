#include "block/craft_table.hpp"

cgp::mesh_drawable Craft_table::craftingTableInstanced;
bool Craft_table::drawable_initialized = false;

Craft_table::Craft_table() : Block() {
    opened = false;
    item_name = "Crafting Table";
    type = BlockType::CRAFT_TABLE;
}

void Craft_table::initialize() {
    initialize_shared_mesh();
    
}

void Craft_table::setup_texture(){

}

void Craft_table::draw_block_at(const cgp::environment_generic_structure& environment) {
    initialize_shared_mesh();
    
    if (!drawable_initialized) {
        craftingTableInstanced.initialize_data_on_gpu(shared_cube_mesh);
        craftingTableInstanced.material.color = cgp::vec3(0.5, 0.3, 0.1);
        drawable_initialized = true;    
    }
    
    craftingTableInstanced.model.translation = position;
    cgp::draw(craftingTableInstanced, environment);
}

void Craft_table::action (int* entier){
    opened = true;
}

bool Craft_table::get_opened() const{
    return opened;
}
bool& Craft_table::set_opened(){
    return opened;
}