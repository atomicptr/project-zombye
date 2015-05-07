#ifndef __ZOMBYE_STATE_MACHINE_HPP__
#define __ZOMBYE_STATE_MACHINE_HPP__

#include <unordered_map>
#include <memory>
#include <string>
#include <utility>

namespace zombye {
    class game;
    class logger;
    class state;
}

namespace zombye {
    class state_machine {
    public:
        state_machine(game*);
        ~state_machine();

        template<typename T, typename... Arguments>
        void add(std::string name, Arguments... args) {
            auto ptr = std::unique_ptr<T>(new T(this, std::forward<Arguments>(args)...));
            states_.insert(std::make_pair(name, std::move(ptr)));
        }

        void remove(std::string);
        void use(std::string);
        bool has(std::string) const;
        void dispose_current();

        void update(float) const;

        state* current() const noexcept;
        zombye::game* get_game() const;
    private:
        std::unordered_map<std::string, std::unique_ptr<state>> states_;
        state *current_ = nullptr;

        zombye::game* game_;
    };
}

#endif
