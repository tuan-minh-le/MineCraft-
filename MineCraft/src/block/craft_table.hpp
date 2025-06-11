#include "block/interractive.hpp"

class Craft_table : public Block {
private:
    bool opened;
    static cgp::mesh_drawable craftingTableInstanced;
    static cgp::image_structure craftingTableTexture;
    static bool drawable_initialized;

public:
    Craft_table();
    ~Craft_table() = default;
    
    void initialize() override;
    void setup_texture() override;
    void draw_block_at(const cgp::environment_generic_structure& environment) override;
    
    void action(int* entier);
    bool get_opened() const;
    bool& set_opened();
};