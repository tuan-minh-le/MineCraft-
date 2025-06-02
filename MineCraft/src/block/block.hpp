#include "cgp/cgp.hpp"
#include "item.hpp"

class Block : public Item{
    protected:
        int dimension = 1;
        cgp::vec3 position;
        cgp::mesh_drawable block_texture;

    public:
        virtual void initialize() = 0;
        int getDimension() const;
        cgp::vec3 getPosition() const;
        cgp::vec3& setPosition();
};