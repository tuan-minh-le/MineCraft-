#include "block/tool.hpp"

class Raw_chicken : public Tool{
    protected:
        int give_hunger;

    public:

    Raw_chicken();
    void action (int* entier);

    int get_hunger() const;
    int& set_hunger();
    
};