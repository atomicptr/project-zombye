#include <zombye/utils/json_type_helper.hpp>

namespace zombye {
    template<>
    bool get<Json::ValueType::booleanValue>(const Json::Value& value) {
        return value.asBool();
    }

    template<>
    int get<Json::ValueType::intValue>(const Json::Value& value) {
        return value.asInt();
    }

    template<>
    float get<Json::ValueType::realValue>(const Json::Value& value) {
        return value.asFloat();
    }

    template<>
    std::string get<Json::ValueType::stringValue>(const Json::Value& value) {
        return value.asString();
    }
}