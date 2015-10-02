#include "stdio.h"
#include "cstdlib"

#include "libvolume/window/window.h"
#include "libvolume/common/io.h"
#include "libvolume/engine/realm.h"
#include "libvolume/engine/actor.h"
#include "libvolume/engine/voxelactor.h"

void addPlanet(LibVolume::Engine::Realm* realm)
{
	LibVolume::Engine::Actor* planet1 = new LibVolume::Engine::Actor();
	planet1->mesh->loadFromOBJ("../Planet0.obj.test");
	float x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	planet1->state.position = (glm::vec3(x, y, z) - 0.5f) * 100000.0f;
	planet1->mesh->colour = glm::vec3(r, g, b);
	planet1->state.scale = glm::vec3(10.0, 10.0, 10.0);
	realm->addObject(*planet1);
}

int main(int argc, char* argv[])
{
	LibVolume::IO::output("Starting VoxelTest");

	LibVolume::Window::Window window;
	window.activate(true);
	window.outputContextDebug();

	LibVolume::Engine::Realm realm;
	realm.linkTo(window);

	LibVolume::Engine::Actor ship;
	ship.mesh->loadFromOBJ("../spaceship0.obj.test", true);
	ship.state.position = glm::vec3(50.0, 0.0, 0.0);
	ship.state.scale = glm::vec3(20.0, 20.0, 20.0);
	ship.mesh_state.orientation = glm::quat(glm::vec3(-1.55, 0.0, 1.55));
	ship.mesh->colour = glm::vec3(0.5, 0.05, 0.05);
	realm.addObject(ship);

	LibVolume::Engine::Actor ship2;
	ship2.mesh->loadFromOBJ("../spaceship0.obj.test", true);
	ship2.state.position = glm::vec3(500.0, 0.0, 0.0);
	ship2.state.scale = glm::vec3(20.0, 20.0, 20.0);
	ship2.mesh_state.orientation = glm::quat(glm::vec3(-1.55, 0.0, 1.55));
	ship2.mesh->colour = glm::vec3(0.5, 0.05, 0.05);
	realm.addObject(ship2);

	LibVolume::Engine::VoxelActor asteroid(glm::ivec3(16, 16, 16));
	asteroid.state.scale = glm::vec3(200.0, 200.0, 200.0);
	asteroid.state.position = glm::vec3(3000.0, 0.0, 0.0);
	asteroid.mesh_state.position = glm::vec3(-8.0, -8.0, -8.0);
	asteroid.mesh->colour = glm::vec3(0.5, 0.4, 0.2);

	for (int x = -8; x < 8; x ++)
	{
		for (int y = -8; y < 8; y ++)
		{
			for (int z = -8; z < 8; z ++)
			{
				float d = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

				asteroid.getAt(glm::ivec3(x + 8, y + 8, z + 8))->density = (0.1 + d) * (8.0 - std::sqrt(x * x + y * y + z * z));
			}
		}
	}

	float a = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float c = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	asteroid.state.spin = glm::quat(glm::vec3(2.0 * a - 1.0, 2.0 * b - 1.0, 2.0 * c - 1.0) * 0.01f);

	asteroid.extract(LibVolume::Engine::MeshingAlgorithm::MarchingCubes);
	realm.addObject(asteroid);

	/*LibVolume::Engine::Actor* planet0 = new LibVolume::Engine::Actor();
	planet0->mesh->loadFromOBJ("../earth.obj.test");
	planet0->mesh->colour = glm::vec3(0.0, 0.7, 0.0);
	planet0->state.scale = glm::vec3(5000.0, 5000.0, 5000.0);
	realm.objects.push_back(dynamic_cast<LibVolume::Engine::Object*>(planet0));*/

	for (int count = 0; count < 4; count ++)
		addPlanet(&realm);

	LibVolume::Render::Structures::Light sun(LibVolume::Render::Structures::LightType::Directional, glm::vec3(0.5, 0.5, -1.0), glm::vec3(1.0, 1.0, 0.9), 0.5);
	//realm.light_list.push_back(&sun);
	realm.addLight(sun);

	LibVolume::Render::Structures::Mesh lasermesh;
	lasermesh.loadFromOBJ("../laser.obj.test");
	long long time_since_last_shot = 0;

	//Run the window
	while (window.tick() == false)
	{
		if (window.event_manager.keyboard_state.key_space)
			ship.state.velocity += ship.state.orientation * glm::vec3(0.0, 0.1, 0.0);
		if (window.event_manager.keyboard_state.key_shift)
			ship.state.velocity += ship.state.orientation * glm::vec3(0.0, -0.1, 0.0);
		if (window.event_manager.keyboard_state.key_left)
			ship.state.velocity += ship.state.orientation * glm::vec3(-0.1, 0.0, 0.0);
		if (window.event_manager.keyboard_state.key_right)
			ship.state.velocity += ship.state.orientation * glm::vec3(0.1, 0.0, 0.0);
		if (window.event_manager.keyboard_state.key_up)
			ship.state.velocity += ship.state.orientation * glm::vec3(0.0, 0.0, -0.1);
		if (window.event_manager.keyboard_state.key_down)
			ship.state.velocity += ship.state.orientation * glm::vec3(0.0, 0.0, 0.1);

		if (window.event_manager.keyboard_state.key_w)
			ship.state.spin = (glm::quat(ship.state.orientation * glm::vec3(-0.001, 0.0, 0.0))) * ship.state.spin;
		if (window.event_manager.keyboard_state.key_a)
			ship.state.spin = (glm::quat(ship.state.orientation * glm::vec3(0.0, 0.001, 0.0))) * ship.state.spin;
		if (window.event_manager.keyboard_state.key_s)
			ship.state.spin = (glm::quat(ship.state.orientation * glm::vec3(0.001, 0.0, 0.0))) * ship.state.spin;
		if (window.event_manager.keyboard_state.key_d)
			ship.state.spin = (glm::quat(ship.state.orientation * glm::vec3(0.0, -0.001, 0.0))) * ship.state.spin;
		if (window.event_manager.keyboard_state.key_q)
			ship.state.spin = (glm::quat(ship.state.orientation * glm::vec3(0.0, 0.0, 0.001))) * ship.state.spin;
		if (window.event_manager.keyboard_state.key_e)
			ship.state.spin = (glm::quat(ship.state.orientation * glm::vec3(0.0, 0.0, -0.001))) * ship.state.spin;

		realm.camera.state.orientation = glm::inverse(glm::mix(ship.state.spin, glm::quat(), 8.0f) * ship.state.orientation);
		realm.camera.state.position = ship.state.position + glm::vec3(0.0, 50.0, 300.0) * realm.camera.state.orientation;

		ship.state.spin = glm::mix(ship.state.spin, glm::quat(), 0.015f + 0.010f * ship.state.spin.w);

		if (window.event_manager.keyboard_state.key_enter && time_since_last_shot > 10)
		{
			LibVolume::Engine::Actor* laser = new LibVolume::Engine::Actor();
			laser->mesh = &lasermesh;
			laser->mesh->colour = glm::vec3(1.0, 0.0, 0.0);
			laser->state.position = ship.state.position + ship.state.orientation * glm::vec3(50.0, 0.0, -100.0);
			laser->state.scale = glm::vec3(4.0, 4.0, 8.0);
			laser->state.orientation = ship.state.orientation;
			laser->state.velocity = ship.state.velocity + ship.state.orientation * glm::vec3(0.0, 0.0, -100.0);
			realm.addObject(*laser);

			laser = new LibVolume::Engine::Actor();
			laser->mesh = &lasermesh;
			laser->mesh->colour = glm::vec3(1.0, 0.0, 0.0);
			laser->state.position = ship.state.position + ship.state.orientation * glm::vec3(-50.0, 0.0, -100.0);
			laser->state.scale = glm::vec3(4.0, 4.0, 8.0);
			laser->state.orientation = ship.state.orientation;
			laser->state.velocity = ship.state.velocity + ship.state.orientation * glm::vec3(0.0, 0.0, -100.0);
			realm.addObject(*laser);

			time_since_last_shot = 0;
		}

		//ship2.state.velocity += ship2.state.orientation * glm::vec3(0.0, 0.0, -0.1);

		float n = glm::min(0.05f, glm::abs((180.0f / 3.141f) * (float)acos(glm::dot(ship2.state.position - ship.state.position, ship2.state.orientation * glm::vec3(0.0, 0.0, -1.0)))));
		glm::quat p = glm::angleAxis(n, glm::normalize(glm::cross(ship2.state.position - ship.state.position, ship2.state.orientation * glm::vec3(0.0, 0.0, -1.0))));
		ship2.state.orientation = p * ship2.state.orientation;//glm::mix(glm::inverse(glm::toQuat(look)), ship2.state.orientation, 0.97f);

		if (realm.time % 50 == 0)
		{
			LibVolume::Engine::Actor* laser = new LibVolume::Engine::Actor();
			laser->mesh = &lasermesh;
			laser->mesh->colour = glm::vec3(1.0, 0.0, 0.0);
			laser->state.position = ship2.state.position + ship2.state.orientation * glm::vec3(-50.0, 0.0, -100.0);
			laser->state.scale = glm::vec3(4.0, 4.0, 8.0);
			laser->state.orientation = ship.state.orientation;
			laser->state.velocity = ship2.state.velocity + ship2.state.orientation * glm::vec3(0.0, 0.0, -100.0);
			realm.addObject(*laser);

			laser = new LibVolume::Engine::Actor();
			laser->mesh = &lasermesh;
			laser->mesh->colour = glm::vec3(1.0, 0.0, 0.0);
			laser->state.position = ship2.state.position + ship2.state.orientation * glm::vec3(50.0, 0.0, -100.0);
			laser->state.scale = glm::vec3(4.0, 4.0, 8.0);
			laser->state.orientation = ship.state.orientation;
			laser->state.velocity = ship2.state.velocity + ship2.state.orientation * glm::vec3(0.0, 0.0, -100.0);
			realm.addObject(*laser);
		}

		time_since_last_shot ++;

		realm.tick();

		//Gravity
		//ship.state.velocity += glm::normalize(planet->state.position - ship.state.position) * 0.01f;

		realm.render();
	};

	return 0;
}
