#ifndef __ZOMBYE_AUDIO_SYSTEM_HPP__
#define __ZOMBYE_AUDIO_SYSTEM_HPP__

// if sound stutters increase chunk size
#define AUDIO_CHUNKSIZE 4096
#define AUDIO_FREQUENCY 44100
#define AUDIO_CHANNELS 2
#define AUDIO_NUM_CHANNELS 30

#include <string>
#include <memory>

#include <SDL2/SDL_mixer.h>

#include <zombye/audio/music_collection.hpp>
#include <zombye/audio/sound_collection.hpp>

#include <zombye/assets/asset.hpp>
#include <zombye/assets/asset_manager.hpp>
#include <zombye/utils/logger.hpp>

namespace zombye {
    class audio_system {
    public:
        audio_system();
        ~audio_system();

        void set_music_volume(unsigned int) const;
        void set_sound_volume(unsigned int, int) const;

        void play_music(std::string, int loop=-1, int fade_in=0, int fade_out=0);
        void stop_music();

        int play_sound(std::string, int fade_in=0);
        void stop_sound(int, int fade_out=0);

    private:
        bool music_playing_;

        music_collection music_collection_;
        sound_collection sound_collection_;
    };
}

#endif