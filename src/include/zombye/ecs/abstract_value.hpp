#ifndef __ZOMBYE_ABSTRACT_VALUE_HPP__
#define __ZOMBYE_ABSTRACT_VALUE_HPP__

namespace zombye {
    class component;
    class abstract_value {
    public:
        virtual void assign(component* owner) = 0;
    };
}

#endif