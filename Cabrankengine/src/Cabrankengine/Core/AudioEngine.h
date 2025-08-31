#pragma once

#include <string>
#include <irrKlang.h>
#include <Cabrankengine/Core/Core.h>

namespace cabrankengine {

	// Custom deleter for IrrKlang as we must destroy the instance with drop()
	struct IrrKlangDeleter {
		void operator()(irrklang::ISoundEngine* engine) const {
			if (engine) engine->drop();
		}
	};

	class AudioEngine {
		public:
			// Initializes the audio engine
			static void init();

			static void playAudio(const std::string& path, bool looped);

		private:
			static Scope<irrklang::ISoundEngine, IrrKlangDeleter> s_SoundEngine;
	};
}
