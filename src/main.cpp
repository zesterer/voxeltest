#include "stdio.h"

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
	realm.setEventManager(&window.event_manager);
	LibVolume::Engine::Actor* actor = new LibVolume::Engine::Actor();

	actor->mesh.loadFromOBJ("../Link.obj.test");
	actor->state.position.y = -3.0;
	actor->state.scale = glm::vec3(1.0, 1.0, 1.0);
	actor->state.orientation = glm::quat(glm::vec3(0.0, 0.0, 0.0));
	//actor->state.spin = glm::quat(glm::vec3(0.0, 0.03, 0.0));

	realm.objects.push_back(dynamic_cast<LibVolume::Engine::Object*>(actor));

	//Run the window
	while (window.tick() == false)
	{
		realm.tick();
		realm.render();

		if (window.event_manager.keyboard_state.key_w)
			realm.camera.state.position += glm::vec3(0.0, 0.0, -0.3) * realm.camera.state.orientation;
		if (window.event_manager.keyboard_state.key_a)
			realm.camera.state.position += glm::vec3(-0.3, 0.0, 0.0) * realm.camera.state.orientation;
		if (window.event_manager.keyboard_state.key_s)
			realm.camera.state.position += glm::vec3(0.0, 0.0, 0.3) * realm.camera.state.orientation;
		if (window.event_manager.keyboard_state.key_d)
			realm.camera.state.position += glm::vec3(0.3, 0.0, 0.0) * realm.camera.state.orientation;
		if (window.event_manager.keyboard_state.key_space)
			realm.camera.state.position += glm::vec3(0.0, 0.3, 0.0) * realm.camera.state.orientation;
		if (window.event_manager.keyboard_state.key_shift)
			realm.camera.state.position += glm::vec3(0.0, -0.3, 0.0) * realm.camera.state.orientation;

		if (window.event_manager.keyboard_state.key_up)
			realm.camera.state.orientation = glm::quat(glm::vec3(0.03, 0.0, 0.0)) * realm.camera.state.orientation;
		if (window.event_manager.keyboard_state.key_left)
			realm.camera.state.orientation = glm::quat(glm::vec3(0.0, -0.03, 0.0)) * realm.camera.state.orientation;
		if (window.event_manager.keyboard_state.key_down)
			realm.camera.state.orientation = glm::quat(glm::vec3(-0.03, 0.0, 0.0)) * realm.camera.state.orientation;
		if (window.event_manager.keyboard_state.key_right)
			realm.camera.state.orientation = glm::quat(glm::vec3(0.0, 0.03, 0.0)) * realm.camera.state.orientation;
	};

	return 0;
}
