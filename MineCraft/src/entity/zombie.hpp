#include "cgp/cgp.hpp"
#include "mobHostile.hpp"
#include "world/world.hpp"

class Zombie : public MobHostile{
    protected:

    World* world;

    public:
        void move(Player player, float speed);
        void initialize(cgp::input_devices& inputs, cgp::window_structure& window, World* wrd);
};