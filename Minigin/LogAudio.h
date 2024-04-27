#ifndef LOGAUDIO_H
#define LOGAUDIO_H

#include "AudioService.h"
#include <memory>
#include <iostream>

namespace dae
{

	class LogAudio final : public AudioService
	{
	public:

		LogAudio(std::unique_ptr<AudioService>&& pService) :
			m_pRealService{ std::move(pService) }
		{}
		virtual ~LogAudio() = default;

		LogAudio(const LogAudio&) = delete;
		LogAudio(LogAudio&&) noexcept = delete;
		LogAudio& operator= (const LogAudio&) = delete;
		LogAudio& operator= (LogAudio&&) noexcept = delete;

		virtual void AddSound(const std::string& filename, SoundID id) override
		{
			m_pRealService->AddSound(filename, id);
			std::cout << "AddSound: filename: " << filename << ", id: " << id << '\n';
		}
		virtual void PlaySoundClip(SoundID id, uint8_t volume, bool repeat) const override
		{
			m_pRealService->PlaySoundClip(id, volume, repeat);
			std::cout << "PlaySoundClip: id: " << id << ", volume: " << static_cast<int>(volume) << ", repeat: " << std::boolalpha << repeat << '\n';
		}
		virtual uint8_t GetVolume(SoundID id) const override
		{
			uint8_t volume = m_pRealService->GetVolume(id);
			std::cout << "GetVolume: id: " << id << ", returned volume: " << static_cast<int>(volume) << '\n';
			return volume;
		}
		virtual void SetVolume(SoundID id, uint8_t newVolume) override
		{
			m_pRealService->SetVolume(id, newVolume);
			std::cout << "SetVolume: id: " << id << ", new volume: " << static_cast<int>(newVolume) << '\n';
		}
		virtual void PauseSound(SoundID id) const override
		{
			m_pRealService->PauseSound(id);
			std::cout << "PauseSound: id: " << id << '\n';
		}
		virtual void PauseAllSounds() const override
		{
			m_pRealService->PauseAllSounds();
			std::cout << "PauseAllSounds\n";
		}
		virtual void ResumeSound(SoundID id) const override
		{
			m_pRealService->ResumeSound(id);
			std::cout << "ResumeSound: id: " << id << '\n';
		}
		virtual void ResumeAllSounds() const override
		{
			m_pRealService->ResumeAllSounds();
			std::cout << "ResumeAllSounds\n";
		}
		virtual void StopSound(SoundID id) const override
		{
			m_pRealService->StopSound(id);
			std::cout << "StopSound: id: " << id << '\n';
		}
		virtual void StopAllSounds() const override
		{
			m_pRealService->StopAllSounds();
			std::cout << "StopAllSounds\n";
		}
	private:

		std::unique_ptr<AudioService> m_pRealService;
	};
}

#endif // !LOGAUDIO_H