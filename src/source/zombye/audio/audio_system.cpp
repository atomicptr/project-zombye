#include <zombye/audio/audio_system.hpp>

zombye::audio_system::audio_system() {
    zombye::log("init audio system");

    if(Mix_Init(MIX_INIT_OGG) != MIX_INIT_OGG) {
        zombye::log(LOG_ERROR, "Error while trying to init SDL_mixer: " + std::string(Mix_GetError()));
    }

    if(Mix_OpenAudio(AUDIO_FREQUENCY, MIX_DEFAULT_FORMAT, AUDIO_CHANNELS, AUDIO_CHUNKSIZE)) {
        zombye::log(LOG_ERROR, "Error while trying to open audio: " + std::string(Mix_GetError()));
    }

    Mix_AllocateChannels(AUDIO_NUM_CHANNELS);

    music_playing_ = false;

    music_collection_.add("test", "music/test.ogg");
    sound_collection_.add("test", "sound/test.ogg");
}

zombye::audio_system::~audio_system() {
    Mix_CloseAudio();
}

void zombye::audio_system::set_music_volume(unsigned int vol) const {
    if(vol > 128) {
        vol = 128;
    }

    Mix_VolumeMusic(vol);
}

void zombye::audio_system::set_sound_volume(unsigned int vol, int channel) const {
    if(vol > 128) {
        vol = 128;
    }

    Mix_Volume(channel, vol);
}

void zombye::audio_system::play_music(std::string name, int loop, int fade_in, int fade_out) {
    Mix_FadeOutMusic(fade_out);

    auto music = music_collection_.get(name);

    if(Mix_FadeInMusic(music, loop, fade_in) == -1) {
        zombye::log(LOG_ERROR, "Could not play music: " + std::string(Mix_GetError()));
    } else {
        music_playing_ = true;
    }
}

void zombye::audio_system::stop_music() {
    Mix_HaltMusic();
    music_playing_ = false;
}

int zombye::audio_system::play_sound(std::string name, int fade_in) {
    auto sound = sound_collection_.get(name);

    auto channel = Mix_FadeInChannel(-1, sound, 0, fade_in);

    if(channel == -1) {
        zombye::log(LOG_ERROR, "Could not play sound: " + std::string(Mix_GetError()));
    }

    return channel;
}

void zombye::audio_system::stop_sound(int channel, int fade_out) {
    if(channel < -1) {
        return;
    }

    Mix_FadeOutChannel(channel, fade_out);
}