#include "stdio.h"

#include "libvolume/data/voxelfield.h"
#include "libvolume/window/window.h"
#include "libvolume/common/io.h"

int main(int argc, char* argv[])
{
	LibVolume::IO::output("Starting VoxelTest");
	
	LibVolume::Data::VoxelField field(glm::ivec3(16, 16, 16));
	
	LibVolume::Data::Voxel* voxel = field.getAt(glm::ivec3(0, 0, 0));
	
	LibVolume::Window::Window window;
	window.outputContextDebug();
	
	while (window.tick() == false);
	
	return 0;
}
