#ifndef __ZOMBYE_INPUT_MANAGER_HPP__
#define __ZOMBYE_INPUT_MANAGER_HPP__

#include <cmath>
#include <unordered_map>
#include <utility>
#include <string>

#include <zombye/input/input_system.hpp>
#include <zombye/input/button.hpp>
#include <zombye/gameplay/command.hpp>

namespace zombye {
    class input_system;

    class input_manager {
    public:
        input_manager(const input_system*);

        void register_event(std::string, button&);
        void register_command(std::string, command*);

    private:
        const input_system *input_;

        std::unordered_map<std::string, command*> commands_;
    };
}

#endif
