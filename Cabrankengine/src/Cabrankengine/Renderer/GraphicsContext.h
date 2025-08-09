#pragma once

namespace cabrankengine {

	// GraphicsContext is an abstract class that defines the interface for a graphics context.
	class GraphicsContext {
		public:
			// Initializes the graphics context.
			virtual void init() = 0;

			// Buffer swapping is the process of presenting the rendered image to the screen
			// while also preparing the next frame for rendering.
			virtual void swapBuffers() = 0;
	};
}
