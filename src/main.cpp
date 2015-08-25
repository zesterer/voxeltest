#include "stdio.h"

#include "libvolume/data/voxelfield.h"
#include "libvolume/window/window.h"
#include "libvolume/common/io.h"

int main(int argc, char* argv[])
{
	LibVolume::IO::output("Starting VoxelTest");
	
	LibVolume::Window::Window window;
	window.activate(true);
	window.outputContextDebug();
	
	//Run the window
	while (window.tick() == false);
	
	return 0;
}
