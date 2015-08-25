//----LOCAL----
#include "voxelfield.h"

namespace LibVolume
{
	namespace Data
	{
		VoxelField::VoxelField(glm::ivec3 size)
		{
			this->size = size;
		}
		
		Voxel* VoxelField::getAt(glm::ivec3 pos)
		{
			if (pos.x < 0 || pos.x >= this->size.x)
				return &this->empty;
			if (pos.y < 0 || pos.y >= this->size.y)
				return &this->empty;
			if (pos.z < 0 || pos.z >= this->size.z)
				return &this->empty;

			return &this->voxels[this->size.y * this->size.z * pos.x + this->size.z * pos.y + pos.z];
		}
	}
}
