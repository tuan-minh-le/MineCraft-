#include "cgp/cgp.hpp"
#include "mobPassif.hpp"

class Chicken : public MobPassif{
    protected:


    public:
        void move(float speed,const cgp::inputs_keyboard_parameters& keyboard,cgp::mat4& camera_view_matrix);
        void initialize(cgp::input_devices& inputs, cgp::window_structure& window, World& wrd);

};