#include "block/tool.hpp"

class Rotten_flesh : public Tool{
    protected:
        int give_hunger;

    public:

    void action ();
    void initialize();
        
};