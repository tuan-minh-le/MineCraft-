#include "scene.hpp"
#include "entity/player.hpp"

using namespace cgp;


void scene_structure::initialize()
{
	
	player.initialize(inputs,window);
	player.get_camera().look_at({ 3.0f, 2.0f, 2.0f }, {0,0,0}, {0,0,1});
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());


	environment.camera_view = player.get_camera().camera_model.matrix_view();

	Block::initialize_shared_mesh();

	grass.initialize();

	glfwSetInputMode(window.glfw_window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);

}


void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	environment.light = player.get_camera().camera_model.position();

	std::vector<int> positions = {0, 1, 2};
    
	for(int x : positions){
		for(int y : positions){
			for(int z : positions){
				cgp::vec3 position = {x, y, z};
				Grass::draw_grass_at(position, environment);
			}
		}
	}
	
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

}

void scene_structure::mouse_click_event()
{
	
}
void scene_structure::keyboard_event()
{
	
}
void scene_structure::idle_frame()
{
	player.handle_mouse_move(inputs.mouse.position.current, inputs.mouse.position.previous, environment.camera_view);
	player.handle_keyboard_event(inputs.keyboard);
	player.move(player.get_speed(),inputs.keyboard);
}

