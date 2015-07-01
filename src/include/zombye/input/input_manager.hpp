#ifndef __ZOMBYE_INPUT_MANAGER_HPP__
#define __ZOMBYE_INPUT_MANAGER_HPP__

#include <cmath>
#include <unordered_map>
#include <utility>
#include <functional>
#include <string>
#include <queue>

namespace zombye {
    class button;
    class input_system;
}

namespace zombye {
    class input_manager {
    public:
        input_manager(input_system*);

        void register_event(std::string, button&);
        void register_keyboard_event(std::string, std::string);

        void register_action(std::string, std::function<void()>);

        void handle_input();

    private:
        input_system *input_;

        std::unordered_map<std::string, std::function<void()>> commands_;
        std::queue<std::function<void()>> event_queue_;
    };
}

#endif
