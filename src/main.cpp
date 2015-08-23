#include "stdio.h"

#include "libvolume/data/voxelfield.h"

int main(int argc, char* argv[])
{
	printf("Hello, World!\n");
	
	LibVolume::Data::VoxelField field(glm::ivec3(16, 16, 16));
	
	return 0;
}
