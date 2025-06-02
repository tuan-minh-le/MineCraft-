#include "block.hpp"

bool mesh_initialized = false;

class Grass : public Block{
    private:

    public:
        Grass();
        ~Grass();
        void initialize();

};