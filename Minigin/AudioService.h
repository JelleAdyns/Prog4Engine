#ifndef AUDIOSERVICE_H
#define AUDIOSERVICE_H

#include <string>
namespace dae
{
	using SoundID = unsigned int;

	class AudioService
	{
	public:
		AudioService() = default;
		virtual ~AudioService() = default;

		AudioService(const AudioService&) = delete;
		AudioService(AudioService&&) noexcept = delete;
		AudioService& operator= (const AudioService&) = delete;
		AudioService& operator= (AudioService&&) noexcept = delete;

		virtual void AddSound(const std::string& filename, SoundID id) = 0;
		virtual void PlaySoundClip(SoundID id, uint8_t volume, bool repeat) const = 0;
		virtual uint8_t GetVolume(SoundID id) const = 0;
		virtual void SetVolume(SoundID id, uint8_t newVolume) = 0;
		virtual void SetMasterVolume(uint8_t newVolume) = 0;
		virtual void PauseSound(SoundID id) const = 0;
		virtual void PauseAllSounds() const = 0;
		virtual void ResumeSound(SoundID id) const = 0;
		virtual void ResumeAllSounds() const = 0;
		virtual void StopSound(SoundID id) const = 0;
		virtual void StopAllSounds() const = 0;

	};
}

#endif // !AUDIOSERVICE_H
