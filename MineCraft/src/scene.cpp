#include "scene.hpp"
#include "entity/player.hpp"
#include "world/primary_world.hpp"
#include <map> 
#include <memory>


using namespace cgp;


void scene_structure::initialize()
{
	
	player.initialize(inputs,window);
	player.get_camera().look_at({ 3.0f, 2.0f, 2.0f }, {0,0,0}, {0,0,1});
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());

	chunk.initialize();

	world.initialize();
	world.generateChunk(chunk, {0, 0, 0});


	environment.camera_view = player.get_camera().camera_model.matrix_view();
	
	Block::initialize_shared_mesh();
	grass.initialize();
	primary_world.initialize();
	// //primary world initialization


	// std::vector<int> positions = {0, 1, 2};
	// //std::vector<Block*> vectorBlockType;
	// for(int x : positions){
	// 	for(int y : positions){
	// 		for(int z : positions){
	// 			cgp::vec3 position = {x, y, z};
	// 			Grass* grass;
	// 			grass = new Grass();
	// 			grass->setPosition() = position;
	// 			vectorBlockType.push_back(grass);
	// 		}
	// 	}
	// }
	


	glfwSetInputMode(window.glfw_window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);

}


void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	environment.light = player.get_camera().camera_model.position();

	//chunk.render(environment);
	// world.drawChunk(environment);

	for (size_t i = 0; i < primary_world.getVectorBlockType().size(); ++i) {
    	primary_world.getVectorBlockType()[i]->draw_block_at(environment);
		//std::cout<<primary_world.getVectorBlockType()[i]->getPosition()<<std::endl;
	}
	// std::vector<int> positions = {0, 1, 2};
	// std::vector<Block*> vectorBlockType;
	// for(int x : positions){
	// 	for(int y : positions){
	// 		for(int z : positions){
	// 			cgp::vec3 position = {x, y, z};
	// 			Grass* grass;
	// 			grass = new Grass();
	// 			grass->setPosition() = position;
	// 			vectorBlockType.push_back(grass);
	// 			grass->draw_block_at(environment);

	// 		}
	// 	}
	// }
	// std::cout<<vectorBlockType[0]->getPosition()<<std::endl;
	if (gui.display_frame)
		draw(global_frame, environment);

}

void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);
	ImGui::Text("x: %f", player.getPosition().x);
	ImGui::Text("y: %f", player.getPosition().y);
	ImGui::Text("z: %f", player.getPosition().z);
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
	player.handle_keyboard_event(inputs.keyboard,environment.camera_view);
	player.move(player.get_speed(),inputs.keyboard, environment.camera_view);
	player.handle_mouse_event(inputs.mouse);
}

