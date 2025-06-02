#include "cgp/cgp.hpp"
#include "item.hpp"

class Block : public Item{
    protected:
        int dimension = 1;
        cgp::vec3 position;
        cgp::mesh_drawable block_texture;
        static cgp::mesh block_mesh;

    public:
        Block();
        Block(const cgp::vec3& position);
        virtual void initialize() = 0;
        int getDimension() const;
        cgp::vec3 getPosition() const;
        cgp::vec3& setPosition();
};