#include "cgp/cgp.hpp"
#include "mobPassif.hpp"
#include "world/world.hpp"

class Chicken : public MobPassif{
    protected:
        World* world;

    public:
        void move(Player& player, float speed);
        void initialize(const cgp::vec3& p_position, cgp::input_devices& inputs, cgp::window_structure& window, World* wrd);
        World* getWorld() {return world;};

};