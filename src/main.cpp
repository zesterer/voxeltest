#include "stdio.h"

#include "libvolume/data/voxelfield.h"
#include "libvolume/window/window.h"
#include "libvolume/common/io.h"
#include "libvolume/engine/realm.h"
#include "libvolume/engine/actor.h"

int main(int argc, char* argv[])
{
	LibVolume::IO::output("Starting VoxelTest");
	
	LibVolume::Window::Window window;
	window.activate(true);
	window.outputContextDebug();
	
	LibVolume::Engine::Realm realm;
	LibVolume::Engine::Actor* actor = new LibVolume::Engine::Actor();
	
	actor->mesh.loadFromOBJ("../Dugtrio.obj.test");
	
	//actor.state.scale = glm::vec3(100.0, 100.0, 100.0);
	
	realm.objects.push_back(dynamic_cast<LibVolume::Engine::Object*>(actor));
	
	//Run the window
	while (window.tick() == false)
	{
		realm.tick();
		realm.render();
		//realm.renderer.renderActor(&actor);
	};
	
	return 0;
}
