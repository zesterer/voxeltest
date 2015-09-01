#include "stdio.h"
#include "cstdlib"

#include "libvolume/window/window.h"
#include "libvolume/common/io.h"
#include "libvolume/engine/realm.h"
#include "libvolume/engine/actor.h"

void addPlanet(LibVolume::Engine::Realm* realm)
{
	LibVolume::Engine::Actor* planet1 = new LibVolume::Engine::Actor();
	planet1->mesh.loadFromOBJ("../earth.obj.test");
	float x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	planet1->state.position = (glm::vec3(x, y, z) - 0.5f) * 100000.0f;
	planet1->mesh.colour = glm::vec3(r, g, b);
	planet1->state.scale = glm::vec3(5000.0, 5000.0, 5000.0);
	realm->objects.push_back(dynamic_cast<LibVolume::Engine::Object*>(planet1));
}

int main(int argc, char* argv[])
{
	LibVolume::IO::output("Starting VoxelTest");

	LibVolume::Window::Window window;
	window.activate(true);
	window.outputContextDebug();

	LibVolume::Engine::Realm realm;
	realm.setEventManager(&window.event_manager);

	LibVolume::Engine::Actor* ship = new LibVolume::Engine::Actor();
	ship->mesh.loadFromOBJ("../Starfighter.obj.test");
	ship->state.position = glm::vec3(50.0, 0.0, 0.0);
	ship->state.scale = glm::vec3(1.5, 1.5, 1.5);
	ship->mesh_state.position += glm::vec3(0.0, -20.0, 0.0);
	ship->mesh_state.orientation = glm::quat(glm::vec3(3.141, 0.0, 3.141));
	ship->mesh.colour = glm::vec3(0.5, 0.05, 0.05);
	realm.objects.push_back(dynamic_cast<LibVolume::Engine::Object*>(ship));

	LibVolume::Engine::Actor* planet0 = new LibVolume::Engine::Actor();
	planet0->mesh.loadFromOBJ("../earth.obj.test");
	planet0->mesh.colour = glm::vec3(0.0, 0.7, 0.0);
	planet0->state.scale = glm::vec3(5000.0, 5000.0, 5000.0);
	realm.objects.push_back(dynamic_cast<LibVolume::Engine::Object*>(planet0));

	LibVolume::Engine::Actor* sky = new LibVolume::Engine::Actor();
	sky->mesh.loadFromOBJ("../skybox.obj.test");
	sky->mesh_state.scale = glm::vec3(100.0, 100.0, 100.0);
	realm.objects.push_back(dynamic_cast<LibVolume::Engine::Object*>(sky));

	for (int count = 0; count < 10; count ++)
		addPlanet(&realm);

	LibVolume::Render::Structures::Light sun(LibVolume::Render::Structures::LightType::Directional, glm::vec3(0.5, 0.5, -1.0), glm::vec3(1.0, 1.0, 0.9), 0.5);
	realm.light_list.push_back(&sun);

	//Run the window
	while (window.tick() == false)
	{
		realm.tick();

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
			ship->state.spin = (glm::quat(ship->state.orientation * glm::vec3(-0.001, 0.0, 0.0))) * ship->state.spin;
		if (window.event_manager.keyboard_state.key_a)
			ship->state.spin = (glm::quat(ship->state.orientation * glm::vec3(0.0, 0.001, 0.0))) * ship->state.spin;
		if (window.event_manager.keyboard_state.key_s)
			ship->state.spin = (glm::quat(ship->state.orientation * glm::vec3(0.001, 0.0, 0.0))) * ship->state.spin;
		if (window.event_manager.keyboard_state.key_d)
			ship->state.spin = (glm::quat(ship->state.orientation * glm::vec3(0.0, -0.001, 0.0))) * ship->state.spin;
		if (window.event_manager.keyboard_state.key_q)
			ship->state.spin = (glm::quat(ship->state.orientation * glm::vec3(0.0, 0.0, 0.001))) * ship->state.spin;
		if (window.event_manager.keyboard_state.key_e)
			ship->state.spin = (glm::quat(ship->state.orientation * glm::vec3(0.0, 0.0, -0.001))) * ship->state.spin;

		realm.camera.state.position = glm::inverse(ship->state.orientation) * ship->state.velocity + ship->state.position + glm::vec3(0.0, 50.0, 300.0) * glm::inverse(ship->state.orientation);
		realm.camera.state.orientation = glm::inverse(ship->state.orientation * glm::inverse(ship->state.spin * ship->state.spin));

		ship->state.spin = glm::mix(ship->state.spin, glm::quat(), 0.015f * ship->state.spin.w);

		sky->state.position = ship->state.position;

		//Gravity
		//ship->state.velocity += glm::normalize(planet->state.position - ship->state.position) * 0.01f;

		realm.render();
	};

	return 0;
}
