#include "CabrankEngine.h"

using namespace cabrankengine;

int CabrankEngine::init()
{
	// Initialize all SDL systems
	SDL_Init(SDL_INIT_EVERYTHING);

	// Alternate buffers for more speed
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	return 0;
}