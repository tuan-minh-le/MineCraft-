#include "scene.hpp"


using namespace cgp;


void scene_structure::initialize()
{
	player.initialize(inputs,window);
	// camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	// camera_control.set_rotation_axis_y();
	// camera_control.look_at({ 3.0f, 2.0f, 2.0f }, {0,0,0}, {0,0,1});
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());

}


void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	environment.light = player.get_camera().camera_model.position();
	
	if (gui.display_frame)
		draw(global_frame, environment);

}

void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);

}

void scene_structure::mouse_move_event()
{
	if (!inputs.keyboard.shift)
		player.get_camera().action_mouse_move(environment.camera_view);
	player.handle_mouse_move(inputs.mouse.position.current, inputs.mouse.position.previous, environment.camera_view);
}
void scene_structure::mouse_click_event()
{
	player.get_camera().action_mouse_click(environment.camera_view);
}
void scene_structure::keyboard_event()
{
	player.get_camera().action_keyboard(environment.camera_view);
}
void scene_structure::idle_frame()
{
	player.get_camera().idle_frame(environment.camera_view);
}

