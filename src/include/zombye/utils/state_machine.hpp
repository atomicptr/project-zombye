#ifndef __ZOMBYE_STATE_MACHINE_HPP__
#define __ZOMBYE_STATE_MACHINE_HPP__

#include <zombye/core/game.hpp>
#include <zombye/utils/state.hpp>

#include <zombye/utils/logger.hpp>

#include <unordered_map>
#include <memory>
#include <string>
#include <utility>

namespace zombye {
    class game;

    class state_machine {
    public:
        state_machine(game* game) : game_(game) {}

        ~state_machine() {
            if(current_ != nullptr) {
                current_->leave();
            }
        }

        template<typename T, typename... Arguments>
        void add(std::string name, Arguments... args) {
            auto s = std::make_shared<T>(this, std::forward<Arguments>(args)...);
            states.emplace(std::make_pair(name, s));
        }

        void remove(std::string name) {
            states.erase(name);
        }

        void use(std::string name) {
            if(has(name)) {
                if(current_ != nullptr) {
                    current_->leave();
                }

                current_ = states.at(name).get();
                current_->enter();
            } else {
                zombye::log(LOG_ERROR, "Unknown state: " + name);
            }
        }

        bool has(std::string name) {
            auto it = states.find(name);

            return it != states.end();
        }

        void update(float delta_time) const {
            if(current_ != nullptr) {
                current_->update(delta_time);
            }
        }

        state* current() const noexcept {
            return current_;
        }

        zombye::game* game() const {
            return game_;
        }
    private:
        std::unordered_map<std::string, std::shared_ptr<state>> states;
        state *current_ = nullptr;

        zombye::game* game_;
    };
}

#endif