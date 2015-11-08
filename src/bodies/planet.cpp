//----LIBRARY----
#include "libvolume/engine/realm.h"
#include "libvolume/common/io.h"
#include "libvolume/generation/perlin.h"

//----LOCAL----
#include "planet.h"

namespace Vast
{
	namespace Bodies
	{
        Planet::Planet()
        {
        	const glm::vec3 parts[20 * 3] =
        	{
        		{0.000000, -1.000000, 0.000000}, {0.723600, -0.447215, 0.525720}, {-0.276385, -0.447215, 0.850640},
				{0.723600, -0.447215, 0.525720}, {0.000000, -1.000000, 0.000000}, {0.723600, -0.447215, -0.525720},
				{0.000000, -1.000000, 0.000000}, {-0.276385, -0.447215, 0.850640}, {-0.894425, -0.447215, 0.000000},
				{0.000000, -1.000000, 0.000000}, {-0.894425, -0.447215, 0.000000}, {-0.276385, -0.447215, -0.850640},
				{0.000000, -1.000000, 0.000000}, {-0.276385, -0.447215, -0.850640}, {0.723600, -0.447215, -0.525720},
				{0.723600, -0.447215, 0.525720}, {0.723600, -0.447215, -0.525720}, {0.894425, 0.447215, 0.000000},
				{-0.276385, -0.447215, 0.850640}, {0.723600, -0.447215, 0.525720}, {0.276385, 0.447215, 0.850640},
				{-0.894425, -0.447215, 0.000000}, {-0.276385, -0.447215, 0.850640}, {-0.723600, 0.447215, 0.525720},
				{-0.276385, -0.447215, -0.850640}, {-0.894425, -0.447215, 0.000000}, {-0.723600, 0.447215, -0.525720},
				{0.723600, -0.447215, -0.525720}, {-0.276385, -0.447215, -0.850640}, {0.276385, 0.447215, -0.850640},
				{0.723600, -0.447215, 0.525720}, {0.894425, 0.447215, 0.000000}, {0.276385, 0.447215, 0.850640},
				{-0.276385, -0.447215, 0.850640}, {0.276385, 0.447215, 0.850640}, {-0.723600, 0.447215, 0.525720},
				{-0.894425, -0.447215, 0.000000}, {-0.723600, 0.447215, 0.525720}, {-0.723600, 0.447215, -0.525720},
				{-0.276385, -0.447215, -0.850640}, {-0.723600, 0.447215, -0.525720}, {0.276385, 0.447215, -0.850640},
				{0.723600, -0.447215, -0.525720}, {0.276385, 0.447215, -0.850640}, {0.894425, 0.447215, 0.000000},
				{0.276385, 0.447215, 0.850640}, {0.894425, 0.447215, 0.000000}, {0.000000, 1.000000, 0.000000},
				{-0.723600, 0.447215, 0.525720}, {0.276385, 0.447215, 0.850640}, {0.000000, 1.000000, 0.000000},
				{-0.723600, 0.447215, -0.525720}, {-0.723600, 0.447215, 0.525720}, {0.000000, 1.000000, 0.000000},
				{0.276385, 0.447215, -0.850640}, {-0.723600, 0.447215, -0.525720}, {0.000000, 1.000000, 0.000000},
				{0.894425, 0.447215, 0.000000}, {0.276385, 0.447215, -0.850640}, {0.000000, 1.000000, 0.000000},
        	};

        	for (int c = 0; c < 20; c ++)
			{
				this->parts[c].poly.a.position = parts[c * 3 + 0];
				this->parts[c].poly.b.position = parts[c * 3 + 1];
				this->parts[c].poly.c.position = parts[c * 3 + 2];
				this->parts[c].owner = this;

				this->parts[c].poly.a.normal = glm::normalize(this->parts[c].poly.a.position);
				this->parts[c].poly.b.normal = glm::normalize(this->parts[c].poly.b.position);
				this->parts[c].poly.c.normal = glm::normalize(this->parts[c].poly.c.position);

				//this->parts[c].poly.correctNormals();
			}
        }

        void Planet::tick()
        {
        	LibVolume::Engine::Actor::tick();

			this->state.update();
        	glm::f64mat4 inv = glm::inverse(this->state.matrix * this->mesh_state.matrix);

        	bool should_remesh = false;

        	glm::vec3 camera_relative = (glm::vec3)(inv * glm::f64vec4(this->parent->camera.state.position, 1.0));

			if (this->parent->time % 10 == 0)
			{
				for (int c = 0; c < 20; c ++)
				{
					should_remesh |= this->parts[c].update(camera_relative);
				}

				if (should_remesh)
				{
					this->mesh->clear();

					for (int c = 0; c < 20; c ++)
					{
						this->makeMesh(&this->mesh->polygons, &this->parts[c]);
					}
				}
			}

			this->parent->background_colour = glm::mix(this->parent->background_colour, glm::mix(glm::vec3(2.0, 2.0, 6.0), glm::vec3(0.0, 0.0, 0.0), glm::max(0.0f, glm::min(1.0f, glm::pow(glm::length(camera_relative), 3.0f) * 5.0f))), 0.01);

			float light_factor = glm::max(0.0f, glm::dot(glm::normalize(camera_relative), glm::normalize(-glm::vec3(0.5, 0.5, -1.0))));

			if (glm::length(camera_relative) >= 1.1f)
			{
				this->parent->camera.fog_distance = 0.0f;
				this->parent->background_colour = glm::vec3(0.0, 0.0, 0.0);
			}
			else
			{
				this->parent->camera.fog_distance = 20000.0f / (1.1f - glm::length(camera_relative));
				this->parent->background_colour = glm::mix(glm::vec3(0.0, 0.0, 0.0), light_factor * glm::vec3(2.0, 2.0, 6.0), -5.0f * (glm::length(camera_relative) - 1.1f));
			}
        }

		TerrainPart::~TerrainPart()
		{
			delete this->child[0];
			delete this->child[1];
        	delete this->child[2];
        	delete this->child[3];
		}

        void Planet::makeMesh(std::vector<LibVolume::Render::Structures::Polygon>* poly_vector, TerrainPart* part)
        {
        	if (!part->has_split)
				poly_vector->push_back(part->poly);

			if (part->has_split)
			{
				this->makeMesh(poly_vector, part->child[0]);
				this->makeMesh(poly_vector, part->child[1]);
				this->makeMesh(poly_vector, part->child[2]);
				this->makeMesh(poly_vector, part->child[3]);
			}
        }

        bool TerrainPart::split()
        {
        	if (!this->has_split && this->owner->max_depth > this->depth)
			{
				this->has_split = true;

				for (int c = 0; c < 4; c ++)
				{
					this->child[c] = new TerrainPart();
					this->child[c]->poly = this->poly;
					this->child[c]->depth = this->depth + 1;
					this->child[c]->parent = this;
					this->child[c]->owner = this->owner;
				}


				this->child[0]->poly.a.position = (this->poly.a.position + this->poly.a.position) / 2.0f;
				this->child[0]->poly.b.position = (this->poly.a.position + this->poly.b.position) / 2.0f;
				this->child[0]->poly.c.position = (this->poly.a.position + this->poly.c.position) / 2.0f;

				this->child[1]->poly.a.position = (this->poly.a.position + this->poly.b.position) / 2.0f;
				this->child[1]->poly.b.position = (this->poly.b.position + this->poly.b.position) / 2.0f;
				this->child[1]->poly.c.position = (this->poly.b.position + this->poly.c.position) / 2.0f;

				this->child[2]->poly.a.position = (this->poly.a.position + this->poly.c.position) / 2.0f;
				this->child[2]->poly.b.position = (this->poly.b.position + this->poly.c.position) / 2.0f;
				this->child[2]->poly.c.position = (this->poly.c.position + this->poly.c.position) / 2.0f;

				this->child[3]->poly.a.position = (this->poly.a.position + this->poly.b.position) / 2.0f;
				this->child[3]->poly.b.position = (this->poly.b.position + this->poly.c.position) / 2.0f;
				this->child[3]->poly.c.position = (this->poly.a.position + this->poly.c.position) / 2.0f;

				for (int c = 0; c < 4; c ++)
				{
					LibVolume::Generation::PerlinNoise noise;

					this->child[c]->poly.a.position = this->owner->getPositionAt(this->child[c]->poly.a.position);
					this->child[c]->poly.b.position = this->owner->getPositionAt(this->child[c]->poly.b.position);
					this->child[c]->poly.c.position = this->owner->getPositionAt(this->child[c]->poly.c.position);

					//this->child[c]->poly.a.normal = glm::normalize(this->child[c]->poly.a.position);
					//this->child[c]->poly.b.normal = glm::normalize(this->child[c]->poly.b.position);
					//this->child[c]->poly.c.normal = glm::normalize(this->child[c]->poly.c.position);

					this->child[c]->poly.a.normal = this->owner->getNormalAt(this->child[c]->poly.a.position, this->depth);
					this->child[c]->poly.b.normal = this->owner->getNormalAt(this->child[c]->poly.b.position, this->depth);
					this->child[c]->poly.c.normal = this->owner->getNormalAt(this->child[c]->poly.c.position, this->depth);

					/*if (this->owner->getHeightAt(this->child[c]->poly.a.position) < 0.999f)
						this->child[c]->poly.a.colour = glm::vec3(0.3, 0.6, 1.0);
					if (this->owner->getHeightAt(this->child[c]->poly.b.position) < 0.999f)
						this->child[c]->poly.b.colour = glm::vec3(0.3, 0.6, 1.0);
					if (this->owner->getHeightAt(this->child[c]->poly.c.position) < 0.999f)
						this->child[c]->poly.c.colour = glm::vec3(0.3, 0.6, 1.0);*/

					//this->child[c]->poly.correctNormals();

					//this->child[c]->poly.a.normal *= 0.001f;// = glm::normalize(this->child[c]->poly.a.position);
					//this->child[c]->poly.b.normal *= 0.001f;// = glm::normalize(this->child[c]->poly.b.position);
					//this->child[c]->poly.c.normal *= 0.001f;// = glm::normalize(this->child[c]->poly.c.position);

					//this->child[c]->poly.correctNormals();

					//glm::vec3 avg = (this->child[c]->poly.a.normal + this->child[c]->poly.b.normal + this->child[c]->poly.c.normal) / 3.0f;

					/*if (glm::dot(avg, va) < 0.8)
						this->child[c]->poly.a.normal = va;

					if (glm::dot(avg, vb) < 0.8)
						this->child[c]->poly.b.normal = vb;

					if (glm::dot(avg, vc) < 0.8)
						this->child[c]->poly.c.normal = vc;*/

					/*this->child[c]->poly.a.normal = glm::mix(v, this->child[c]->poly.a.normal, glm::pow(glm::dot(avg, v) - 0.8f, 1.0));
					this->child[c]->poly.b.normal = glm::mix(v, this->child[c]->poly.b.normal, glm::pow(glm::dot(avg, v) - 0.8f, 1.0));
					this->child[c]->poly.c.normal = glm::mix(v, this->child[c]->poly.c.normal, glm::pow(glm::dot(avg, v) - 0.8f, 1.0));

					this->child[c]->poly.a.normal = glm::normalize(glm::cross(this->owner->getPositionAt(glm::mix(this->child[c]->poly.a.position, this->child[c]->poly.b.position, 0.1f)) - this->child[c]->poly.a.position, this->owner->getPositionAt(glm::mix(this->child[c]->poly.a.position, this->child[c]->poly.c.position, 0.1f)) - this->child[c]->poly.a.position));
                    if (glm::dot(this->child[c]->poly.a.normal, glm::normalize(this->child[c]->poly.a.position)) < 0.0)
						this->child[c]->poly.a.normal *= -1;

					this->child[c]->poly.b.normal = glm::normalize(glm::cross(this->owner->getPositionAt(glm::mix(this->child[c]->poly.b.position, this->child[c]->poly.c.position, 0.1f)) - this->child[c]->poly.b.position, this->owner->getPositionAt(glm::mix(this->child[c]->poly.b.position, this->child[c]->poly.a.position, 0.1f)) - this->child[c]->poly.b.position));
                    if (glm::dot(this->child[c]->poly.b.normal, glm::normalize(this->child[c]->poly.b.position)) < 0.0)
						this->child[c]->poly.b.normal *= -1;

					this->child[c]->poly.c.normal = glm::normalize(glm::cross(this->owner->getPositionAt(glm::mix(this->child[c]->poly.c.position, this->child[c]->poly.a.position, 0.1f)) - this->child[c]->poly.c.position, this->owner->getPositionAt(glm::mix(this->child[c]->poly.c.position, this->child[c]->poly.b.position, 0.1f)) - this->child[c]->poly.c.position));
                    if (glm::dot(this->child[c]->poly.c.normal, glm::normalize(this->child[c]->poly.c.position)) < 0.0)
						this->child[c]->poly.c.normal *= -1;*/
				}

				return true;
			}

			return false;
        }

        float Planet::getHeightAt(glm::vec3 pos)
        {
        	return 1.0 + 0.005 * LibVolume::Generation::PerlinNoise::getPerlin(glm::vec4(pos, 7.0) * 512.0f, -3.5, 2.0, 2.0);
        }

        float Planet::getDistanceAt(glm::vec3 pos)
        {
        	return glm::length(pos) - this->getHeightAt(pos);
        }

        glm::vec3 Planet::getNormalAt(glm::vec3 pos, int detail)
        {
        	//LibVolume::IO::output("Hello. " + std::to_string(detail));

        	float multiplier = 0.0001f;
        	return glm::mix(glm::normalize(pos), glm::normalize(glm::vec3(
							this->getDistanceAt(pos + glm::vec3(+0.01, +0.00, +0.00) * multiplier) - this->getDistanceAt(pos + glm::vec3(-0.01, +0.00, +0.00) * multiplier),
							this->getDistanceAt(pos + glm::vec3(+0.00, +0.01, +0.00) * multiplier) - this->getDistanceAt(pos + glm::vec3(+0.00, -0.01, +0.00) * multiplier),
							this->getDistanceAt(pos + glm::vec3(+0.00, +0.00, +0.01) * multiplier) - this->getDistanceAt(pos + glm::vec3(+0.00, +0.00, -0.01) * multiplier)
							)), glm::max(0.0f, 1.0f - 1.0f / glm::max(0.0f, glm::pow(glm::max(0.0f, detail - 3.5f), 0.5f))));
        }

        glm::vec3 Planet::getPositionAt(glm::vec3 pos)
        {
        	return glm::normalize(pos) * this->getHeightAt(pos);
        }

        bool TerrainPart::join()
        {
        	if (this->has_split)
			{
				this->has_split = false;

				for (int c = 0; c < 4; c ++)
				{
					delete this->child[c];
					this->child[c] = nullptr;
				}

				return true;
			}

			return false;
        }

        bool TerrainPart::update(glm::vec3 camera_pos)
        {
        	bool result = false;

        	glm::vec3 average = (this->poly.a.position + this->poly.b.position + this->poly.c.position) / 3.0f;

			float detail = glm::max(7.0f, 0.55f * (glm::sqrt(glm::length(average - camera_pos)) - 0.13f) * glm::pow(2.0f, (float)this->depth));
        	if (detail < 8.0f)
				result |= this->split();
			else if (detail > 6.0f)
				result |= this->join();

        	if (this->has_split)
			{
				for (int c = 0; c < 4; c ++)
				{
					result |= this->child[c]->update(camera_pos);
				}
			}

			return result;
        }

        void Planet::collide(LibVolume::Engine::Entity& other)
        {
        	glm::f64mat4 inv = glm::inverse(this->state.matrix * this->mesh_state.matrix);
        	glm::vec3 other_relative = (glm::vec3)(inv * glm::f64vec4(other.state.position, 1.0));

        	glm::vec3 height = this->getPositionAt(other_relative);

        	if (glm::length(other_relative) < glm::length(height))
			{
				other.state.position = glm::f64vec3(this->state.matrix * this->mesh_state.matrix * glm::f64vec4(height, 1.0));
				//if (glm::length(other.state.velocity) > 0.0)
					//other.state.velocity = glm::reflect(other.state.velocity, glm::f64vec3(height)) * 0.3;
			}
        }
	}
}
