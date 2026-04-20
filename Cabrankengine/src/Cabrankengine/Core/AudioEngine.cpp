#include <pch.h>
#include "AudioEngine.h"

using namespace irrklang;

namespace cbk {

	Scope<ISoundEngine, IrrKlangDeleter> AudioEngine::s_SoundEngine = nullptr;

	void AudioEngine::init() {
		CBK_PROFILE_FUNCTION();
		CBK_CORE_ASSERT(!s_SoundEngine, "The Audio Engine cannot be initialized twice!");

		if (!s_SoundEngine) {
			s_SoundEngine = Scope<ISoundEngine, IrrKlangDeleter>(createIrrKlangDevice());

			if (!s_SoundEngine)
				CBK_CORE_ERROR("Failed to initialize the Audio Engine!");
			else
				CBK_CORE_INFO("Audio Engine initialized successfully!");
		}
	}

	void AudioEngine::playAudio(const std::string& path, bool looped) {
		CBK_PROFILE_FUNCTION();
		s_SoundEngine->play2D(path.c_str(), looped);
	}
}
