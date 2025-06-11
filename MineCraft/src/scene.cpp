#include "scene.hpp"


using namespace cgp;

void scene_structure::initialize()
{
	player.get_camera().look_at({ 3.0f, 2.0f, 2.0f }, {0,0,0}, {0,0,1});
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());



	world.initialize(10, 10, 30);
	player.initialize(inputs,window,world);
	chick.initialize(inputs,window,world);
	zombie.initialize(inputs,window,world);
    
    std::vector<cgp::vec3> testPositions = {
        {0, 0, 0},      // Should be in first chunk
        {5, 5, 5},      // Should be in first chunk
        {15, 15, 15},   // Should be in first chunk
        {8, 0, 8},      // Should be in first chunk
        {0, 5, 0},      // Should be in first chunk, middle height
        {player.getPosition().x, player.getPosition().y, player.getPosition().z} // Player position
    };
    
    //std::cout << "\nTesting world block access:" << std::endl;
    for(size_t i = 0; i < testPositions.size(); i++) {
        cgp::vec3 pos = testPositions[i];
        //std::cout << "\n--- Test " << i+1 << ": Position (" << pos.x << ", " << pos.y << ", " << pos.z << ") ---" << std::endl;
        
        BlockType block = world.getBlock(pos);
        //std::cout << "Result: Block type " << static_cast<int>(block) << std::endl;
        
        if(block != AIR) {
            //std::cout << "SUCCESS: Found non-air block!" << std::endl;
            break; 
        }
    }

	glfwSetInputMode(window.glfw_window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);

}


void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	environment.light = player.get_camera().camera_model.position();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	world.renderCached(player.getPosition(), environment);

	

	cgp::draw(chick.get_mesh_drawable(),environment);
	cgp::draw(zombie.get_mesh_drawable(),environment);

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
		glfwSetInputMode(window.glfw_window,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
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

		Inventory& craft = player.get_craft();
		Inventory& inventory = player.get_inventory();

		for (int i = 0; i < rows * columns-1; ++i) {
			int row = i / columns;
			int col = i % columns;

			ImVec2 pos = ImVec2(
				basePos.x + 250 + col * (slotSize + slotSpacingx),
				basePos.y + 60 + row * (slotSize + slotSpacingy));

			if (i == 2) {pos.y += slotSize/2; pos.x += 40;}
			
			std::vector<std::shared_ptr<Item>> vec_item = craft.get_inventory()[i];

			std::string text;
			if (!vec_item.empty()) {text = vec_item[0]->getItemName() + std::to_string(vec_item.size()) + " craft " + std::to_string(i);}
			else
			{
				text = std::to_string(i);
			}

			std::string label = "craft " + std::to_string(i);
			ImGui::SetCursorScreenPos(pos);

			if (ImGui::Button(text.c_str(), ImVec2(slotSize, slotSize))) {
				
				if (inputs.keyboard.is_pressed(GLFW_KEY_LEFT_SHIFT))
				{
					player.set_craft().fast_switch_inventory(i,SlotType::CRAFT,inventory);
					player.set_craft().check_craft();
				}				
			}

			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
				DragPayload payload_data = { i, SlotType::CRAFT };
				ImGui::SetDragDropPayload("ITEM_SLOT", &payload_data, sizeof(DragPayload));
				ImGui::Text("Déplacer %s", text.c_str()); // Apparence de l'objet en drag
				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ITEM_SLOT")) {
					const DragPayload* payload_data = static_cast<const DragPayload*>(payload->Data);
					int from_index = payload_data->index;
					SlotType from_type = payload_data->source;
					std::string bjr;
					if (from_type == SlotType::CRAFT) {bjr = "craft";}else{bjr = "inventory";}
					if (i!=from_index || from_type != SlotType::CRAFT)
					{
						player.set_craft().switch_inventory(from_index, i,from_type,SlotType::CRAFT,inventory,inputs.keyboard.is_pressed(GLFW_KEY_R),inputs.keyboard.is_pressed(GLFW_KEY_T));
						player.set_craft().check_craft();

						if (from_type== SlotType::CRAFT and from_index == 2)
						{
							player.set_craft().finish_craft(true);
							player.set_craft().check_craft();
						}
						
					}
				}
				ImGui::EndDragDropTarget();
			}
		}

		columns = 9;
		rows = 4;

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
			

			std::vector<std::shared_ptr<Item>> vec_item = inventory.get_inventory()[i];

			std::string text;
			if (!vec_item.empty()) {text = vec_item[0]->getItemName() + std::to_string(vec_item.size()) + " inv " + std::to_string(i);}
			else
			{
				text = std::to_string(i);
			}
			
			ImVec2 pos = ImVec2(
				basePos.x + col * (slotSize + slotSpacingx),
				basePos.y + 250 + row * (slotSize + slotSpacingy)
			);

			std::string label = "inv " + std::to_string(i);
			ImGui::SetCursorScreenPos(pos);

			if (ImGui::Button(text.c_str(), ImVec2(slotSize, slotSize))) {
				
				if (inputs.keyboard.is_pressed(GLFW_KEY_LEFT_SHIFT))
				{
					player.set_inventory().fast_switch_inventory(i,SlotType::INVENTORY,inventory);
				}
			}

			// Si l'utilisateur commence à "tirer" le bouton
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
				DragPayload payload_data = { i, SlotType::INVENTORY };
				ImGui::SetDragDropPayload("ITEM_SLOT", &payload_data, sizeof(DragPayload));
				ImGui::Text("Déplacer %s", text.c_str()); // Apparence de l'objet en drag
				ImGui::EndDragDropSource();
			}

			// Si c’est une cible de drop
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ITEM_SLOT")) {
					const DragPayload* payload_data = static_cast<const DragPayload*>(payload->Data);
					int from_index = payload_data->index;
					SlotType from_type = payload_data->source;
					std::string bjr;
					if (from_type == SlotType::CRAFT) {bjr = "craft";}else{bjr = "inventory";}
					if (i!=from_index || from_type != SlotType::INVENTORY)
					{
						player.set_inventory().switch_inventory(from_index, i,from_type,SlotType::INVENTORY,craft,inputs.keyboard.is_pressed(GLFW_KEY_R),inputs.keyboard.is_pressed(GLFW_KEY_T));
						player.set_craft().check_craft();
						if (from_type== SlotType::CRAFT and from_index == 2)
						{
							player.set_craft().finish_craft(true);
							player.set_craft().check_craft();
						}
					}
				}
				ImGui::EndDragDropTarget();
			}
		}

		ImGui::End();
	}
	else{
		glfwSetInputMode(window.glfw_window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);

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
		ImU32 color;

		for (int i = 0; i < 9; ++i) {
			// Position du slot i
			ImVec2 pos = ImVec2(basePos.x + i * (slotSize + slotSpacing), basePos.y);
			ImVec2 size = ImVec2(pos.x + slotSize, pos.y + slotSize);

			if (player.get_item_ind() == i){
				color = IM_COL32(250,30,30,255);
			}
			else{
				color = IM_COL32(255, 255, 255, 255);
			}

			drawList->AddRect(pos, size, color, 0.0f, 0, 2.0f);

			std::vector<std::shared_ptr<Item>> vec_item = inventory.get_inventory()[i];

			std::string text;
			if (!vec_item.empty()) {text = vec_item[0]->getItemName() + std::to_string(vec_item.size());}
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

		ImVec2 center = ImVec2(window.width * 0.5f, window.height * 0.5f);
		float size = 25.0f;

		ImGui::SetNextWindowPos(ImVec2(center.x-2*size,center.y-2*size));
		ImGui::SetNextWindowSize(ImVec2(5*size, 5*size));
		ImGui::Begin("Cursor", nullptr,
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoBackground);

		basePos = ImGui::GetCursorScreenPos();
		drawList = ImGui::GetWindowDrawList();
		color = IM_COL32(0, 0, 0, 255);

		drawList->AddLine(ImVec2(center.x - size, center.y),
						ImVec2(center.x + size, center.y), color, 3.0f);

		drawList->AddLine(ImVec2(center.x, center.y - size),
                       ImVec2(center.x, center.y + size), color, 3.0f);
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

