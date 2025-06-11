#include "block/interractive.hpp"

class Craft_table : public Interractive{
    protected:

    public:

    bool opened;

    void action ();
    void initialize();

    bool get_opened() const;
    bool& set_opened();
    
};