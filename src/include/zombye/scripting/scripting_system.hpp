#ifndef __ZOMBYE_SCRIPTING_SYSTEM_HPP__
#define __ZOMBYE_SCRIPTING_SYSTEM_HPP__

#include <functional>
#include <memory>
#include <type_traits>
#include <vector>

#include <angelscript.h>
#include <scriptbuilder/scriptbuilder.h>

#include <zombye/ecs/entity.hpp>
#include <zombye/utils/logger.hpp>

namespace zombye {
	class game;
}

namespace zombye {
	class scripting_system {
	private:
		game& game_;
		std::unique_ptr<asIScriptEngine, void(*)(asIScriptEngine*)> script_engine_;
		std::unique_ptr<CScriptBuilder> script_builder_;
		std::vector<std::unique_ptr<asIScriptContext, void(*)(asIScriptContext*)>> context_pool_;
		std::vector<std::unique_ptr<asIScriptContext, void(*)(asIScriptContext*)>> used_context_;

	public:
		scripting_system(game& game);
		~scripting_system() = default;

		scripting_system(const scripting_system& rhs) = delete;
		scripting_system& operator=(const scripting_system& rhs) = delete;

		scripting_system(scripting_system&& rhs) = delete;
		scripting_system& operator=(scripting_system&& rhs) = delete;

		void begin_module(const std::string& module_name);
		void load_script(const std::string& file_name);
		void end_module();
		void exec(const std::string& function_decl, const std::string& module_name);
		void exec();
		void prepare(asIScriptFunction& function);

		template <typename t>
		void argument(int position, t& arg);

		template <typename t>
		void argument(int position, t* arg) {
			allocate_context();
			used_context_.back()->SetArgObject(position, arg);
		}

		template <typename t>
		void register_global_object(const std::string& decl, const t& object) {
			auto result = script_engine_->RegisterGlobalProperty(decl.c_str(), object);
			if (result < 0) {
				throw std::runtime_error("could not register global property " + decl);
			}
		}

		template <typename t>
		void register_function(const std::string function_decl, const t& function) {
			auto result = script_engine_->RegisterGlobalFunction(function_decl.c_str(), asFUNCTION(function), asCALL_CDECL);
			if (result < 0) {
				throw std::runtime_error("Could not register function " + function_decl);
			}
		}

		template <typename t>
		void register_function(const std::string function_decl, std::function<t>& function) {
			auto result = script_engine_->RegisterGlobalFunction(function_decl.c_str(),
				asMETHOD(std::remove_reference<typename std::remove_const<decltype(function)>::type>::type, operator()),
				asCALL_THISCALL_ASGLOBAL,
				reinterpret_cast<void*>(&function));
			if (result < 0) {
				throw std::runtime_error("Could not register function " + function_decl);
			}
		}

		template <typename t>
		void register_type(const std::string& type_name) {
			auto result = script_engine_->RegisterObjectType(type_name.c_str(), sizeof(t), asOBJ_REF | asOBJ_NOCOUNT);
			if (result < 0) {
				throw std::runtime_error("Could not register type " + type_name);
			}
		}

		template <typename t>
		void register_pod_type(const std::string& type_name) {
			auto result = script_engine_->RegisterObjectType(type_name.c_str(), sizeof(t), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<t>());
			if (result < 0) {
				throw std::runtime_error("Could not register type " + type_name);
			}
		}

		template <typename t>
		void register_constructor(const std::string type_name, const std::string& function_decl, const t& function) {
			auto result = script_engine_->RegisterObjectBehaviour(type_name.c_str(), asBEHAVE_CONSTRUCT,
				function_decl.c_str(), asFUNCTION(function), asCALL_CDECL_OBJFIRST);
			if (result < 0) {
				throw std::runtime_error("Could not register constructor " + function_decl + " at type " + type_name);
			}
		}

		template <typename t>
		void register_factory(const std::string& type_name, const std::string& function_decl, std::function<t>& function) {
			auto result = script_engine_->RegisterObjectBehaviour(type_name.c_str(), asBEHAVE_FACTORY,
				function_decl.c_str(),
				asMETHOD(std::remove_reference<typename std::remove_const<decltype(function)>::type>::type, operator()),
				asCALL_THISCALL_ASGLOBAL,
				reinterpret_cast<void*>(&function));
			if (result < 0) {
				throw std::runtime_error("Could not register factory function " + function_decl + " at type " + type_name);
			}
		}

		template <typename t>
		void register_factory(const std::string& type_name, const std::string& function_decl, const t& function) {
			auto result = script_engine_->RegisterObjectBehaviour(type_name.c_str(), asBEHAVE_FACTORY,
				function_decl.c_str(), asFUNCTION(function), asCALL_CDECL);
			if (result < 0) {
				throw std::runtime_error("Could not register factory function " + function_decl + " at type " + type_name);
			}
		}

		void register_destructor(const std::string& type_name, void(*function)(void*));
		void register_member(const std::string& type_name, const std::string& member_decl, size_t offset);

		template <typename t>
		void register_member_function(const std::string& type_name, const std::string& function_decl, const t& function, asDWORD call_conv = asCALL_CDECL_OBJFIRST) {
			auto result = script_engine_->RegisterObjectMethod(type_name.c_str(), function_decl.c_str(), asFUNCTION(function), call_conv);
			if (result < 0) {
				throw std::runtime_error("Could not register member function " + function_decl + " at type " + type_name);
			}
		}

		auto& script_engine() {
			return *script_engine_;
		}

	private:
		void allocate_context();

		void register_glm();
	};
}

#endif
