#pragma once


#include "cgp/cgp.hpp"
#include "environment.hpp"
#include "entity/player.hpp"
#include "block/grass.hpp"
#include "block/sand.hpp"
#include "block/stone.hpp"
#include "world/world.hpp"
#include "world/primary_world.hpp"
#include "world/chunk.hpp"
#include "inventory/inventory.hpp"
#include "entity/mob.hpp"
#include "entity/chicken.hpp"
#include "entity/zombie.hpp"
#include <map> 
#include <memory>
#include "stb_image.h"

using cgp::mesh_drawable;


struct gui_parameters {
	bool display_frame = true;
	bool display_wireframe = true;
};

// The structure of the custom scene
struct scene_structure : cgp::scene_inputs_generic {
	
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //
	camera_controller_orbit_euler camera_control;
	camera_projection_perspective camera_projection;
	window_structure window;
	Player player;
	Chicken chick;
	Zombie zombie;


	Chunk chunk;
	World world;
	Primary_World primary_world;

	//Grass* grass;
	Grass grass;
	std::vector<Block*> vectorBlockType;

	mesh_drawable global_frame;          // The standard global frame
	environment_structure environment;   // Standard environment controler
	input_devices inputs;                // Storage for inputs status (mouse, keyboard, window dimension)
	gui_parameters gui;                  // Standard GUI element storage
	
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //




	// ****************************** //
	// Functions
	// ****************************** //

	void initialize();    // Standard initialization to be called before the animation loop
	void display_frame(); // The frame display to be called within the animation loop
	void display_gui();   // The display of the GUI, also called within the animation loop

	void display_inventory_ui();


	void mouse_move_event();
	void mouse_click_event();
	void keyboard_event();
	void idle_frame();

};





