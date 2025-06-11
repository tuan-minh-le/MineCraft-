#include "block/interractive.hpp"

class Craft_table : public Interractive{
    protected:

    public:

    bool opened;

    void action (int* entier);
    Craft_table();

    bool get_opened() const;
    bool& set_opened();
    
};