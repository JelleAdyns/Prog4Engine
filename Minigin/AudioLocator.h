#ifndef AUDIOLOCATOR_H
#define AUDIOLOCATOR_H

#include "AudioService.h"
#include "NullAudio.h"
#include <memory>

namespace dae
{
	class AudioLocator final
	{
	public:

		AudioLocator() = delete;

		AudioLocator(const AudioLocator&) = delete;
		AudioLocator(AudioLocator&&) noexcept = delete;
		AudioLocator& operator= (const AudioLocator&) = delete;
		AudioLocator& operator= (AudioLocator&&) noexcept = delete;

		static AudioService& GetAudioService() { return *m_Instance; }
		static void RegisterAudioService(std::unique_ptr<AudioService>&& audioService)
		{
			if (audioService == nullptr) m_Instance = std::make_unique<NullAudio>();
			else m_Instance = std::move(audioService);
		}
	private:
		static std::unique_ptr<AudioService> m_Instance;
	};

}

#endif // !AUDIOLOCATOR_H

