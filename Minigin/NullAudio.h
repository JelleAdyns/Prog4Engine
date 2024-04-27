#ifndef NULLAUDIO_H
#define NULLAUDIO_H

#include "AudioService.h"

namespace dae
{
	class NullAudio final : public AudioService
	{
	public:
		NullAudio() = default;
		virtual ~NullAudio() = default;

		NullAudio(const NullAudio&) = delete;
		NullAudio(NullAudio&&) noexcept = delete;
		NullAudio& operator= (const NullAudio&) = delete;
		NullAudio& operator= (NullAudio&&) noexcept = delete;

		virtual void AddSound(const std::string&, SoundID) override {}
		virtual void PlaySoundClip(SoundID, uint8_t, bool) const override {}
		virtual uint8_t GetVolume(SoundID) const override { return 0; }
		virtual void SetVolume(SoundID, uint8_t) override{}
		virtual void PauseSound(SoundID) const override{}
		virtual void PauseAllSounds() const override{}
		virtual void ResumeSound(SoundID) const override{}
		virtual void ResumeAllSounds() const override{};
		virtual void StopSound(SoundID) const override {}
		virtual void StopAllSounds() const override {}
	};
}

#endif // !NULLAUDIO_H