#include <Cabrankengine/Core/AudioEngine.h>

#include <Cabrankengine/Core/Logger.h>
#include <Cabrankengine/Debug/Instrumentator.h>

using namespace irrklang;

namespace cabrankengine {

	Scope<ISoundEngine, IrrKlangDeleter> AudioEngine::s_SoundEngine = nullptr;

	void AudioEngine::init() {
		CE_PROFILE_FUNCTION();
		CE_CORE_ASSERT(!s_SoundEngine, "The Audio Engine cannot be initialized twice!");

		if (!s_SoundEngine) {
			s_SoundEngine = Scope<ISoundEngine, IrrKlangDeleter>(createIrrKlangDevice());

			if (!s_SoundEngine)
				CE_CORE_ERROR("Failed to initialize the Audio Engine!");
			else
				CE_CORE_INFO("Audio Engine initialized successfully!");
		}
	}

	void AudioEngine::playAudio(const std::string& path, bool looped) {
		CE_PROFILE_FUNCTION();
		s_SoundEngine->play2D(path.c_str(), looped);
	}
}