//----STANDARD----
#include "stdio.h"
#include "cstdlib"

//----LIBRARY----
#include "libvolume/window/window.h"
#include "libvolume/common/io.h"
#include "libvolume/engine/realm.h"
#include "libvolume/engine/actor.h"
#include "libvolume/engine/voxelactor.h"
#include "libvolume/engine/voxelterrain.h"
#include "libvolume/generation/perlin.h"

//----LOCAL----
#include "bodies/planet.h"

void addPlanet(LibVolume::Engine::Realm* realm)
{
	LibVolume::Engine::VoxelActor* planet1 = new LibVolume::Engine::VoxelActor(glm::ivec3(16, 16, 16));
	planet1->state.scale = glm::vec3(200.0, 200.0, 200.0);
	planet1->mesh_state.position = glm::vec3(-16.0, -16.0, -16.0);
	//planet1->mesh->loadFromOBJ("../Planet0.obj.test");

	LibVolume::Generation::PerlinNoise noise;

	float adjust = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	for (int x = -16; x < 16; x ++)
	{
		for (int y = -16; y < 16; y ++)
		{
			for (int z = -16; z < 16; z ++)
			{
				glm::vec3 n = glm::normalize(glm::vec3(x, y, z)) * 20.0f + glm::vec3(537.0, 537.0, 537.0);
				float variant = noise.getPerlin(glm::vec4(n.x, n.y, n.z, 7.0 + adjust * 200.0) * 4.0f, -7.5, 3.0, 1.5);

				planet1->getAt(glm::ivec3(x + 8, y + 8, z + 8))->density = (0.9) * (8.0 + variant * 3.0 - std::sqrt(x * x + y * y + z * z));
			}
		}
	}

	planet1->extract(LibVolume::Engine::MeshingAlgorithm::MarchingCubes);

	float a = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float c = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	planet1->state.spin = glm::quat(glm::vec3(2.0 * a - 1.0, 2.0 * b - 1.0, 2.0 * c - 1.0) * 0.000f);

	float x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	planet1->state.position = (glm::vec3(x, y, z) - 0.5f) * 100000.0f;
	planet1->mesh->colour = glm::vec3(r, r, r);
	realm->addObject(*planet1);
}

int main(int argc, char* argv[])
{
	LibVolume::IO::output("Starting VoxelTest");

	LibVolume::Window::Window window;
	window.setTitle("VoxelTest");
	window.activate(true);
	window.outputContextDebug();

	LibVolume::Engine::Realm realm;
	realm.linkTo(window);

	LibVolume::Engine::Actor ship;
	ship.mesh->loadFromOBJ("../spaceship0.obj.test", true);
	ship.state.position = glm::vec3(50.0, 0.0, 0.0);
	ship.state.scale = glm::vec3(20.0, 20.0, 20.0);
	ship.mesh_state.orientation = glm::quat(glm::vec3(-1.55, 0.0, 1.55));
	ship.mesh->colour = glm::vec3(1.0, 1.0, 1.0) * 1.0f;
	realm.addObject(ship);

	LibVolume::Engine::Actor ship2;
	ship2.mesh->loadFromOBJ("../spaceship0.obj.test", true);
	ship2.state.position = glm::vec3(500.0, 0.0, 0.0);
	ship2.state.scale = glm::vec3(20.0, 20.0, 20.0);
	ship2.mesh_state.orientation = glm::quat(glm::vec3(-1.55, 0.0, 1.55));
	//realm.addObject(ship2);

	LibVolume::Generation::PerlinNoise noise;

	/*LibVolume::Engine::Actor planet;
	planet.state.scale = glm::vec3(2500.0, 2500.0, 2500.0);
	planet.state.position = glm::vec3(0.0, 0.0, 0.0);
	planet.mesh->colour = glm::vec3(1.0, 2.5, 1.0);
	planet.state.spin = glm::quat(glm::vec3(0.001, 0.001, 0.001));
	realm.addObject(planet);

	planet.mesh->loadFromOBJ("../icosahedron.obj");*/

	/*glm::vec3 ico[3 * 4] =
	{
		{+0.000, -1.000, +0.7071067811}, {+0.000, +1.000, +0.7071067811}, {-1.000, +0.000, -0.7071067811},
		{+0.000, -1.000, +0.7071067811}, {-1.000, +0.000, -0.7071067811}, {+1.000, +0.000, -0.7071067811},
		{-1.000, +0.000, -0.7071067811}, {+0.000, +1.000, +0.7071067811}, {+1.000, +0.000, -0.7071067811},
		{+0.000, +1.000, +0.7071067811}, {+0.000, -1.000, +0.7071067811}, {+1.000, +0.000, -0.7071067811},
	};

	for (int c = 0; c < 4; c ++)
	{
		LibVolume::Render::Structures::Polygon poly;
		poly.a.position = glm::normalize(ico[c * 3 + 0]);
		poly.b.position = glm::normalize(ico[c * 3 + 1]);
		poly.c.position = glm::normalize(ico[c * 3 + 2]);
		poly.correctNormals();
		planet.mesh->polygons.push_back(poly);
	}*/

	Vast::Bodies::Planet planet2;
	planet2.state.scale = glm::vec3(80000.0, 80000.0, 80000.0);
	planet2.state.position = glm::vec3(1310000.0, 0.0, 0.0);
	planet2.mesh->colour = glm::vec3(0.5, 0.4, 0.2);
	realm.addObject(planet2);

	/*for (int c = 0; c < 4; c ++)
	{
		std::vector<LibVolume::Render::Structures::Polygon> polys;

		for (LibVolume::Render::Structures::Polygon poly : planet.mesh->polygons)
		{
			LibVolume::Render::Structures::Polygon p0;
			p0.a.position = glm::normalize(poly.a.position) * 10.0f;
			p0.b.position = glm::normalize((poly.a.position + poly.b.position) / 2.0f) * 10.0f;
			p0.c.position = glm::normalize((poly.a.position + poly.c.position) / 2.0f) * 10.0f;
			p0.correctNormals();
			polys.push_back(p0);

			//printf(("{" + std::to_string(poly.a.position.x) + ", " + std::to_string(poly.a.position.y) + ", " + std::to_string(poly.a.position.z) + "}, ").c_str());
			//printf(("{" + std::to_string(poly.b.position.x) + ", " + std::to_string(poly.b.position.y) + ", " + std::to_string(poly.b.position.z) + "}, ").c_str());
			//printf(("{" + std::to_string(poly.c.position.x) + ", " + std::to_string(poly.c.position.y) + ", " + std::to_string(poly.c.position.z) + "},\n").c_str());

			LibVolume::Render::Structures::Polygon p1;
			p1.a.position = glm::normalize((poly.a.position + poly.b.position) / 2.0f) * 10.0f;
			p1.b.position = glm::normalize(poly.b.position) * 10.0f;
			p1.c.position = glm::normalize((poly.b.position + poly.c.position) / 2.0f) * 10.0f;
			p1.correctNormals();
			polys.push_back(p1);

			LibVolume::Render::Structures::Polygon p2;
			p2.a.position = glm::normalize((poly.a.position + poly.c.position) / 2.0f) * 10.0f;
			p2.b.position = glm::normalize((poly.b.position + poly.c.position) / 2.0f) * 10.0f;
			p2.c.position = glm::normalize(poly.c.position) * 10.0f;
			p2.correctNormals();
			polys.push_back(p2);

			LibVolume::Render::Structures::Polygon p3;
			p3.a.position = glm::normalize((poly.a.position + poly.b.position) / 2.0f) * 10.0f;
			p3.b.position = glm::normalize((poly.b.position + poly.c.position) / 2.0f) * 10.0f;
			p3.c.position = glm::normalize((poly.a.position + poly.c.position) / 2.0f) * 10.0f;
			p3.correctNormals();
			polys.push_back(p3);
		}

		planet.mesh->polygons.clear();
		planet.mesh->polygons = polys;
	}

	for (int c = 0; c < planet.mesh->polygons.size(); c ++)
	{
		LibVolume::Render::Structures::Polygon* poly = &planet.mesh->polygons[c];

		poly->a.position *= 0.1 + 0.01 * noise.getPerlin(glm::vec4(poly->a.position, 7.0) * 0.1f, -0.0, 3.0, 1.5);
		poly->b.position *= 0.1 + 0.01 * noise.getPerlin(glm::vec4(poly->b.position, 7.0) * 0.1f, -0.0, 3.0, 1.5);
		poly->c.position *= 0.1 + 0.01 * noise.getPerlin(glm::vec4(poly->c.position, 7.0) * 0.1f, -0.0, 3.0, 1.5);

		poly->a.colour *= 0.5 + 0.5 * noise.getPerlin(glm::vec4(poly->a.position, 1.0) * 5.0f, -0.0, 1.0, 1.5);
		poly->b.colour *= 0.5 + 0.5 * noise.getPerlin(glm::vec4(poly->b.position, 1.0) * 5.0f, -0.0, 1.0, 1.5);
		poly->c.colour *= 0.5 + 0.5 * noise.getPerlin(glm::vec4(poly->c.position, 1.0) * 5.0f, -0.0, 1.0, 1.5);
	}*/



	//Create a terrain object
	/*LibVolume::Engine::VoxelTerrain terrain(glm::vec3(8, 8, 8));
	terrain.state.scale = glm::vec3(50.0, 50.0, 50.0);
	terrain.state.position = glm::vec3(0.0, 0.0, -1000.0);

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

	realm.addObject(terrain);*/

	for (int count = 0; count < 8; count ++)
		addPlanet(&realm);

	LibVolume::Render::Structures::Light sun(LibVolume::Render::Structures::LightType::Directional, glm::vec3(0.5, 0.5, -1.0), glm::vec3(2.0, 2.0, 2.0), 0.01);
	realm.addLight(sun);

	LibVolume::Render::Structures::Light shiplight(LibVolume::Render::Structures::LightType::Point, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), 1000.0f, glm::vec3(1.0, 0.0, 0.0), 0.85);
	realm.addLight(shiplight);

	LibVolume::Render::Structures::Mesh lasermesh;
	lasermesh.loadFromOBJ("../laser.obj.test", false);
	lasermesh.colour = glm::vec3(0.0, 200.0, 50.0);
	long long time_since_last_shot = 0;

	bool fp = false;
	bool c = false;
	bool l = false;
	bool cl = false;

	//Run the window
	while (window.tick() == false)
	{
		if (window.event_manager.keyboard_state.key_space)
			ship.state.velocity += ship.state.orientation * glm::f64vec3(0.0, 0.1, 0.0);
		if (window.event_manager.keyboard_state.key_shift)
			ship.state.velocity += ship.state.orientation * glm::f64vec3(0.0, -0.1, 0.0);
		if (window.event_manager.keyboard_state.key_left)
			ship.state.velocity += ship.state.orientation * glm::f64vec3(-0.1, 0.0, 0.0);
		if (window.event_manager.keyboard_state.key_right)
			ship.state.velocity += ship.state.orientation * glm::f64vec3(0.1, 0.0, 0.0);
		if (window.event_manager.keyboard_state.key_up)
			ship.state.velocity += ship.state.orientation * glm::f64vec3(0.0, 0.0, -0.1);
		if (window.event_manager.keyboard_state.key_down)
			ship.state.velocity += ship.state.orientation * glm::f64vec3(0.0, 0.0, 0.1);

		if (window.event_manager.keyboard_state.key_w)
			ship.state.spin = (glm::f64quat(ship.state.orientation * glm::f64vec3(-0.003, 0.0, 0.0))) * ship.state.spin;
		if (window.event_manager.keyboard_state.key_a)
			ship.state.spin = (glm::f64quat(ship.state.orientation * glm::f64vec3(0.0, 0.003, 0.0))) * ship.state.spin;
		if (window.event_manager.keyboard_state.key_s)
			ship.state.spin = (glm::f64quat(ship.state.orientation * glm::f64vec3(0.003, 0.0, 0.0))) * ship.state.spin;
		if (window.event_manager.keyboard_state.key_d)
			ship.state.spin = (glm::f64quat(ship.state.orientation * glm::f64vec3(0.0, -0.003, 0.0))) * ship.state.spin;
		if (window.event_manager.keyboard_state.key_q)
			ship.state.spin = (glm::f64quat(ship.state.orientation * glm::f64vec3(0.0, 0.0, 0.003))) * ship.state.spin;
		if (window.event_manager.keyboard_state.key_e)
			ship.state.spin = (glm::f64quat(ship.state.orientation * glm::f64vec3(0.0, 0.0, -0.003))) * ship.state.spin;

		ship.state.spin = glm::mix(ship.state.spin, glm::f64quat(), 0.35f / glm::pow(3.0f * ship.state.spin.w, 2.0f) + 0.030f * ship.state.spin.w);

		glm::f64quat spn = ship.state.spin;
		//spn.w *= glm::pow(spn.w, 1.0);
		realm.camera.state.orientation = glm::inverse(glm::mix(spn, glm::f64quat(), 8.0) * ship.state.orientation);

		if (!fp)
			realm.camera.state.position = ship.state.position + ship.state.velocity + glm::f64vec3(0.0, 50.0, 300.0) * realm.camera.state.orientation;
		else
			realm.camera.state.position = ship.state.position + ship.state.velocity + glm::f64vec3(0.0, 0.0, 25.0) * realm.camera.state.orientation;

		if (window.event_manager.keyboard_state.key_f)
		{
			if (!c)
				fp = !fp;
			c = true;
		}
		else
			c = false;

		if (window.event_manager.keyboard_state.key_l)
		{
			if (!cl)
				l = !l;
			cl = true;
		}
		else
			cl = false;

		if (l)
			shiplight.colour = glm::mix(shiplight.colour, glm::vec3(5.0, 5.0, 5.0) * 8.0f, 0.1);
		else
			shiplight.colour = glm::mix(shiplight.colour, glm::vec3(0.0, 0.0, 0.0), 0.1);

		shiplight.position = ship.state.position + ship.state.velocity;
		shiplight.direct = glm::f64vec3(0.0, 0.0, -1.0) * realm.camera.state.orientation;

		if (window.event_manager.keyboard_state.key_enter && time_since_last_shot > 10)
		{
			LibVolume::Engine::Actor* laser = new LibVolume::Engine::Actor();
			laser->mesh = &lasermesh;
			laser->state.position = ship.state.position + ship.state.orientation * glm::f64vec3(50.0, 0.0, -100.0);
			laser->state.scale = glm::vec3(4.0, 4.0, 8.0);
			laser->state.orientation = ship.state.orientation;
			laser->state.velocity = ship.state.velocity + ship.state.orientation * glm::f64vec3(0.0, 0.0, -100.0);
			laser->timeout = 240;
			realm.addObject(*laser);

			laser = new LibVolume::Engine::Actor();
			laser->mesh = &lasermesh;
			laser->state.position = ship.state.position + ship.state.orientation * glm::f64vec3(-50.0, 0.0, -100.0);
			laser->state.scale = glm::vec3(4.0, 4.0, 8.0);
			laser->state.orientation = ship.state.orientation;
			laser->state.velocity = ship.state.velocity + ship.state.orientation * glm::f64vec3(0.0, 0.0, -100.0);
			laser->timeout = 240;
			realm.addObject(*laser);

			time_since_last_shot = 0;
		}

		//ship2.state.velocity += ship2.state.orientation * glm::vec3(0.0, 0.0, -0.1);

		double n = glm::min(0.05f, glm::abs((180.0f / 3.141f) * (float)acos(glm::dot(ship2.state.position - ship.state.position, ship2.state.orientation * glm::f64vec3(0.0, 0.0, -1.0)))));
		glm::f64quat p = glm::angleAxis(n, glm::normalize(glm::cross(ship2.state.position - ship.state.position, ship2.state.orientation * glm::f64vec3(0.0, 0.0, -1.0))));
		ship2.state.orientation = p * ship2.state.orientation;//glm::mix(glm::inverse(glm::toQuat(look)), ship2.state.orientation, 0.97f);

		time_since_last_shot ++;

		realm.tick();

		realm.render();
	};

	return 0;
}
