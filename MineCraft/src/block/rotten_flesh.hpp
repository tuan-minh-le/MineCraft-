#include "block/tool.hpp"
#include "entity/player.hpp"

class Rotten_flesh : public Tool{
    protected:
        int give_hunger;

    public:

    void action (Player& player);
    void action ();
    void initialize();
        
};