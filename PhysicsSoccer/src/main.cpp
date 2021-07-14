#include "PhysicsAppApp.h"

int main() {
	
	// allocation
	auto app = new PhysicsAppApp();

	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}