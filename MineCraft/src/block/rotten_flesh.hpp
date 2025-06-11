#include "block/tool.hpp"

class Rotten_flesh : public Tool{
    protected:
        int give_hunger;

    public:

    void action (int* entier);
    Rotten_flesh();

    int get_hunger() const;
    int& set_hunger();
        
};