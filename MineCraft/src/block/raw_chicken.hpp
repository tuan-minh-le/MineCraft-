#include "block/tool.hpp"
#include "entity/player.hpp"

class Raw_chicken : public Tool{
    protected:
        int give_hunger;

    public:

    void action (Player& player);
    void action ();
    void initialize();
    
};