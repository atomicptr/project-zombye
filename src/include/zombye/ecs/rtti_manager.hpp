#ifndef __ZOMBYE_PROPERTY_MANAGER_HPP__
#define __ZOMBYE_PROPERTY_MANAGER_HPP__

#include <string>
#include <unordered_map>

#include <zombye/ecs/rtti.hpp>

namespace zombye {
	class rtti_manager {
		static std::unordered_map<std::string, rtti*>& rttis();
	public:
		static void register_type(rtti* type_info);

		static rtti* type_info(const std::string& name);
	};
}

#endif