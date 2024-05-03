#include "SDLAudio.h"
#include <SDL_mixer.h>
#include <map>
#include <queue>
#include <memory>
#include <functional>
#include <thread>
#include "ResourceManager.h"
#include <mutex>

namespace dae
{
	//Implementation
	class SDLAudio::SDLAudioImpl
	{
	public:
		SDLAudioImpl() { m_Thread = std::jthread{ &SDLAudio::SDLAudioImpl::HandleRequests, this}; }
		~SDLAudioImpl() {m_ServiceIsActive = false;}

		SDLAudioImpl(const SDLAudioImpl&) = delete;
		SDLAudioImpl(SDLAudioImpl&&) noexcept = delete;
		SDLAudioImpl& operator= (const SDLAudioImpl&) = delete;
		SDLAudioImpl& operator= (SDLAudioImpl&&) noexcept = delete;

		void AddSoundImpl(const std::string& filename, SoundID id);
		void PlaySoundClipImpl(SoundID id, uint8_t volume, bool repeat);
		uint8_t GetVolumeImpl(SoundID id) const;
		void SetVolumeImpl(SoundID id, uint8_t newVolume);
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
			uint8_t volume;
			bool repeat;
		};

		void HandleRequests();

		bool m_ServiceIsActive{ true };
		std::string m_DataPath{ ResourceManager::GetInstance().GetDataPath() };

		std::jthread m_Thread;
		std::mutex m_EventsMutex;
		std::mutex m_MapMutex;

		std::map<SoundID, AudioClip> m_pMapMixMusics{};
		std::queue<SoundID> m_Events{};
	};


	void SDLAudio::SDLAudioImpl::AddSoundImpl(const std::string& filename, SoundID id)
	{
		AudioClip clip;
		clip.filename = m_DataPath + filename;

		std::lock_guard<std::mutex> lk{ m_MapMutex };
		m_pMapMixMusics[id] = std::move(clip);
	}
	void SDLAudio::SDLAudioImpl::PlaySoundClipImpl(SoundID id, uint8_t volume, bool repeat)
	{
		std::lock_guard<std::mutex> mapLock{ m_MapMutex };

		if (m_pMapMixMusics.contains(id))
		{
			m_pMapMixMusics.at(id).volume = volume;
			m_pMapMixMusics.at(id).repeat = repeat;

			std::lock_guard<std::mutex> eventsLock{ m_EventsMutex };
			m_Events.push(id);
		}
		
	}
	uint8_t SDLAudio::SDLAudioImpl::GetVolumeImpl(SoundID id) const
	{
		if (m_pMapMixMusics.at(id).pMixChunk != nullptr)
		{
			return static_cast<uint8_t>( Mix_Volume(id, -1) );
		}
		return 0;
	}
	void SDLAudio::SDLAudioImpl::SetVolumeImpl(SoundID id, uint8_t newVolume)
	{
		if (m_pMapMixMusics.at(id).pMixChunk != nullptr)
		{
			Mix_Volume(id, newVolume);
		}
	}
	void SDLAudio::SDLAudioImpl::PauseSoundImpl(SoundID id) const
	{
		Mix_Pause(id);
	}
	void SDLAudio::SDLAudioImpl::PauseAllSoundsImpl() const
	{
		Mix_Pause(-1);
	}
	void SDLAudio::SDLAudioImpl::ResumeSoundImpl(SoundID id) const
	{
		Mix_Resume(id);
	}
	void SDLAudio::SDLAudioImpl::ResumeAllSoundsImpl() const
	{
		Mix_Resume(-1);
	}
	void SDLAudio::SDLAudioImpl::StopSoundImpl(SoundID id) const
	{
		Mix_HaltChannel(id);
	}
	void SDLAudio::SDLAudioImpl::StopAllSoundsImpl() const
	{
		Mix_HaltChannel(-1);
	}

	void SDLAudio::SDLAudioImpl::HandleRequests()
	{
		while (m_ServiceIsActive)
		{
			std::unique_lock<std::mutex> eventsLock{m_EventsMutex};
			if (m_Events.empty()) continue;

			SoundID id = m_Events.front();
			m_Events.pop();
			eventsLock.unlock();


			std::lock_guard<std::mutex> mapLock{ m_MapMutex };
			auto& audioClip = m_pMapMixMusics.at(id);

			if (Mix_Playing(id)) Mix_HaltChannel(id);
			else audioClip.pMixChunk.reset(Mix_LoadWAV(audioClip.filename.c_str()));

			Mix_Volume(id, audioClip.volume);

			Mix_PlayChannel(id, audioClip.pMixChunk.get(), audioClip.repeat ? -1 : 0);
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
