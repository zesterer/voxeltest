#include "stdio.h"
#include "cstdlib"

#include "libvolume/window/window.h"
#include "libvolume/common/io.h"
#include "libvolume/engine/realm.h"
#include "libvolume/engine/actor.h"
#include "libvolume/engine/voxelactor.h"
#include "libvolume/engine/voxelterrain.h"
#include "libvolume/generation/perlin.h"

void addPlanet(LibVolume::Engine::Realm* realm)
{
	LibVolume::Engine::VoxelActor* planet1 = new LibVolume::Engine::VoxelActor(glm::ivec3(32, 32, 32));
	planet1->state.scale = glm::vec3(1000.0, 1000.0, 1000.0);
	planet1->mesh_state.position = glm::vec3(-16.0, -16.0, -16.0);
	//planet1->mesh->loadFromOBJ("../Planet0.obj.test");

	for (int x = -16; x < 16; x ++)
	{
		for (int y = -16; y < 16; y ++)
		{
			for (int z = -16; z < 16; z ++)
			{
				float d = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

				planet1->getAt(glm::ivec3(x + 16, y + 16, z + 16))->density = (0.1 + d) * (16.0 - std::sqrt(x * x + y * y + z * z));
			}
		}
	}

	planet1->extract(LibVolume::Engine::MeshingAlgorithm::MarchingCubes);

	float a = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float c = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	planet1->state.spin = glm::quat(glm::vec3(2.0 * a - 1.0, 2.0 * b - 1.0, 2.0 * c - 1.0) * 0.001f);

	float x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	planet1->state.position = (glm::vec3(x, y, z) - 0.5f) * 100000.0f;
	planet1->mesh->colour = glm::vec3(r, g, b);
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
	realm.addObject(ship);

	LibVolume::Engine::Actor ship2;
	ship2.mesh->loadFromOBJ("../spaceship0.obj.test", true);
	ship2.state.position = glm::vec3(500.0, 0.0, 0.0);
	ship2.state.scale = glm::vec3(20.0, 20.0, 20.0);
	ship2.mesh_state.orientation = glm::quat(glm::vec3(-1.55, 0.0, 1.55));
	//realm.addObject(ship2);

	//Create a terrain object
	LibVolume::Engine::VoxelTerrain terrain(glm::vec3(8, 8, 8));
	terrain.state.scale = glm::vec3(50.0, 50.0, 50.0);
	terrain.state.position = glm::vec3(0.0, 0.0, -3000.0);

	LibVolume::Generation::PerlinNoise noise;

	for (int xx = -3; xx < 3; xx ++)
	{
		for (int yy = -3; yy < 3; yy ++)
		{
			for (int zz = -3; zz < 3; zz ++)
			{
				terrain.loadAt(glm::ivec3(xx, yy, zz));
				terrain.getAt(glm::ivec3(xx, yy, zz))->mesh->colour = glm::vec3(0.8, 0.6, 0.4);//glm::vec3(cr, cg, cb);
				glm::ivec3 p = terrain.getAt(glm::ivec3(xx, yy, zz))->location;

				for (int x = 0; x < 8; x ++)
				{
					for (int y = 0; y < 8; y ++)
					{
						for (int z = 0; z < 8; z ++)
						{
							glm::vec3 n = glm::normalize(glm::vec3((p.x + x), (p.y + y), (p.z + z))) * 20.0f + glm::vec3(537.0, 537.0, 537.0);
							float variant = noise.getPerlin(glm::vec4(n.x, n.y, n.z, 7.0) * 4.0f, -7.5, 3.0, 1.5);
							terrain.getAt(glm::ivec3(xx, yy, zz))->getAt(glm::ivec3(x, y, z))->density = (0.9) * (16.0 + variant * 6.0 - std::sqrt((p.x + x) * (p.x + x) + (p.y + y) * (p.y + y) + (p.z + z) * (p.z + z)));
						}
					}
				}
			}
		}
	}

	for (int xx = -3; xx < 3; xx ++)
	{
		for (int yy = -3; yy < 3; yy ++)
		{
			for (int zz = -3; zz < 3; zz ++)
			{
				if (terrain.existsAt(glm::ivec3(xx, yy, zz)))
					terrain.getAt(glm::ivec3(xx, yy, zz))->extract(LibVolume::Engine::MeshingAlgorithm::MarchingCubes);
			}
		}
	}

	realm.addObject(terrain);

	//for (int count = 0; count < 4; count ++)
		//addPlanet(&realm);

	LibVolume::Render::Structures::Light sun(LibVolume::Render::Structures::LightType::Directional, glm::vec3(0.5, 0.5, -1.0), glm::vec3(2.0, 2.0, 2.0), 0.1);
	realm.addLight(sun);

	LibVolume::Render::Structures::Light shiplight(LibVolume::Render::Structures::LightType::Point, glm::vec3(0.0, 0.0, 0.0), glm::vec3(4.0, 0.0, 0.0) * 1.0f, 1000.0f);
	realm.addLight(shiplight);

	LibVolume::Render::Structures::Mesh lasermesh;
	lasermesh.loadFromOBJ("../laser.obj.test", false);
	lasermesh.colour = glm::vec3(100.0, 0.0, 0.0);
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
			laser->state.position = ship.state.position + ship.state.orientation * glm::vec3(50.0, 0.0, -100.0);
			laser->state.scale = glm::vec3(4.0, 4.0, 8.0);
			laser->state.orientation = ship.state.orientation;
			laser->state.velocity = ship.state.velocity + ship.state.orientation * glm::vec3(0.0, 0.0, -100.0);
			realm.addObject(*laser);

			laser = new LibVolume::Engine::Actor();
			laser->mesh = &lasermesh;
			laser->state.position = ship.state.position + ship.state.orientation * glm::vec3(-50.0, 0.0, -100.0);
			laser->state.scale = glm::vec3(4.0, 4.0, 8.0);
			laser->state.orientation = ship.state.orientation;
			laser->state.velocity = ship.state.velocity + ship.state.orientation * glm::vec3(0.0, 0.0, -100.0);
			realm.addObject(*laser);

			time_since_last_shot = 0;
		}

		shiplight.position = ship.state.position;

		//ship2.state.velocity += ship2.state.orientation * glm::vec3(0.0, 0.0, -0.1);

		float n = glm::min(0.05f, glm::abs((180.0f / 3.141f) * (float)acos(glm::dot(ship2.state.position - ship.state.position, ship2.state.orientation * glm::vec3(0.0, 0.0, -1.0)))));
		glm::quat p = glm::angleAxis(n, glm::normalize(glm::cross(ship2.state.position - ship.state.position, ship2.state.orientation * glm::vec3(0.0, 0.0, -1.0))));
		ship2.state.orientation = p * ship2.state.orientation;//glm::mix(glm::inverse(glm::toQuat(look)), ship2.state.orientation, 0.97f);

		time_since_last_shot ++;

		realm.tick();

		realm.render();
	};

	return 0;
}
