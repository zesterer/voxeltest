#ifndef LIBVOLUME_DATA_VOXELFIELD_H
#define LIBVOLUME_DATA_VOXELFIELD_H

//----STANDARD----
#include "vector"

//----LIBRARY----
#include "glm/glm.hpp"
#include "glm/vec3.hpp"

//----LOCAL----
#include "voxel.h"

namespace LibVolume
{
	namespace Data
	{
		class VoxelField
		{
			public:
				//The field dimensions
				glm::ivec3 size;
			
				//The voxel data
				std::vector<Voxel> voxels;
				
				//An empty voxel, used for meshing
				Voxel empty;
				
				VoxelField(glm::ivec3 size);
				//Find the voxel at the specified position within the field
				Voxel* getAt(glm::ivec3 pos);
		};
	}
}

#endif
