#include "AudioLocator.h"

namespace dae
{
	std::unique_ptr<AudioService> AudioLocator::m_Instance{ std::make_unique<NullAudio>() };
}