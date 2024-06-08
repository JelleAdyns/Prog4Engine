#ifndef SDLAUDIO_H
#define SDLAUDIO_H

#include "AudioService.h"

namespace dae
{

	class SDLAudio final : public AudioService
	{
	public:

		SDLAudio();
		virtual ~SDLAudio();

		SDLAudio(const SDLAudio&) = delete;
		SDLAudio(SDLAudio&&) noexcept = delete;
		SDLAudio& operator= (const SDLAudio&) = delete;
		SDLAudio& operator= (SDLAudio&&) noexcept = delete;

		virtual void AddSound(const std::string& filename, SoundID id) override;
		virtual void PlaySoundClip(SoundID id, uint8_t volume, bool repeat) const override;
		virtual uint8_t GetVolume(SoundID id) const override;
		virtual void SetVolume(SoundID id, uint8_t newVolume) override;
		virtual void SetMasterVolume(uint8_t newVolume) override;
		virtual void PauseSound(SoundID id) const override;
		virtual void PauseAllSounds() const override;
		virtual void ResumeSound(SoundID id) const override;
		virtual void ResumeAllSounds() const override;
		virtual void StopSound(SoundID id) const override;
		virtual void StopAllSounds() const override;
	private:

		class SDLAudioImpl;
		SDLAudioImpl* m_pImpl;
	};
}

#endif // !SDLAUDIO_H
