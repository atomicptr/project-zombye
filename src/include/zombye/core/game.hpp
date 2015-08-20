#ifndef __ZOMBYE_GAME_HPP__
#define __ZOMBYE_GAME_HPP__

#include <string>
#include <memory>

#include <SDL2/SDL.h>

namespace zombye {
    class animation_system;
    class asset_manager;
    class audio_system;
    class config_system;
    class entity_manager;
    class gameplay_system;
    class game_state;
    class input_system;
    class physics_system;
    class rendering_system;
    class logger;
    class scripting_system;
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

        auto& asset_manager() noexcept {
            return *asset_manager_;
        }

        auto& entity_manager() noexcept {
            return *entity_manager_;
        }

        auto& animation_system() noexcept {
            return *animation_system_;
        }

        auto& rendering_system() noexcept {
            return *rendering_system_;
        }

        auto& scripting_system() noexcept {
            return *scripting_system_;
        }

        input_system* input();
        audio_system* audio();
        gameplay_system* gameplay();
        config_system* config();
        physics_system* physics();
    private:
        std::string title_;
        int width_;
        int height_;

        bool running_;
        bool fullscreen_;

        std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window_;
        std::unique_ptr<zombye::asset_manager> asset_manager_;

        std::unique_ptr<zombye::config_system> config_system_;
        std::unique_ptr<zombye::scripting_system> scripting_system_;
        std::unique_ptr<input_system> input_system_;
        std::unique_ptr<audio_system> audio_system_;
        std::unique_ptr<physics_system> physics_system_;
        std::unique_ptr<zombye::animation_system> animation_system_;
        std::unique_ptr<zombye::rendering_system> rendering_system_;
        std::unique_ptr<gameplay_system> gameplay_system_;

        // This MUST stay the last thing here, thank you :P
        std::unique_ptr<zombye::entity_manager> entity_manager_;

        void update_fps(float delta_time);
    };
}

// easter egg :P
int glCreateGame(const char*);

#endif
