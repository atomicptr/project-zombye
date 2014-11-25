#ifndef __ZOMBYE_COMMAND_HPP__
#define __ZOMBYE_COMMAND_HPP__

namespace zombye {
    class command {
    public:
        virtual void execute() = 0;
    };
}

#endif
