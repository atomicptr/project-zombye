#ifndef __ZOMBYE_GAME_HPP__
#define __ZOMBYE_GAME_HPP__

#include <string>
#include <memory>

#include <SDL2/SDL.h>

#include <zombye/input/input_system.hpp>
#include <zombye/audio/audio_system.hpp>
#include <zombye/config/config_system.hpp>
#include <zombye/ecs/entity_manager.hpp>
#include <zombye/gameplay/gameplay_system.hpp>
#include <zombye/gameplay/game_states.hpp>
#include <zombye/utils/sdlhelper.hpp>
#include <zombye/utils/logger.hpp>
#include <zombye/utils/os.h>

namespace zombye {
    class gameplay_system;
}

namespace zombye {
    class game {
    public:
        game(std::string);
        ~game();

        void update(float delta_time);
        void run();
        void quit();
        void register_components();

        int width() const;
        int height() const;

        zombye::entity_manager& entity_manager() noexcept {
            return *entity_manager_;
        }

        input_system* input();
        audio_system* audio();
        gameplay_system* gameplay();
        config_system* config();
    private:
        std::string title_;
        int width_;
        int height_;

        bool running_;
        bool fullscreen_;

        std::unique_ptr<zombye::config_system> config_system_;
        std::unique_ptr<zombye::entity_manager> entity_manager_;
        std::unique_ptr<input_system> input_system_;
        std::unique_ptr<audio_system> audio_system_;
        std::unique_ptr<gameplay_system> gameplay_system_;
    };
}

// easter egg :P
int glCreateGame(const char*);

#endif
