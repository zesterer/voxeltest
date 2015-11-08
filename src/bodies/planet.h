//----LIBRARY----
#include "libvolume/engine/actor.h"
#include "libvolume/render/structures/vertex.h"

namespace Vast
{
	namespace Bodies
	{
		class Planet;

		struct TerrainPart
		{
			public:
				LibVolume::Render::Structures::Polygon poly;

				TerrainPart* child[4] = {nullptr, nullptr, nullptr, nullptr};

				int depth = 0;
				bool has_split = false;

				TerrainPart* parent = nullptr;
				Planet* owner = nullptr;

				~TerrainPart();
				bool split();
				bool join();
				bool update(glm::vec3 camera_pos);
		};

		class Planet : public LibVolume::Engine::Actor
		{
			public:
				TerrainPart parts[20];

				int max_depth = 11;

				Planet();
				void tick() override;
				void collide(LibVolume::Engine::Entity& other) override;
				void makeMesh(std::vector<LibVolume::Render::Structures::Polygon>* poly_vector, TerrainPart* part);
				float getHeightAt(glm::vec3 pos);
				float getDistanceAt(glm::vec3 pos);
				glm::vec3 getPositionAt(glm::vec3 pos);
				glm::vec3 getNormalAt(glm::vec3 pos, int detail = 0);
		};
	}
}
