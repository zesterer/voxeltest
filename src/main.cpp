#include "stdio.h"

#include "libvolume/window/window.h"
#include "libvolume/common/io.h"
#include "libvolume/engine/realm.h"
#include "libvolume/engine/actor.h"

int main(int argc, char* argv[])
{
	LibVolume::IO::output("Starting VoxelTest");

	LibVolume::Window::Window window;
	window.activate(true);
	window.outputContextDebug();

	LibVolume::Engine::Realm realm;
	realm.setEventManager(&window.event_manager);

	LibVolume::Engine::Actor* ship = new LibVolume::Engine::Actor();
	ship->mesh.loadFromOBJ("../X-Wing.obj.test");
	ship->state.position = glm::vec3(50.0, 0.0, 0.0);
	ship->state.scale = glm::vec3(1.5, 1.5, 1.5);
	ship->mesh_state.position += glm::vec3(0.0, -20.0, 0.0);
	ship->mesh_state.orientation = glm::quat(glm::vec3(3.141, 0.0, 3.141));
	realm.objects.push_back(dynamic_cast<LibVolume::Engine::Object*>(ship));

	LibVolume::Engine::Actor* planet = new LibVolume::Engine::Actor();
	planet->mesh.loadFromOBJ("../Planet0.obj.test");
	realm.objects.push_back(dynamic_cast<LibVolume::Engine::Object*>(planet));

	LibVolume::Render::Structures::Light sun(LibVolume::Render::Structures::LightType::Directional, glm::vec3(0.5, 0.5, -1.0), glm::vec3(1.0, 1.0, 0.9), 0.2);
	realm.light_list.push_back(&sun);

	//Run the window
	while (window.tick() == false)
	{
		if (window.event_manager.keyboard_state.key_space)
			ship->state.velocity += ship->state.orientation * glm::vec3(0.0, 0.1, 0.0);
		if (window.event_manager.keyboard_state.key_shift)
			ship->state.velocity += ship->state.orientation * glm::vec3(0.0, -0.1, 0.0);
		if (window.event_manager.keyboard_state.key_left)
			ship->state.velocity += ship->state.orientation * glm::vec3(-0.1, 0.0, 0.0);
		if (window.event_manager.keyboard_state.key_right)
			ship->state.velocity += ship->state.orientation * glm::vec3(0.1, 0.0, 0.0);
		if (window.event_manager.keyboard_state.key_up)
			ship->state.velocity += ship->state.orientation * glm::vec3(0.0, 0.0, -0.1);
		if (window.event_manager.keyboard_state.key_down)
			ship->state.velocity += ship->state.orientation * glm::vec3(0.0, 0.0, 0.1);

		if (window.event_manager.keyboard_state.key_w)
			ship->state.orientation *= glm::quat(glm::vec3(-0.03, 0.0, 0.0));// * ship->state.orientation;
		if (window.event_manager.keyboard_state.key_a)
			ship->state.orientation *= glm::quat(glm::vec3(0.0, 0.03, 0.0));// * ship->state.orientation;
		if (window.event_manager.keyboard_state.key_s)
			ship->state.orientation *= glm::quat(glm::vec3(0.03, 0.0, 0.0));// * ship->state.orientation;
		if (window.event_manager.keyboard_state.key_d)
			ship->state.orientation *= glm::quat(glm::vec3(0.0, -0.03, 0.0));// * ship->state.orientation;
		if (window.event_manager.keyboard_state.key_q)
			ship->state.orientation *= glm::quat(glm::vec3(0.0, 0.0, 0.03));// * ship->state.orientation;
		if (window.event_manager.keyboard_state.key_e)
			ship->state.orientation *= glm::quat(glm::vec3(0.0, 0.0, -0.03));// * ship->state.orientation;

		realm.camera.state.position = ship->state.position + glm::vec3(0.0, 50.0, 300.0) * glm::inverse(ship->state.orientation);
		realm.camera.state.orientation = glm::inverse(ship->state.orientation);

		//Gravity
		//ship->state.velocity += glm::normalize(planet->state.position - ship->state.position) * 0.01f;

		realm.tick();
		realm.render();
	};

	return 0;
}
