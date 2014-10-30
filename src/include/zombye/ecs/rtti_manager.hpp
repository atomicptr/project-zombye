#ifndef __ZOMBYE_PROPERTY_MANAGER_HPP__
#define __ZOMBYE_PROPERTY_MANAGER_HPP__

#include <string>
#include <unordered_map>

#include <zombye/ecs/rtti.hpp>

namespace zombye {
	class rtti_manager {
		static std::unordered_map<std::string, rtti*> rttis;
	public:
		static void register_type(rtti* type_info) {
			auto pos = rttis.find(type_info->type_name());
			if (pos == rttis.end()) {
				rttis.insert(std::make_pair(type_info->type_name(), type_info));
			}
		}

		static rtti* type_info(const std::string& name) {
			auto pos = rttis.find(name);
			if (pos != rttis.end()) {
				return rttis[name];
			}
			return nullptr;
		}
	};
}

#endif