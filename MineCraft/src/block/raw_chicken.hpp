#include "block/tool.hpp"

class Raw_chicken : public Tool{
    protected:
        int give_hunger;

    public:

    void action ();
    void initialize();
    
};