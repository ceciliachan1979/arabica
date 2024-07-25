#pragma once

#include <SDL2/SDL.h>
#include <cmath>

namespace arabica {

class Sound {
public:
  static uint32_t s_sample_index;

  Sound(uint32_t sample_rate = 44100, uint32_t frequency = 440, int16_t volume = 3000)
    : _device(0)
    , sample_rate(sample_rate)
    , frequency(frequency)
    , volume(volume) {
  }

  bool init() {
    // The following comments source from: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.5
    //
    // Chip-8 provides a sound timer.
    //
    // The sound timer is active whenever the sound timer register (ST) is non-zero.
    // This timer also decrements at a rate of 60Hz, however,
    // as long as ST's value is greater than zero, the Chip-8 buzzer will sound.
    // When ST reaches zero, the sound timer deactivates.
    //
    // The sound produced by the Chip-8 interpreter has only one tone.
    // The frequency of this tone is decided by the author of the interpreter.
    //
    SDL_zero(_desired_spec);
    {
      _desired_spec.freq     = sample_rate;    // https://en.wikipedia.org/wiki/44,100_Hz
      _desired_spec.format   = AUDIO_S16LSB;   //
      _desired_spec.channels = 1;              // mono
      _desired_spec.samples  = 512;            // Buffer size in samples per channel
      _desired_spec.callback = audio_callback; //
      _desired_spec.userdata = this;           //
    }

    _device = SDL_OpenAudioDevice(nullptr, 0, &_desired_spec, &_spec, 0);
    if (_device == 0) {
      SDL_Log("Failed to open audio: %s", SDL_GetError());
      return false;
    }

    return true;
  }

  ~Sound() {
    if (_device != 0) {
      SDL_CloseAudioDevice(_device);
    }
    SDL_Quit();
  }

  void tick() {
    if (frequency > 0) {
      frequency--;
      if (frequency == 0) {
        stop_beep();
      }
    }
  }

  void start_beep() {
    if (_device != 0) {
      SDL_PauseAudioDevice(_device, 0);
    }
  }

  void stop_beep() {
    if (_device != 0) {
      SDL_PauseAudioDevice(_device, 1);
    }
  }

  uint32_t sample_rate;
  uint32_t frequency;
  int16_t  volume;

private:
  SDL_AudioDeviceID _device;
  SDL_AudioSpec     _spec{};
  SDL_AudioSpec     _desired_spec{};

  // The following implementation source from the function which in the following GitHub repository
  // link: https://github.com/queso-fuego/chip8_emulator_c/blob/master/chip8.c#L98
  static void audio_callback(void* const userdata, Uint8* const stream, const int len) {
    auto* const sound = static_cast<Sound*>(userdata);

    int16_t* const audio_data              = reinterpret_cast<int16_t*>(stream);
    const int32_t  square_wave_period      = sound->sample_rate / sound->frequency;
    const int32_t  half_square_wave_period = square_wave_period >> 1;

    for (int i = 0; i < (len >> 1); i++) {
      audio_data[i] = ((s_sample_index++ / half_square_wave_period) % 2) ? sound->volume : -sound->volume;
    }
  }
};

inline uint32_t Sound::s_sample_index = 0;

} // namespace arabica
