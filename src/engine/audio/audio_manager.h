#ifndef _ENGINE_AUDIO_AUDIO_MANAGER_H_
#define _ENGINE_AUDIO_AUDIO_MANAGER_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <SDL.h>

#include "resource.h"
#include "sound.h"

namespace engine {
namespace audio {

/*
 * This class provides audio functionality.
 */
class AudioManager {
private:
	// the used audio device's name
	std::string device_name;

	// the audio output format
	SDL_AudioSpec device_spec;
	// the used audio device's id
	SDL_AudioDeviceID device_id;

	std::unique_ptr<int32_t[]> mix_buffer;

public:
	AudioManager(int freq, SDL_AudioFormat format, uint8_t channels,
			uint16_t samples);

	// pass empty device name to indicate, that the default device should be
	// used
	AudioManager(const std::string &device_name, int freq,
			SDL_AudioFormat format, uint8_t channels, uint16_t samples);
	
	~AudioManager();

	// delete copy and move constructors and operators
	AudioManager(const AudioManager&) = delete;
	AudioManager(AudioManager&&) = delete;

	AudioManager &operator=(const AudioManager&) = delete;
	AudioManager &operator=(AudioManager&&) = delete;

	void audio_callback(int16_t *stream, int len);

	/**
	 * Returns the currently used audio output format.
	 */
	SDL_AudioSpec get_device_spec() const;

	/**
	 * Returns a vector of all available device names.
	 */
	static std::vector<std::string> get_devices();

	/**
	 * Returns a vector of all available driver names.
	 */
	static std::vector<std::string> get_drivers();

	/**
	 * Returns the name of the currently used driver.
	 */
	static std::string get_current_driver();
};

}
}

#endif