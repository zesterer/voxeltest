#include "stdio.h"

#include "libvolume/data/voxelfield.h"
#include "libvolume/window/window.h"
#include "libvolume/common/io.h"
#include "libvolume/engine/realm.h"

int main(int argc, char* argv[])
{
	LibVolume::IO::output("Starting VoxelTest");
	
	LibVolume::Window::Window window;
	window.activate(true);
	window.outputContextDebug();
	
	LibVolume::Engine::Realm realm;
	
	//Run the window
	while (window.tick() == false)
	{
		realm.tick();
		realm.render();
	};
	
	return 0;
}
