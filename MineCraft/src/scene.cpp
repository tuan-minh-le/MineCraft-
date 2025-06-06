#include "scene.hpp"
#include "entity/player.hpp"
#include "world/primary_world.hpp"
#include <map> 
#include <memory>
#include "stb_image.h"


using namespace cgp;


void scene_structure::initialize()
{
	player.get_camera().look_at({ 3.0f, 2.0f, 2.0f }, {0,0,0}, {0,0,1});
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());


	world.initialize(1, 1, 20);
	player.initialize(inputs,window,world);


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

	world.render(player.getPosition(), environment);
	// world.drawChunk(environment);

	// for (size_t i = 0; i < primary_world.getVectorBlockType().size(); ++i) {
    // 	primary_world.getVectorBlockType()[i]->draw_block_at(environment);
	// 	//std::cout<<primary_world.getVectorBlockType()[i]->getPosition()<<std::endl;
	// }
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

GLuint LoadTextureFromFile(const char* filename)
{
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 4);
    if (!data) {
        std::cerr << "Erreur lors du chargement de l'image : " << filename << std::endl;
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    return textureID;
}


void scene_structure::display_inventory_ui()
{
	if (player.get_inventory().get_opened_inventory()){
		ImGui::SetNextWindowPos(ImVec2(700, 250));
		ImGui::SetNextWindowSize(ImVec2(700, 600));
		ImGui::Begin("full_inventory", nullptr,
					ImGuiWindowFlags_NoTitleBar |
					ImGuiWindowFlags_NoResize |
					ImGuiWindowFlags_NoMove |
					ImGuiWindowFlags_NoCollapse);

		GLuint steve_tex = LoadTextureFromFile("assets/Minecraft_steve/steve_skin.png");
		ImTextureID steve = (ImTextureID)(intptr_t)steve_tex;

		ImVec2 winPos = ImGui::GetWindowPos();
		ImVec2 winSize = ImGui::GetWindowSize();
		auto* drawList = ImGui::GetWindowDrawList();

		drawList->AddRectFilled(winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y),
								IM_COL32(255, 255, 255, 255));

		ImVec2 basePos = ImGui::GetCursorScreenPos();

		ImGui::Image(steve, ImVec2(75, 225));

		int slotSize = 70;
		int slotSpacingx = 5;
		int slotSpacingy = 5;
		int columns = 3;
		int rows = 2;

		for (int i = 0; i < rows * columns-1; ++i) {
			int row = i / columns;
			int col = i % columns;

			ImVec2 pos = ImVec2(
				basePos.x + 250 + col * (slotSize + slotSpacingx),
				basePos.y + 60 + row * (slotSize + slotSpacingy));

			if (i == 2) {pos.y += slotSize/2; pos.x += 40;}
			
			ImVec2 size = ImVec2(pos.x + slotSize, pos.y + slotSize);
			
			drawList->AddRectFilled(pos,size,IM_COL32(180, 180, 180, 255));

			std::string text = std::to_string(i);
			ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
			ImVec2 textPos = ImVec2(
				pos.x + (slotSize - textSize.x) / 2,
				pos.y + (slotSize - textSize.y) / 2
			);
			drawList->AddText(textPos, IM_COL32(0, 0, 0, 255), text.c_str());
		}

		columns = 9;
		rows = 4;

		Inventory inventory = player.get_inventory();

		for (int i = 0; i < rows * columns; ++i) {
			int row = rows - 1 - (i / columns);
			int col = i % columns;

			if (row == rows-1) {
				slotSpacingy = 15;
			}
			else
			{
				slotSpacingy = 5;
			}
			

			std::shared_ptr<Item> item = inventory.get_inventory()[i];

			std::string text;
			if (item != nullptr) {
				text = item->getItemName();
			} else {
				text = std::string(" ");
			}
			
			ImVec2 pos = ImVec2(
				basePos.x + col * (slotSize + slotSpacingx),
				basePos.y + 250 + row * (slotSize + slotSpacingy)
			);
			ImVec2 size = ImVec2(pos.x + slotSize, pos.y + slotSize);

			drawList->AddRectFilled(pos, size, IM_COL32(180, 180, 180, 255));

			ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
			ImVec2 textPos = ImVec2(
				pos.x + (slotSize - textSize.x) / 2,
				pos.y + (slotSize - textSize.y) / 2
			);
			drawList->AddText(textPos, IM_COL32(0, 0, 0, 255), text.c_str());
		}

		ImGui::End();
	}
	else{

		ImGui::SetNextWindowPos(ImVec2(500, 870));
		ImGui::SetNextWindowSize(ImVec2(500, 450));
		ImGui::Begin("Life", nullptr,
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoBackground);

		GLuint heartTex = LoadTextureFromFile("assets/heart/heart_right.png");
		ImTextureID heart_right = (ImTextureID)(intptr_t)heartTex;
		heartTex = LoadTextureFromFile("assets/heart/heart_left.png");
		ImTextureID heart_left = (ImTextureID)(intptr_t)heartTex;

		for (int i = 0; i < player.getLife()/2; ++i) {
			ImGui::Image(heart_left, ImVec2(20, 20));
			ImGui::SameLine(0.0f, 0.20f);
			ImGui::Image(heart_right, ImVec2(20,20));
			ImGui::SameLine(0.0f,0.20f);
		}

		for (int i = 0; i < player.getLife()%2; ++i) {
			ImGui::Image(heart_left, ImVec2(20, 20));
			ImGui::SameLine(0.0f, 0.20f);
		}

		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(1000, 870));
		ImGui::SetNextWindowSize(ImVec2(500, 450));
		ImGui::Begin("Hunger", nullptr,
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoBackground);

		GLuint hungerTex = LoadTextureFromFile("assets/hunger/hunger_right.png");
		ImTextureID hunger_right = (ImTextureID)(intptr_t)hungerTex;
		hungerTex = LoadTextureFromFile("assets/hunger/hunger_left.png");
		ImTextureID hunger_left = (ImTextureID)(intptr_t)hungerTex;

		for (int i = 0; i < player.get_hunger()/2; ++i) {
			ImGui::Image(hunger_left, ImVec2(20, 20));
			ImGui::SameLine(0.0f, 0.20f);
			ImGui::Image(hunger_right, ImVec2(20,20));
			ImGui::SameLine(0.0f,0.20f);
		}

		for (int i = 0; i < player.get_hunger()%2; ++i) {
			ImGui::Image(hunger_left, ImVec2(20, 20)); 
			ImGui::SameLine(0.0f, 0.20f);
		}

		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(660, 900));
		ImGui::SetNextWindowSize(ImVec2(600, 100));
		ImGui::Begin("Inventory", nullptr,
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoBackground);

		ImVec2 basePos = ImGui::GetCursorScreenPos();  // position absolue de début
		auto* drawList = ImGui::GetWindowDrawList();

		int slotSize = 64;
		int slotSpacing = 0;

		Inventory inventory = player.get_inventory();

		for (int i = 0; i < 9; ++i) {
			// Position du slot i
			ImVec2 pos = ImVec2(basePos.x + i * (slotSize + slotSpacing), basePos.y);
			ImVec2 size = ImVec2(pos.x + slotSize, pos.y + slotSize);

			// Fond transparent (ne rien dessiner ou transparent total)
			// Bordure blanche
			drawList->AddRect(pos, size, IM_COL32(255, 255, 255, 255), 0.0f, 0, 5.0f); // épaisseur 2

			std::shared_ptr<Item> item = inventory.get_inventory()[i];

			std::string text;
			if (item != nullptr) {text = item->getItemName();}
			else
			{
				text = std::string(" ");
			}

			ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
			ImVec2 textPos = ImVec2(
				pos.x + (slotSize - textSize.x) / 2,
				pos.y + (slotSize - textSize.y) / 2
			);
			drawList->AddText(textPos, IM_COL32(0, 0, 0, 255), text.c_str());
		}
		ImGui::End();
	}
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

