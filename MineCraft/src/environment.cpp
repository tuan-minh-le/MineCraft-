#include "environment.hpp"

// Change these global values to modify the default behavior
// ************************************************************* //
// The initial zoom factor on the GUI
float project::gui_scale = 1.0f;
// Is FPS limited automatically
bool project::fps_limiting = false;
// Maximal default FPS (used only of fps_max is true)
float project::fps_max=60.0f;
// Automatic synchronization of GLFW with the vertical-monitor refresh
bool project::vsync=false;     
// Initial dimension of the OpenGL window (ratio if in [0,1], and absolute pixel size if > 1)
float project::initial_window_size_width  = 0.95f; 
float project::initial_window_size_height = 0.95f;
// ************************************************************* //


// This path will be automatically filled when the program starts
std::string project::path = ""; 




void environment_structure::send_opengl_uniform(opengl_shader_structure const& shader, bool expected) const
{
	opengl_uniform(shader, "projection", camera_projection, expected);
	opengl_uniform(shader, "view", camera_view, expected);
	opengl_uniform(shader, "light", light, false);

	uniform_generic.send_opengl_uniform(shader, expected);

}