#include "SDLAudio.h"
#include <SDL_mixer.h>
#include <map>
#include <queue>
#include <memory>
#include <functional>
#include <thread>
#include "ResourceManager.h"
#include <mutex>
#include <iostream>

namespace dae
{
	//Implementation
	class SDLAudio::SDLAudioImpl
	{
	public:
		SDLAudioImpl() { 
			m_Thread = std::jthread{ &SDLAudio::SDLAudioImpl::HandleRequests, this};
			Mix_AllocateChannels(20);
		}
		~SDLAudioImpl() {m_ServiceIsActive = false;}

		SDLAudioImpl(const SDLAudioImpl&) = delete;
		SDLAudioImpl(SDLAudioImpl&&) noexcept = delete;
		SDLAudioImpl& operator= (const SDLAudioImpl&) = delete;
		SDLAudioImpl& operator= (SDLAudioImpl&&) noexcept = delete;

		void AddSoundImpl(const std::string& filename, SoundID id);
		void PlaySoundClipImpl(SoundID id, uint8_t volume, bool repeat);
		uint8_t GetVolumeImpl(SoundID id) const;
		void SetVolumeImpl(SoundID id, uint8_t newVolume);
		void SetMasterVolumeImpl(uint8_t newVolume);
		void ToggleMuteImpl();
		void PauseSoundImpl(SoundID id) const;
		void PauseAllSoundsImpl() const;
		void ResumeSoundImpl(SoundID id) const;
		void ResumeAllSoundsImpl() const;
		void StopSoundImpl(SoundID id) const;
		void StopAllSoundsImpl() const;
	private:

		// I've got this idea of this Deleter from Wout Firlefyn
		struct ChunkDeleter
		{
			void operator()(Mix_Chunk* pMusic) { Mix_FreeChunk(pMusic); }
		};

		struct AudioClip
		{
			std::unique_ptr<Mix_Chunk, ChunkDeleter> pMixChunk;
			std::string filename;
		};
		struct AudioInfo
		{
			SoundID id;
			uint8_t volume;
			bool repeat;
		};
		std::map<SoundID, AudioClip> m_pMapMusicClips{};
		std::queue<AudioInfo> m_Events{};

		void HandleRequests();

		bool m_ServiceIsActive{ true };
		bool m_IsMute{ false };
		std::string m_DataPath{ ResourceManager::GetInstance().GetDataPath() };

		std::jthread m_Thread;
		std::mutex m_EventsMutex;
		mutable std::mutex m_MapMutex;
	};


	void SDLAudio::SDLAudioImpl::AddSoundImpl(const std::string& filename, SoundID id)
	{
		std::lock_guard<std::mutex> mapLock{m_MapMutex};
		m_pMapMusicClips[id].filename = m_DataPath + filename;
	}
	void SDLAudio::SDLAudioImpl::PlaySoundClipImpl(SoundID id, uint8_t volume, bool repeat)
	{
		if(!m_IsMute)
		{
			std::unique_lock<std::mutex> mapLock{ m_MapMutex };
			if (m_pMapMusicClips.contains(id))
			{
				mapLock.unlock();

				AudioInfo info{};
				info.id = id;
				info.volume = volume;
				info.repeat = repeat;

				std::lock_guard<std::mutex> eventsLock{ m_EventsMutex };
				m_Events.push(info);
			}
			else throw std::runtime_error("SoundID has not been added yet.\n");
		}
		
	}
	uint8_t SDLAudio::SDLAudioImpl::GetVolumeImpl(SoundID id) const
	{
		std::lock_guard<std::mutex> mapLock{ m_MapMutex };
		if (m_pMapMusicClips.at(id).pMixChunk != nullptr) return static_cast<uint8_t>( Mix_Volume(id, -1) );
		return 0;
	}
	void SDLAudio::SDLAudioImpl::SetVolumeImpl(SoundID id, uint8_t newVolume)
	{
		std::lock_guard<std::mutex> mapLock{ m_MapMutex };
		if (m_pMapMusicClips.at(id).pMixChunk != nullptr) Mix_Volume(id, newVolume);
	}
	void SDLAudio::SDLAudioImpl::SetMasterVolumeImpl(uint8_t newVolume)
	{
		std::lock_guard<std::mutex> mapLock{ m_MapMutex };
		Mix_Volume(-1, newVolume);
	}
	void SDLAudio::SDLAudioImpl::ToggleMuteImpl()
	{
		m_IsMute = !m_IsMute;
		if (m_IsMute) PauseAllSoundsImpl();
		else ResumeAllSoundsImpl();
	}
	void SDLAudio::SDLAudioImpl::PauseSoundImpl(SoundID id) const
	{
		std::lock_guard<std::mutex> mapLock{ m_MapMutex };
		Mix_Pause(id);
	}
	void SDLAudio::SDLAudioImpl::PauseAllSoundsImpl() const
	{
		std::lock_guard<std::mutex> mapLock{ m_MapMutex };
		Mix_Pause(-1);
	}
	void SDLAudio::SDLAudioImpl::ResumeSoundImpl(SoundID id) const
	{
		std::lock_guard<std::mutex> mapLock{ m_MapMutex };
		Mix_Resume(id);
	}
	void SDLAudio::SDLAudioImpl::ResumeAllSoundsImpl() const
	{
		std::lock_guard<std::mutex> mapLock{ m_MapMutex };
		Mix_Resume(-1);
	}
	void SDLAudio::SDLAudioImpl::StopSoundImpl(SoundID id) const
	{
		std::lock_guard<std::mutex> mapLock{ m_MapMutex };
		Mix_HaltChannel(id);
	}
	void SDLAudio::SDLAudioImpl::StopAllSoundsImpl() const
	{
		std::lock_guard<std::mutex> mapLock{ m_MapMutex };
		Mix_HaltChannel(-1);
	}

	void SDLAudio::SDLAudioImpl::HandleRequests()
	{
		while (m_ServiceIsActive)
		{
			////////////
			// Events lock
			std::unique_lock<std::mutex> eventsLock{ m_EventsMutex };

			if (m_Events.empty())
			{
				eventsLock.unlock();
				continue;
			}
			AudioInfo info = m_Events.front();
			m_Events.pop();

			eventsLock.unlock();
			////////////

			SoundID id = info.id;

			////////////
			// Map lock
			std::unique_lock<std::mutex> mapLock{ m_MapMutex };

			if (Mix_Playing(id)) Mix_HaltChannel(id);
			else m_pMapMusicClips.at(id).pMixChunk.reset(Mix_LoadWAV(m_pMapMusicClips.at(id).filename.c_str()));

			Mix_Chunk* pMusic = m_pMapMusicClips.at(id).pMixChunk.get();

			mapLock.unlock();
			////////////

			Mix_Volume(id, info.volume);

			Mix_PlayChannel(id, pMusic, info.repeat ? -1 : 0);
		}
	}


	//SDLAudio
	SDLAudio::SDLAudio() :
		m_pImpl{ new SDLAudioImpl{} }
	{
	}

	SDLAudio::~SDLAudio()
	{
		delete m_pImpl;
	}

	void SDLAudio::PlaySoundClip(SoundID id, uint8_t volume, bool repeat) const
	{
		m_pImpl->PlaySoundClipImpl(id, volume, repeat);
	}

	uint8_t SDLAudio::GetVolume(SoundID id) const
	{
		return m_pImpl->GetVolumeImpl(id);
	}

	void SDLAudio::SetVolume(SoundID id, uint8_t newVolume)
	{
		m_pImpl->SetVolumeImpl(id, newVolume);
	}

	void SDLAudio::SetMasterVolume(uint8_t newVolume)
	{
		m_pImpl->SetMasterVolumeImpl(newVolume);
	}

	void SDLAudio::ToggleMute()
	{
		m_pImpl->ToggleMuteImpl();
	}

	void SDLAudio::PauseSound(SoundID id) const
	{
		m_pImpl->PauseSoundImpl(id);
	}

	void SDLAudio::PauseAllSounds() const
	{
		m_pImpl->ResumeAllSoundsImpl();
	}

	void SDLAudio::ResumeSound(SoundID id) const
	{
		m_pImpl->ResumeSoundImpl(id);
	}

	void SDLAudio::ResumeAllSounds() const
	{
		m_pImpl->ResumeAllSoundsImpl();
	}

	void SDLAudio::StopSound(SoundID id) const
	{
		m_pImpl->StopSoundImpl(id);
	}

	void SDLAudio::AddSound(const std::string& filename, SoundID id)
	{
		m_pImpl->AddSoundImpl(filename, id);
	}

	void SDLAudio::StopAllSounds() const
	{
		m_pImpl->StopAllSoundsImpl();
	}
	
}
